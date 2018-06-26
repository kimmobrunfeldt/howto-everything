# Using knex stream API nicely with async iterator

Here's a short intro to the new Node 10 feature: https://zaiste.net/nodejs_10_asynchronous_iteration_async_generators/

```js
// Requires node 10+
const Knex = require('knex');
const streamToIterator = require('stream-to-iterator');

const knex = Knex({
  client: 'pg',
  connection: 'postgres://user:pass@localhost:5432/db',
  debug: false,
});

async function getValues() {
  const stream = knex.raw('SELECT generate_series(1, 10)').stream();

  // Uncomment to test
  // throw new Error('test');

  for await (const row of streamToIterator(stream)) {
    console.log(JSON.stringify(row));

    // Uncomment to test
    // throw new Error('test');
  }
}

function main() {
  return getValues();
}

main()
  .then(() => console.log('Done.'))
  .catch(err => console.error('Main error caught', err));
```
