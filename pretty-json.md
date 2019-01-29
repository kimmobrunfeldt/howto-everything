# Prettify / beautify / format / stringify JSON

Stringifying JSON in JS is easy with built-in function: `JSON.stringify(myObject, null, 2)`. 
This works great for most cases, but sometimes you might want a bit more control over the output format.

## Option 1: prettier (my recommendation)

https://github.com/prettier/prettier

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

The good feature is that it nicely inlines objects which can nicely fit in one line. This makes the JSON 
in my opionion more readable. It also nicely inlines only the leaf objects in the JSON tree.


### Example code

```js
const prettier = require('prettier')

const str = `
{
  "from": "decisions",
  "where": {"product.name": {"$match": "Banana"}, "behavior.user": "veronica"},
  "recommend": "product",
  "goal": {"purchase": true},
  "limit": 5
}
`

const result = prettier.format(str, { parser: 'json' })
console.log(result)
```


## Option 2: json-stringify-pretty-compact

https://github.com/lydell/json-stringify-pretty-compact

This tool is also great, but with the caveat that the formatting rules are a bit more simpler. 
It might inline objects which are not the leaf objects in the JSON tree, if they fit the given character width limit. This output was produced by the tool:

```json
{
  "from": "decisions",
  "where": {"product.name": {"$match": "Banana"}, "behavior.user": "veronica"},
  "recommend": "product",
  "goal": {"purchase": true},
  "limit": 5
}
```

It looks clean (you could also add spacing for brackets with margin), but it inlines the where object completely, which I don't prefer.



## Other resources

* https://www.npmjs.com/package/pretty-format for `util.inspect` replacement
