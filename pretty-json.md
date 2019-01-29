# Prettify / beautify / format / stringify JSON

Stringifying JSON in JS is easy with built-in function: `JSON.stringify(myObject, null, 2)`. 
This works great for most cases, but sometimes you might want a bit more control over the output format.


## Option 1: json-stringify-pretty-compact 

**NOTE:** This PR needs to be merged before the explanation below is true: https://github.com/lydell/json-stringify-pretty-compact/pull/19

https://github.com/lydell/json-stringify-pretty-compact

Turns this

```json
{
  "from": "decisions",
  "where": {"product.name": {"$match": "Banana"}, "behavior.user": "veronica"},
  "recommend": "product",
  "goal": {"purchase": true},
  "limit": 5
}
```

into this:

```json
{
  "from": "decisions",
  "where": {
    "product.name": { "$match": "Banana" },
    "behavior.user": "veronica"
  },
  "recommend": "product",
  "goal": { "purchase": true },
  "limit": 5
}
```

It looks clean, and also inlines objects and arrays which don't have new arrays or objects inside them, and fit the given maxLength width.


### Example code

```js
const stringify = require('json-stringify-pretty-compact')

const obj = {
  "from": "decisions",
  "where": {"product.name": {"$match": "Banana"}, "behavior.user": "veronica"},
  "recommend": "product",
  "goal": {"purchase": true},
  "limit": 5
}

const result = stringify(obj, { maxNesting: 1, margins: true })
console.log(result)
```



## Option 2: prettier

https://github.com/prettier/prettier


Does the same thing as the option 1 but doesn't have a similar option to maxNesting.


## Other resources

* https://www.npmjs.com/package/pretty-format for `util.inspect` replacement
