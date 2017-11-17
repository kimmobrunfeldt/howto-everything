# Gapless counter with Postgres

This is a guide how to implement a gapless counter for bookkeeping purposes.
According to the law of Finland, each bookkeeping transaction needs to have a unique number.
In addition to that, the number must be an gapless, increasing sequence of numbers.

**Example:**

* Valid: 1, 2, 3, 4, 5, ...
* Not valid: 1, 2, 4, 5, ...

Apparently this is one way to make sure companies haven't cheated in bookkeeping.


## Implementation

**We can't use Postgres sequences** for this purpose, because they are never rolled back.
Sequence guarantees to generate unique numbers, but [not a gapless sequence of numbers][1]. This is by design, because
gapless sequence has its performance drawbacks.

From PG docs:

> Because nextval and setval calls are never rolled back, sequence objects cannot be used if "gapless" assignment of sequence numbers is needed. It is possible to build gapless assignment by using exclusive locking of a table containing a counter; but this solution is much more expensive than sequence objects, especially if many transactions need sequence numbers concurrently.


### The solution

.. or at least one.

Create a table which has one bigint column, and one row of data. This table holds the value of the counter. Let's call this table `increment`. *In singular to implicate that it only has one row.*

| gapless_counter (bigint) |
|--------------------------|
| 1                        |


```sql
CREATE TABLE increment (
  gapless_counter bigint NOT NULL DEFAULT 0
) WITH (
  OIDS=FALSE
);

INSERT INTO increment (gapless_counter) VALUES (1);
```

You can also add [optional safety mechanism](#optional-safety-triggers) for this table.

The created table itself isn't enough to keep the sequence gapless. It's
important to use the table correctly. The rest of the guide is a bit use case dependent.
You could increment the counter inside a Postgres trigger but in our case, we're
using a transaction. We need to increment the counter, and use it's new value when inserting
a new row to another table (`stuff`).

The `stuff` table looks like this:

| id (bigint) | counter (bigint) | created_at (timestamp)        |
|-------------|------------------|-------------------------------|
| 1           | 1                | 2017-11-17 12:44:17.488974+02 |

```sql
CREATE SEQUENCE stuff_id_seq
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 9223372036854775807
  START 1
  CACHE 1;
CREATE TABLE stuff (
  id bigint NOT NULL DEFAULT nextval('stuff_id_seq'::regclass),
  counter bigint NOT NULL,
  created_at timestamp with time zone NOT NULL DEFAULT now(),
  CONSTRAINT stuff_pkey PRIMARY KEY (id)
) WITH (
  OIDS=FALSE
);
```

**Usage:**

1. Begin transaction
2. Lock `increment` table with [`ACCESS EXCLUSIVE` mode][2]. This will block even reads to increment table before the transaction is finished. It's necessary for our use case.
3. Increment `gapless_counter` in `increment` table and return the value to our application code (Node)
4. Insert a new row to `stuff` table. The data has the new value of `gapless_counter` inside it.
5. Commit transaction

You can simulate this with psql:

```sql
BEGIN;
LOCK TABLE increment IN ACCESS EXCLUSIVE MODE;

UPDATE increment SET gapless_counter = gapless_counter + 1;
INSERT INTO stuff (counter) SELECT (gapless_counter) FROM increment;

END;
```

As a test, you can launch another psql session to the database and run `SELECT * FROM increment` between the lock aquire and transaction end.
The call will not finish until the transaction has been either rolled back or commited.
**Note that by default, there is no timeout. It will block forever if the transaction isn't ended.**

To set a timeout for the current session, you can say `SET LOCAL lock_timeout = '30s';` right after `BEGIN` transaction. "The effects of SET LOCAL last only till the end of the current transaction, whether committed or not." [Source][3].

### Application code with Node

We're using [knex](http://knexjs.org/) library to use Postgres. The steps to
get the counter correctly incremented are quite strict and there can't be any
errors. It is better to wrap the process to a convenience function.

```js
function withCounter(func) {
  return knex.transaction((trx) => {
    return knex.raw('SET LOCAL lock_timeout = \'30s\'')
      .then(() => knex.raw('LOCK TABLE increment IN ACCESS EXCLUSIVE MODE'))
      .then(() => knex.raw('UPDATE increment SET gapless_counter = gapless_counter + 1 RETURNING gapless_counter'))
      .then(({ rows }) => func(trx, rows[0].gapless_counter));
  })
}
```

Then the actual usage shrinks to:

```js
// ... In a Promise chain
return withCounter((trx, counter) => {
  // Note that in this case we are not using the counter in application code
  // at all, but we could use it in our queries inside this withCounter `func`.
  return trx.raw('INSERT INTO stuff (counter) SELECT (gapless_counter) FROM increment');
});
```

If any error is raised inside the `func`, the gapless_counter will be correctly
rolled back to its previous state. No other concurrent session can read the
results which happen in the middle of the transaction.


That's it! As a reference, the full code to create the PG database is below.


## Reference code

Run `psql`, and type: `CREATE DATABASE test;`. Then run `psql -d test` and write
the following SQL:

```sql
CREATE TABLE increment (
  gapless_counter bigint NOT NULL DEFAULT 0
) WITH (
  OIDS=FALSE
);

INSERT INTO increment (gapless_counter) VALUES (1);

CREATE SEQUENCE stuff_id_seq
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 9223372036854775807
  START 1
  CACHE 1;
CREATE TABLE stuff (
  id bigint NOT NULL DEFAULT nextval('stuff_id_seq'::regclass),
  counter bigint NOT NULL,
  created_at timestamp with time zone NOT NULL DEFAULT now(),
  CONSTRAINT stuff_pkey PRIMARY KEY (id)
) WITH (
  OIDS=FALSE
);
```

Now you're ready to test the insertion examples mentioned above.


## Optional safety triggers

To prevent accidental mistakes, we added the following safety mechanisms for `increment` table:

1. Prevent deletion of any rows. This way you need to explicitly drop the table to delete data.
2. Prevent inserting more than one row, because that's the design of this table.

```sql
-- Safety mechanism to prevent accidental deletion
CREATE FUNCTION prevent_delete() RETURNS trigger AS $prevent_delete$
    BEGIN
        RAISE EXCEPTION 'row deletion has been disabled for safety, delete the whole table if you really want to delete';
    END;
$prevent_delete$ LANGUAGE plpgsql;

CREATE TRIGGER prevent_delete BEFORE DELETE ON increment FOR EACH ROW EXECUTE PROCEDURE prevent_delete();

-- Safety mechanism to prevent inserting more then 1 row
CREATE FUNCTION prevent_insert() RETURNS trigger AS $prevent_insert$
    BEGIN
        IF (SELECT COUNT(*) FROM increment) > 0 THEN
            RAISE EXCEPTION 'inserting too many rows has been disabled for safety, the table has been designed to have one row';
        END IF;

        RETURN NEW;
    END;
$prevent_insert$ LANGUAGE plpgsql;

CREATE TRIGGER prevent_insert BEFORE INSERT ON increment FOR EACH ROW EXECUTE PROCEDURE prevent_insert();
```

## Links

* https://stackoverflow.com/questions/19004453/postgresql-generate-sequence-with-no-gap


[1]: https://www.postgresql.org/docs/9.5/static/sql-createsequence.html
[2]: https://www.postgresql.org/docs/9.4/static/explicit-locking.html
[3]: https://www.postgresql.org/docs/8.3/static/sql-set.html
