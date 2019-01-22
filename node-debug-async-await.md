# Debugging async/await when default stack traces are not helpful

The issue with stack traces is described perfectly here https://github.com/nodejs/node/issues/11865#issuecomment-324983591.

One way to solve this is to use [ndb](https://github.com/GoogleChromeLabs/ndb). Steps to solve:

1. Move the script you're testing to an npm script (so it can be run `npm run my-script`). This helps ndb to discover the script.
1. Go to your project directory root and run `npm i -g ndb && ndb`. Debugger window should open up
1. From top-right icons, click the one which says "Pause on exceptions"
1. Then run your `my-script` from bottom-left in the window
1. When the exception is thrown, you'll be able to see the whole stack trace under "Call stack" in the right column.
