## Using Postgres advisory locks for app locking

Here's a helper module to use Postgres advisory locks for arbitrary key based locking at app level.



### `pgLock.ts`

```typescript
import { DatabaseError } from 'pg'
import { refineError } from 'src/lib/utils'
import pool, { DatabaseTransactionConnection } from 'src/slonik'
import { sql, SqlFragment } from 'slonik'

// The lock identifier patterns need to be unique across the app.
// Having all defined in one place helps to spot potential overlap.
export const lockKeyCreators = {
  someOperation: (key: string) => `someOperation/${key}/`
}

export class ExclusiveAdvisoryLockAcquireError extends Error {}
export class TransactionIdleTimeoutError extends Error {}

// 0 to disable transaction retries
const TRANSACTION_RETRY_LIMIT = 0

type LockTransactionOptions = {
  lockAcquireTimeout?: SqlFragment
  idleInTransactionTimeout?: SqlFragment
}
/**
 * Provides the same API as Slonik's pool.transaction but acquires and releases a PG advisory lock automatically.
 *
 * We use a transaction-based lock for release safety: it's automatically released once the transaction ends.
 * The caller doesn't _have_ to do all operations within the transaction if only lock protection is required.
 *
 * @see https://www.postgresql.org/docs/15/functions-admin.html#FUNCTIONS-ADVISORY-LOCKS
 */
export async function withExclusiveAdvisoryLockTransaction<T>(
  key: string,
  fn: (transaction: DatabaseTransactionConnection) => Promise<T>,
  {
    lockAcquireTimeout = sql`1s`,
    idleInTransactionTimeout = sql`30s`,
  }: LockTransactionOptions = {},
): Promise<T> {
  const result = await pool.transaction(async transaction => {
    try {
      // "Terminate any session that has been idle (that is, waiting for a client query) within an open transaction for
      // longer than the specified amount of time."
      // "This option can be used to ensure that idle sessions do not hold locks for an unreasonable amount of time."
      await transaction.query(
        sql`SET LOCAL idle_in_transaction_session_timeout = '${idleInTransactionTimeout}'`,
      )

      // Set the statement_timeout temporarily to 0 (inf) because we want everything to be driven by the lock_timeout.
      await transaction.query(sql`SET LOCAL statement_timeout = 0`)

      // Set a separate statement timeout for lock acquire operation.
      // Usually it's good to keep to lock acquire timeout low. For example in pubsub message handling:
      // when acquire timeout is low, we don't accidentally clog our processing by causing all message handlers
      // to wait for a lock (in case we happen to process a ton of messages competing for the same lock at once).
      await transaction.query(
        sql`SET LOCAL lock_timeout = '${lockAcquireTimeout}'`,
      )

      // We are aware that the hashing method (hash(arbitrary string) -> bigint)
      // is making collisions more likely than even plain md5. However,
      // collisions only lower throughput, not compromise the mutual exclusion guarantee.
      // Online examples used both md5 and sha1, but md5 doesn't need pgcrypto.
      // Also: yes, the `'x' ||` trick is weird but intentional, see: https://postgres.cz/wiki/PostgreSQL_SQL_Tricks_II#Conversion_between_hex_and_dec_numbers
      await transaction.query(sql`
        -- Lock is released when transaction ends
        SELECT pg_advisory_xact_lock(
          ('x' || md5(${key}))::bit(64)::bigint
        )
      `)
    } catch (err) {
      if (isLockAcquireError(err)) {
        throw refineError(
          err,
          new ExclusiveAdvisoryLockAcquireError(
            `Acquiring exclusive advisory lock with key '${key}' failed`,
          ),
        )
      }
      throw err
    }
    // Return statement timeout back to default for actual operation
    await transaction.query(sql`SET LOCAL statement_timeout = DEFAULT`)

    try {
      return await fn(transaction)
    } catch (err) {
      if (isTransactionIdleTimeoutError(err)) {
        throw refineError(
          err,
          new TransactionIdleTimeoutError(
            `Transaction idle timeout exceeded while holding '${key}' lock`,
          ),
        )
      }
      throw err
    }
  }, TRANSACTION_RETRY_LIMIT)
  return result
}

/**
 * Provides the same API as Slonik's pool.transaction but acquires and releases a PG advisory lock automatically.
 *
 * We use a transaction-based lock for release safety: it's automatically released once the transaction ends.
 * The caller doesn't _have_ to do all operations within the transaction if only lock protection is required.
 *
 * @see https://www.postgresql.org/docs/15/functions-admin.html#FUNCTIONS-ADVISORY-LOCKS
 */
export async function withTryExclusiveAdvisoryLockTransaction<T>(
  key: string,
  fn: (transaction: DatabaseTransactionConnection) => Promise<T>,
  { idleInTransactionTimeout = sql`30s` }: LockTransactionOptions = {}, // note: lockAcquireTimeout unused
): Promise<T> {
  const result = await pool.transaction(async transaction => {
    // "Terminate any session that has been idle (that is, waiting for a client query) within an open transaction for
    // longer than the specified amount of time."
    // "This option can be used to ensure that idle sessions do not hold locks for an unreasonable amount of time."
    await transaction.query(
      sql`SET LOCAL idle_in_transaction_session_timeout = '${idleInTransactionTimeout}'`,
    )

    // We are aware that the hashing method (hash(arbitrary string) -> bigint)
    // is making collisions more likely than even plain md5. However,
    // collisions only lower throughput, not compromise the mutual exclusion guarantee.
    // Online examples used both md5 and sha1, but md5 doesn't need pgcrypto.
    // Also: yes, the `'x' ||` trick is weird but intentional, see: https://postgres.cz/wiki/PostgreSQL_SQL_Tricks_II#Conversion_between_hex_and_dec_numbers
    if (
      !(await transaction.oneFirst(sql`
        -- Lock is released when transaction ends
        SELECT pg_try_advisory_xact_lock(
          ('x' || md5(${key}))::bit(64)::bigint
        )
      `))
    ) {
      throw new ExclusiveAdvisoryLockAcquireError(
        `Acquiring exclusive advisory lock with key '${key}' failed`,
      )
    }

    // Return statement timeout back to default for actual operation
    await transaction.query(sql`SET LOCAL statement_timeout = DEFAULT`)

    try {
      return await fn(transaction)
    } catch (err) {
      if (isTransactionIdleTimeoutError(err)) {
        throw refineError(
          err,
          new TransactionIdleTimeoutError(
            `Transaction idle timeout exceeded while holding '${key}' lock`,
          ),
        )
      }
      throw err
    }
  }, TRANSACTION_RETRY_LIMIT)
  return result
}

function isLockAcquireError(
  err: unknown,
): err is ExclusiveAdvisoryLockAcquireError {
  return (
    err instanceof DatabaseError &&
    err.message.includes('canceling statement due to lock timeout')
  )
}

function isTransactionIdleTimeoutError(
  err: unknown,
): err is TransactionIdleTimeoutError {
  // Unfortunately pg or slonik don't give any more details of the error. I tried to manually dig _any_
  // property that could be a better identifier, but this generic message was the only hint.
  // In the context of this lock usage, it's safe to assume that this error message will _likely_
  // mean that we hit transaction idle timeout. It's not 100% proof as this error AFAIK is used for other purposes too.
  return (
    err instanceof Error &&
    typeof err.message === 'string' && // seen in the wild: InvalidInputError for which typeof err.message === 'object' :melting-face:
    err.message.includes(
      'Client has encountered a connection error and is not queryable',
    )
  )
}

/**
 * Searches for an active advisory lock with given `key` string.
 * If the lock is active, its data is returned. Otherwise null is returned.
 */
export async function maybeFindExclusiveAdvisoryLockByKey(key: string) {
  /**
   * "A bigint key is displayed with its high-order half in the classid column, its low-order half in the objid column,
   *  and objsubid equal to 1. The original bigint value can be reassembled with the expression
   *  `(classid::bigint << 32) | objid::bigint`."
   *
   * @see https://www.postgresql.org/docs/15/view-pg-locks.html
   */
  return await pool.maybeOne<{
    lockBigintId: number | null // when null -> too large value to fit JS number
    locktype: 'advisory'
  }>(sql`
    WITH bigint_advisory_locks as (
      SELECT *, ((classid::bigint << 32) | objid::bigint) as lock_bigint_id FROM pg_locks WHERE
      locktype = 'advisory' AND
      objsubid = 1
    )
    SELECT * from bigint_advisory_locks
    WHERE lock_bigint_id = ('x' || md5(${key}))::bit(64)::bigint
  `)
}
```


