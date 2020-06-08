# Configuring Fedora 32 for dev work


* Increase max_user_watches https://unix.stackexchange.com/questions/13751/kernel-inotify-watch-limit-reached
* Use dead keys variant of keyboard for easier markdown backtick writing
* Install Gnome Tweaks to control fine grained Gnome options
* Install [Peek](https://github.com/phw/peek) for sharing quick interactions to Slack or PRs

    sudo dnf install ffmpeg
    sudo dnf install peek

* sudo dnf install postgresql for psql client

* Install docker https://docs.docker.com/engine/install/fedora/

    See this issue with Fedora 32: https://github.com/docker/for-linux/issues/955#issuecomment-620932240

## Python

* sudo dnf install python3-devel.x86_64
* Install pipenv for per-project installs instead of global https://docs.python-guide.org/dev/virtualenvs/#installing-pipenv

