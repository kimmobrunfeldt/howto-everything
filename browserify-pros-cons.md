## Pros

* More readable syntax. Doesn't seem that "hack". Node-style module exporting (Opinionated, I like it)

* JS modules have to be re-built after each change in local development

    The build process is the same as when building for production. If things work in local development, they
    work in production. With RequireJS, I sometimes had problems in the build process.

* Delegate UMD pain to *browserify* with `--standalone` parameter. Browserify will build a bundle of your module which can be loaded with AMD and CommonJS


## Cons

* No dynamic **require()** calls. You can't `require('./config/' + config.environment)`. Original issue [here](https://github.com/substack/node-browserify/issues/377)

* No asynchronous loading for modules

    In reality, I have never used asynchronous module loading with RequireJS. The whole app is usually just built and bundled
    to a single package which contains JS in single bundle.

* You **will** install 3rd party libraries which are [not released in NPM](https://github.com/highslide-software/highcharts.com/issues/3616) so you have to shim them with e.g. [browserify-shim](https://github.com/thlorenz/browserify-shim)

* JS modules have to be re-built after each change in local development.

    This means you need to watch changes in JS files and wait them to rebuild after each change. In big projects, this may be  time consuming. Fortunately, there is [watchify](https://github.com/substack/watchify) to make [faster builds](http://blog.avisi.nl/2014/04/25/how-to-keep-a-fast-build-with-browserify-and-reactjs/)
