# Using knex stream API nicely with async iterator

Here's a short intro to the new Node 10 feature: https://zaiste.net/nodejs_10_asynchronous_iteration_async_generators/

```js
// Requires node 10+
const Knex = require('knex');
const _ = require('lodash');
const BPromise = require('bluebird');
const asyncIterStream = require('async-iter-stream');

const knex = Knex({
  client: 'pg',
  connection: 'postgres://rank:rank@localhost:5432/rank',
  debug: false,
});

function withSafeStream(stream, func) {
  return new BPromise((resolve, reject) => {
    const rejectOnce = _.once(reject);
    stream.on('error', (err) => {
      rejectOnce(err);
    });

    const safeFunc = BPromise.method(func);

    safeFunc(stream)
      .then(val => resolve(val))
      .catch(err => rejectOnce(err));
  });
}

async function getValues() {
  const s = knex.raw('SELECT generate_series(1, 10)').stream();

  // Uncomment to test errors thrown from SQL
  // const s = knex.raw('SELECT ,,, generate_series(1, 10)').stream();

  return withSafeStream(s, async (stream) => {
    // Uncomment to test
    // throw new Error('test');

    for await (const row of asyncIterStream.wrap(stream)) {
      console.log(JSON.stringify(row));

      // Uncomment to test
      // throw new Error('test');
    }
  });
}

function main() {
  return getValues();
}

main()
  .then(() => console.log('Done.'))
  .catch(err => console.error('Main error caught', err));
```
