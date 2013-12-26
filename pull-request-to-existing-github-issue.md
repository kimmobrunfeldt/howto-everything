* Install hub with `brew install hub`
* Fork a project and clone it to your computer
* In your fork's folder, run:
    
        hub pull-request -b <original-author-github-name>:master -h <your-github-name>:master <link-to-github-existing-issue>

For example:

    hub pull-request -b laanwj:master -h kimmobrunfeldt:master "https://github.com/laanwj/bitcoin-python/issues/51"
