
My history:

* [-2012 Ubuntu](https://apple.stackexchange.com/questions/68848/switching-from-ubuntu-to-os-x-and-im-lost)
* [2012-2020 OS X / Macos](https://gist.github.com/kimmobrunfeldt/350f4898d1b82cf10bce)
* Today Fedora for work, Macbook for personal coding, Windows for gaming, 3D, and Adobe tools



# Configuring Fedora 32 for dev work

* Configure windowing to use X11 instead of new default Wayland: https://docs.fedoraproject.org/en-US/quick-docs/configuring-xorg-as-default-gnome-session/

   With Wayland, bluetooth mouse refresh rate was very slow and sluggish.
   
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

* `sudo dnf install postgresql` for psql client

* Install Docker

    **Options are:**
    
    1. [podman](https://podman.io/) for running docker images 
    1. Install moby-engine, see Option 2 at https://www.linuxuprising.com/2019/11/how-to-install-and-use-docker-on-fedora.html
    1. Docker https://docs.docker.com/engine/install/fedora/

    This issue contains useful info about Docker issues with Fedora 32: https://github.com/docker/for-linux/issues/955#issuecomment-620932240
    
    Aliases:
    
    ```bash
    alias docker='podman'
    alias ubuntu='docker run -v $HOME/.ssh:/root/.ssh -v $PWD:/root --security-opt label=disable -w /root --rm -i -t kimmobrunfeldt/ubuntu /bin/bash'
    ```

* Install [scm_breeze](https://github.com/scmbreeze/scm_breeze) for very useful git shortcuts.

* `sudo dnf install git-cola` for a git UI. I also have it aliased as `cola` (and also `alias stree=git-cola`..)
   
      It provides a nice way to stage changes by lines. For all other operations I use CLI.
      
* Install [z for jumping](https://github.com/rupa/z). I have it aliased as `j` for jumping to directories
* `alias open='xdg-open'` Allows you to run `open .` to open file browser, etc.
* Install VS Code

   My settings:
   ```js
   {
       "window.zoomLevel": 3,
       "explorer.openEditors.visible": 0,
       "editor.renderWhitespace": "all",
       "files.trimTrailingWhitespace": true,
       "files.trimFinalNewlines": true,
       "git.decorations.enabled": false,
       // This option is needed to disable the behavior where Ctrl - X cuts
       // a line even when there's no selection
       "editor.emptySelectionClipboard": false,
       "explorer.confirmDragAndDrop": false,
       "workbench.editor.enablePreview": false,
       "workbench.editor.enablePreviewFromQuickOpen": false,
       "editor.tabSize": 2,
       "editor.rulers": [
           80, 120
       ],
       "typescript.updateImportsOnFileMove.enabled": "always",
       "update.showReleaseNotes": false,
       "editor.acceptSuggestionOnEnter": "off"
   }
   ```
   
   
   Keyboard shortcuts:
   ```js
   // Place your key bindings in this file to override the defaultsauto[]
   [
       {
           "key": "ctrl+x",
           "command": "-notebook.cell.cut",
           "when": "notebookEditorFocused && !inputFocus"
       },
       {
           "key": "ctrl+shift+alt+x",
           "command": "filesExplorer.cut",
           "when": "explorerViewletVisible && filesExplorerFocus && !explorerResourceIsRoot && !inputFocus"
       },
       {
           "key": "ctrl+x",
           "command": "-filesExplorer.cut",
           "when": "explorerViewletVisible && filesExplorerFocus && !explorerResourceIsRoot && !inputFocus"
       },
       {
           "key": "ctrl+x",
           "command": "-editor.action.clipboardCutAction",
           "when": "textInputFocus && !editorReadonly"
       },
       {
           "key": "ctrl+x",
           "command": "editor.action.clipboardCutAction"
       },
       {
           "key": "ctrl+shift+l",
           "command": "editor.action.insertCursorAtEndOfEachLineSelected",
           "when": "editorTextFocus"
       },
       {
           "key": "shift+alt+i",
           "command": "-editor.action.insertCursorAtEndOfEachLineSelected",
           "when": "editorTextFocus"
       }
   ]
   ```
   
   Extensions:
   * GitLens
   * JSON Tools
   * MDX
   * Prettier
   * Python
   
* Use Ctrl + PgDown and Ctrl + PgUp for changing tabs in apps with keyboard
* Some times copy - pasting stuff to terminal with middle click brings up weidr [200~ chars, see: https://cirw.in/blog/bracketed-paste
* Resize windows with Super + Mouse middle click and drag (Gnome)
* Move windows with Super + Mouse left click and drag (Gnome)
   

## Python

* `sudo dnf install python3-devel.x86_64`
* Install pipenv for per-project installs instead of global https://docs.python-guide.org/dev/virtualenvs/#installing-pipenv


