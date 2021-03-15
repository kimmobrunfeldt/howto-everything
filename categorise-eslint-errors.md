
# Categorising eslint errors 

```
// First run eslint with --format json flag and output it to eslint.json
// Then run node this_script.js eslint.json

const fs = require('fs');
const axios = require('axios');
const _ = require('lodash');

const REPO_URL = 'https://github.com/org/repo'

function fileToIndividualMessages(fileObj) {
  return fileObj.messages.map((message) => {
    return {
      ...message,
      filePath: fileObj.filePath,
    };
  });
}

// https://github.com/sindresorhus/p-map-series
// MIT Copyright (c) Sindre Sorhus <sindresorhus@gmail.com> (sindresorhus.com)
async function mapSeriesAsync(iterable, cb) {
  const results = [];
  let index = 0;
  for (const item of iterable) {
    const result = await cb(await item, index++);
    results.push(result);
  }
  return results;
}

function ruleIdToUrl(ruleId) {
  if (_.startsWith(ruleId, 'sonarjs/')) {
    const [__, sonarRule] = ruleId.split('/');
    return `https://github.com/SonarSource/eslint-plugin-sonarjs/blob/master/docs/rules/${sonarRule}.md`;
  } else if (_.startsWith(ruleId, '@typescript-eslint/')) {
    const [__, tsRule] = ruleId.split('/');
    return `https://github.com/typescript-eslint/typescript-eslint/blob/master/packages/eslint-plugin/docs/rules/${tsRule}.md`;
  }

  return `https://eslint.org/docs/rules/${ruleId}`;
}

function getGhLink(message) {
  const file = getRelativeFilePath(message.filePath);
  return `${REPO_URL}/blob/master/${file}#L${message.line}-L${message.endLine}`;
}

function getRelativeFilePath(fullPath) {
  return fullPath.split('ui/console/')[1];
}

async function main() {
  const lints = JSON.parse(fs.readFileSync(process.argv[2], { encoding: 'utf8' }));
  const allMessages = _.flatten(lints.map(fileToIndividualMessages));
  const groupedById = _.groupBy(allMessages, (m) => m.ruleId);

  console.log('# ESLint rules');
  const arr = _.map(groupedById, (val, key) => ({ messages: val, ruleId: key }));
  const sortedArr = _.sortBy(arr, i => i.messages.length);
  await mapSeriesAsync(sortedArr, ({ messages, ruleId }) => {
    const link = ruleIdToUrl(ruleId);
    console.log(`## [${ruleId}](${link}) (${messages.length})\n`);
    _.forEach(messages, message => {
      const file = getRelativeFilePath(message.filePath);
      let lineInfo = `L${message.line}`;
      if (message.line !== message.endLine) {
        lineInfo += `-L${message.endLine}`;
      }
      console.log(`* [${file}:${lineInfo}](${getGhLink(message)})`);
      const quote = message.message.split('\n').map(line => `    > ${line}`).join('/n');
      console.log(`${quote}\n`);
    });
  });
}

main().catch((err) => {
  throw err;
});
```