### Tests 

```typescript
import _ from 'lodash'
import { DatabaseError } from 'pg'
import { QueryFunction } from 'slonik/dist/src/types'
import { waitForExpect } from 'src/lib/testing/testUtil'
import { promiseForEachSeries, sleep } from 'src/lib/utils'
import pool from 'src/slonik'
import {
  ExclusiveAdvisoryLockAcquireError,
  maybeFindExclusiveAdvisoryLockByKey,
  TransactionIdleTimeoutError,
  withExclusiveAdvisoryLockTransaction,
  withTryExclusiveAdvisoryLockTransaction,
} from 'src/slonik/pgLock'
import { sql } from 'slonik'

async function waitForLockAcquired(key: string) {
  await waitForExpect(
    async () => {
      const lock = await maybeFindExclusiveAdvisoryLockByKey(key)
      expect(lock).not.toEqual(null)
    },
    { timeoutMs: 100 },
  )
}

async function executeConcurrentScenario({
  lockKey,
  withLockTransactionWrapper,
}: {
  lockKey?: string
  withLockTransactionWrapper: typeof withExclusiveAdvisoryLockTransaction
}) {
  const values: string[] = []
  const withTransactionFn = lockKey
    ? (fn: (...args: any[]) => any) => withLockTransactionWrapper(lockKey, fn)
    : // When no lockKey is defined, we use a regular transaction
      (fn: (...args: any[]) => any) => pool.transaction(fn, 0)

  let resolveTransactionStartedPromise: () => void
  const transactionStartedPromise = new Promise<void>(resolve => {
    resolveTransactionStartedPromise = resolve
  })

  const firstPromise = withTransactionFn(async () => {
    resolveTransactionStartedPromise()
    // Hold on to the lock for a short while by not ending the transaction immediately
    await new Promise(resolve => setTimeout(resolve, 500))
    values.push('FIRST')
  })

  // This guarantees that we have entered the first transaction
  await transactionStartedPromise

  // Now execute the second operation
  const secondTrxPromise = withTransactionFn(async () => {
    values.push('SECOND')
  })

  await Promise.all([firstPromise, secondTrxPromise])
  return values
}

function mockQueryWithinTransaction(
  fn: (opts: {
    args: Parameters<QueryFunction>
    originalQuery: QueryFunction
  }) => Promise<any>,
) {
  const originalPoolTransaction = pool.transaction
  jest.spyOn(pool, 'transaction').mockImplementation((handler, retryLimit) => {
    return originalPoolTransaction(trx => {
      const originalTrxQuery = trx.query
      jest.spyOn(trx, 'query').mockImplementation(async (...args) => {
        return await fn({ args, originalQuery: originalTrxQuery })
      })

      return handler(trx)
    }, retryLimit)
  })
}

describe('pgLock', () => {
  beforeEach(() => {
    jest.useRealTimers()
    jest.restoreAllMocks()
  })

  describe('withExclusiveAdvisoryLockTransaction', () => {
    test('makes sure concurrent application code is executed in correct order', async () => {
      const values = await executeConcurrentScenario({
        lockKey: 'arbitrary lock key',
        withLockTransactionWrapper: withExclusiveAdvisoryLockTransaction,
      })

      // If everything went as expected, the second update should not go through
      // before the first
      expect(values).toEqual(['FIRST', 'SECOND'])
    })

    test('prove that omitting the lock causes an undesirable order of execution', async () => {
      const values = await executeConcurrentScenario({
        lockKey: undefined,
        withLockTransactionWrapper: withExclusiveAdvisoryLockTransaction,
      })

      // The second value was pushed before the first one
      expect(values).toEqual(['SECOND', 'FIRST'])
    })

    test('concurrency fuzzy scenario', async () => {
      const iterations = 5
      // Note: if you increase the concurrency, we might run out of connections in pool
      const concurrency = 10

      const nums = [0]
      await promiseForEachSeries(_.range(iterations), () =>
        Promise.all([
          ..._.range(concurrency).map(() =>
            withExclusiveAdvisoryLockTransaction('fuzzy test key', async () => {
              const tail = _.last(nums)!
              // wait a bit to let other concurrent executions potentially mess up with the state
              await sleep(2)
              nums.push(tail + 1)
            }),
          ),
        ]),
      )

      // If the locking works, numbers should be in correct order
      expect(nums).toEqual(_.range(0, iterations * concurrency + 1))
    })

    describe('error handling', () => {
      describe('failing to acquire lock in given time throws', () => {
        test('using mocks setup', async () => {
          mockQueryWithinTransaction(({ args, originalQuery }) => {
            const sqlObj = args[0]

            // Simulate lock acquire timeout
            if (sqlObj.sql.includes('SELECT pg_advisory_xact_lock')) {
              throw new DatabaseError(
                'Error: canceling statement due to lock timeout',
                0, // length unused here
                'error', // name doesn't matter
              )
            }

            return originalQuery(...args)
          })

          await expect(
            withExclusiveAdvisoryLockTransaction('any key', async () => {
              // Have an assertion that always fails, because this should never be called
              expect(true).toBe(false)
            }),
          ).rejects.toThrow(ExclusiveAdvisoryLockAcquireError)
        })

        test('using real timers', async () => {
          const lockKey = 'some key' // note: using a different key than other tests just in case
          // Note: this will actually hold on to the lock for ~500ms even after the test case has executed
          const promise = withExclusiveAdvisoryLockTransaction(
            lockKey,
            async () => {
              // Hold on to the lock for a short while by not ending the transaction immediately
              await new Promise(resolve => setTimeout(resolve, 1000))
            },
          )

          await waitForLockAcquired(lockKey)

          await expect(
            withExclusiveAdvisoryLockTransaction(
              lockKey,
              async () => {
                // Have an assertion that always fails, because this should never be called
                expect(true).toBe(false)
              },
              {
                lockAcquireTimeout: sql`1ms`,
              },
            ),
          ).rejects.toThrow(ExclusiveAdvisoryLockAcquireError)
          await promise // required to allow pool.end() to go through
        })
      })

      test('transaction idle timeout error is detected by message', async () => {
        mockQueryWithinTransaction(({ args, originalQuery }) => {
          const sqlObj = args[0]

          // Simulate transaction idle timeout. Unfortunately pg or slonik are not passing any details of the
          // error actually being "idle-in-transaction timeout" error.
          if (sqlObj.sql.includes('SELECT 1')) {
            throw new Error(
              'Client has encountered a connection error and is not queryable',
            )
          }
          return originalQuery(...args)
        })

        await expect(
          withExclusiveAdvisoryLockTransaction(
            'any key',
            async trx => {
              const { value } = await trx.one<{ value: number }>(
                sql`SELECT 2 as value`,
              )
              expect(value).toEqual(2)

              // This should throw TransactionIdleTimeoutError
              await trx.query(sql`SELECT 1`)

              // Have an assertion that always fails, because this should never be called
              expect(true).toBe(false)
            },
            {
              lockAcquireTimeout: sql`1ms`,
            },
          ),
        ).rejects.toThrow(TransactionIdleTimeoutError)
      })

      test('other errors are thrown as is', async () => {
        class MyError extends Error {}

        await expect(
          withExclusiveAdvisoryLockTransaction(
            'another key',
            async () => {
              throw new MyError('test')
            },
            {
              lockAcquireTimeout: sql`10ms`,
            },
          ),
        ).rejects.toThrow(MyError)
      })
    })
  })

  describe('withTryExclusiveAdvisoryLockTransaction', () => {
    test('concurrent locking is not possible', async () => {
      await expect(
        executeConcurrentScenario({
          lockKey: 'arbitrary lock key',
          withLockTransactionWrapper: withTryExclusiveAdvisoryLockTransaction,
        }),
      ).rejects.toThrow(ExclusiveAdvisoryLockAcquireError)
    })

    test('prove that omitting the lock causes an undesirable order of execution', async () => {
      const values = await executeConcurrentScenario({
        lockKey: undefined,
        withLockTransactionWrapper: withTryExclusiveAdvisoryLockTransaction,
      })

      // The second value was pushed before the first one
      expect(values).toEqual(['SECOND', 'FIRST'])
    })

    test('concurrency fuzzy scenario', async () => {
      const iterations = 5
      // Note: if you increase the concurrency, we might run out of connections in pool
      const concurrency = 10

      const nums = [0]
      await promiseForEachSeries(_.range(iterations), () =>
        Promise.all([
          ..._.range(concurrency).map(async () => {
            // Retry 100 times before failing
            for (let i = 0; i < 100; ++i) {
              try {
                return await withTryExclusiveAdvisoryLockTransaction(
                  'fuzzy test key',
                  async () => {
                    const tail = _.last(nums)!
                    // wait a bit to let other concurrent executions potentially mess up with the state
                    await sleep(2)
                    nums.push(tail + 1)
                  },
                )
              } catch (err) {
                // eslint-disable-next-line jest/no-conditional-expect
                expect(err).toBeInstanceOf(ExclusiveAdvisoryLockAcquireError)
                // And then fall through for a next iteration
              }
            }

            expect.fail('This should not happen')
          }),
        ]),
      )

      // If the locking works, numbers should be in correct order
      expect(nums).toEqual(_.range(0, iterations * concurrency + 1))
    })

    describe('error handling', () => {
      describe('failing to acquire lock in given time throws', () => {
        test('using real timers', async () => {
          const lockKey = 'some key' // note: using a different key than other tests just in case
          // Note: this will actually hold on to the lock for ~500ms even after the test case has executed
          const promise = withTryExclusiveAdvisoryLockTransaction(
            lockKey,
            async () => {
              // Hold on to the lock for a short while by not ending the transaction immediately
              await new Promise(resolve => setTimeout(resolve, 1000))
            },
          )

          await waitForLockAcquired(lockKey)

          await expect(
            withTryExclusiveAdvisoryLockTransaction(
              lockKey,
              async () => {
                // Have an assertion that always fails, because this should never be called
                expect(true).toBe(false)
              },
              {
                lockAcquireTimeout: sql`1ms`,
              },
            ),
          ).rejects.toThrow(ExclusiveAdvisoryLockAcquireError)
          await promise // required to allow pool.end() to go through
        })
      })

      test('transaction idle timeout error is detected by message', async () => {
        mockQueryWithinTransaction(({ args, originalQuery }) => {
          const sqlObj = args[0]

          // Simulate transaction idle timeout. Unfortunately pg or slonik are not passing any details of the
          // error actually being "idle-in-transaction timeout" error.
          if (sqlObj.sql.includes('SELECT 1')) {
            throw new Error(
              'Client has encountered a connection error and is not queryable',
            )
          }
          return originalQuery(...args)
        })

        await expect(
          withTryExclusiveAdvisoryLockTransaction(
            'any key',
            async trx => {
              const { value } = await trx.one<{ value: number }>(
                sql`SELECT 2 as value`,
              )
              expect(value).toEqual(2)

              // This should throw TransactionIdleTimeoutError
              await trx.query(sql`SELECT 1`)

              // Have an assertion that always fails, because this should never be called
              expect(true).toBe(false)
            },
            {
              lockAcquireTimeout: sql`1ms`,
            },
          ),
        ).rejects.toThrow(TransactionIdleTimeoutError)
      })

      test('other errors are thrown as is', async () => {
        class MyError extends Error {}

        await expect(
          withTryExclusiveAdvisoryLockTransaction(
            'another key',
            async () => {
              throw new MyError('test')
            },
            {
              lockAcquireTimeout: sql`10ms`,
            },
          ),
        ).rejects.toThrow(MyError)
      })
    })
  })
})
```
