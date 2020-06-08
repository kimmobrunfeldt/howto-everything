# Configuring Fedora 32 for dev work


* Increase max_user_watches https://unix.stackexchange.com/questions/13751/kernel-inotify-watch-limit-reached
* Use dead keys variant of keyboard for easier markdown backtick writing
* Install Gnome Tweaks to control fine grained Gnome options
* Install [Peek](https://github.com/phw/peek) for sharing quick interactions to Slack or PRs

    sudo dnf install ffmpeg
    sudo dnf install peek

* sudo dnf install postgresql for psql client

* Install Docker

    **Options are:**
    
    1. [podman](https://podman.io/) for running docker images 
    1. Install moby-engine, see Option 2 at https://www.linuxuprising.com/2019/11/how-to-install-and-use-docker-on-fedora.html
    1. Docker https://docs.docker.com/engine/install/fedora/

    This issue contains useful info about Docker issues with Fedora 32: https://github.com/docker/for-linux/issues/955#issuecomment-620932240

## Python

* sudo dnf install python3-devel.x86_64
* Install pipenv for per-project installs instead of global https://docs.python-guide.org/dev/virtualenvs/#installing-pipenv

