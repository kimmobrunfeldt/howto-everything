# Minimal Circle CI config.yml for Node

This uses the version 2 config format.

```yml
version: 2
jobs:
  build:
    docker:
      - image: circleci/node:8
    steps:
      - checkout
      - run: npm install
      - run: npm run lint
      - run: npm test
```
