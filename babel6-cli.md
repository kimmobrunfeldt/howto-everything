# Babel 6 CLI quick start

Suppose you have index.js with ES2015 code:
```js
import _ from 'lodash';
const {map} = _;

console.log(map([1, 2, 3], i => i * i));
```

You can run the code from command line:

```bash
$ npm install babel-cli babel-preset-es2015 lodash
$ ./node_modules/.bin/babel-node --presets es2015 index.js
[ 1, 4, 9 ]
```
