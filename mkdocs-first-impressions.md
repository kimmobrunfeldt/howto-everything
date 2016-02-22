# [MkDocs](https://github.com/mkdocs/mkdocs) first impressions


I converted progressbar.js docs to use MkDocs(+ReadTheDocs): http://progressbarjs.readthedocs.org/en/latest/

**Good:**

* Simple and clean look, but also other [themes](https://github.com/mkdocs/mkdocs/wiki/MkDocs-Themes) are available
* Markdown (love it)
* Custom markdown [extensions](http://www.mkdocs.org/user-guide/configuration/#markdown_extensions), e.g. those warning boxes. Gives convenient features with cost of not being supported in other rendering engines
* Result is static site, can be hosted anywhere: GitHub Pages, S3 ...
* Supports GH-pages deployment from the cli
* ReadTheDocs is a "CI" to build your docs and to provide other convenient features

**But:**

* Python(.. but so what)
* No good solution like ReadTheDocs for private repos
* Github pages deployment is not possible from CI, you have to roll your own
* Linking to files in repo has to be done with absolute github blog urls again..
