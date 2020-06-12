
My history:

* [-2012 Ubuntu](https://apple.stackexchange.com/questions/68848/switching-from-ubuntu-to-os-x-and-im-lost)
* [2012-2020 OS X / Macos](https://gist.github.com/kimmobrunfeldt/350f4898d1b82cf10bce)
* Today Fedora for work, Macbook for personal coding, Windows for gaming, 3D, and Adobe tools



# Configuring Fedora 32 for dev work

* Increase max_user_watches https://unix.stackexchange.com/questions/13751/kernel-inotify-watch-limit-reached
* Use dead keys variant of keyboard for easier markdown backtick writing
* Open Gnome keyboard shortcuts
    * Set alt + w for closing window
    * Set alt + q as a new command for `xkill`. See https://blog.christophersmart.com/2009/07/29/simple-graphical-kill-in-gnome/#:~:text=Click%20on%20the%20right%20hand,simply%20by%20clicking%20on%20it.
    
* Install Gnome Tweaks to control fine grained Gnome options
    * Extensions
        * Set "overlay a tasteful logo" to disabled
    * Top bar
        * Disable Activities Overview Hot Corner
        * Tune Top bar clock as you wish
    * Window Titlebars
        * Titlebar Buttons, enable minimize and maximize
    * Workspaces
        * Set to Static Workspaces -> 2
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



