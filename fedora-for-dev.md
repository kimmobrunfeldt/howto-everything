
My history:

* [-2012 Ubuntu](https://apple.stackexchange.com/questions/68848/switching-from-ubuntu-to-os-x-and-im-lost)
* [2012-2020 OS X / Macos](https://gist.github.com/kimmobrunfeldt/350f4898d1b82cf10bce)
* Today Fedora for work, Macbook for personal coding, Windows for gaming, 3D, and Adobe tools



# Configuring Fedora 32 for dev work

*This a personal setup, not all settings might work for you. Read the document through and pick up the good parts to your preferences.*


## Tips

   
* Use Ctrl + PgDown and Ctrl + PgUp for changing tabs in apps with keyboard.
* Lenovo Thinkpad supports Fn + Right Arrow and Fn + Left Arrow for End and Home. Useful to go to the start or end of line.
* Some times copy - pasting stuff to terminal with middle click brings up weidr [200~ chars, see: https://cirw.in/blog/bracketed-paste
* Resize windows with Super + Mouse middle click and drag (Gnome)
* Move windows with Super + Mouse left click and drag (Gnome)
   

## Configuration

* Configure windowing to use X11 instead of new default Wayland: https://docs.fedoraproject.org/en-US/quick-docs/configuring-xorg-as-default-gnome-session/

   With Wayland, bluetooth mouse refresh rate was very slow and sluggish.
   
* `alias open='xdg-open'` Allows you to run `open .` in terminal to open file browser, and other files with defined protocol handlers.

   For example `open $DATABASE_URL`
   
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

* Setup Xmodmap for custom keyboard shortcuts. Use `xev` command to figure out keycodes. ([more](https://unix.stackexchange.com/questions/49650/how-to-get-keycodes-for-xmodmap))

   Instructions for setup: https://wiki.archlinux.org/index.php/Xmodmap. Setting keyboard shortcuts is certainly possible in linux but 
   the consensus how to do this varies A LOT. This seems to be the simplest way to setup keyboard mappings in Fedora 32 + X + Gnome.
   
   After setup, change keyboard mapping to have similar brace behavior as MacOS has. It is very logical, all needed
   characters are under keys 8 and 9.
   
   * Shift + 8 = `(`
   * AltGr + 8 = `[`
   * Shift + AltGr + 8 = `{`
   
   To list current mappings, run `xmodmap -pke`. We'll change a few items there. The fifth item is the one we want to change:
   ```patch
   -keycode  17 = 8 parenleft 8 asterisk bracketleft less
   +keycode  17 = 8 parenleft 8 asterisk bracketleft braceleft
   -keycode  18 = 9 parenright 9 parenleft bracketright greater
   +keycode  18 = 9 parenright 9 parenleft bracketright braceright
   ```
   
   To do this on each startup, add the following lines to `~/.Xmodmap` file:
   
   ```
   keycode  17 = 8 parenleft 8 asterisk bracketleft braceleft
   keycode  18 = 9 parenright 9 parenleft bracketright braceright
   ```
   
   then run `xmodmap ~/.Xmodmap` to reload changes for current X session. To apply changes at startup, follow [these instructions](https://askubuntu.com/a/958510). In case the link dies, here's recap:
   
   Add following lines to a xdg startup items: `nano ~/.config/autostart/xmodmap.desktop`:
   
   ```
   [Desktop Entry]
   Name[en_US]=Xmodmap
   Comment[en_US]=xmodmap ~/.Xmodmap
   Exec=/usr/bin/xmodmap .Xmodmap
   Icon=application-default-icon
   X-GNOME-Autostart-enabled=true
   Type=Application
   ```
   
   More references:
   
   * https://medium.com/@damko/a-simple-humble-but-comprehensive-guide-to-xkb-for-linux-6f1ad5e13450
   * https://yulistic.gitlab.io/2017/12/linux-keymapping-with-udev-hwdb/

 
## Install tools

* Install [Peek](https://github.com/phw/peek) for sharing quick interactions to Slack or PRs

    ```
    sudo dnf install ffmpeg
    sudo dnf install peek
    ```

* Install Postgres client 

   `sudo dnf install postgresql`
   
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

* Install [Git Cola](https://git-cola.github.io/) for git UI.
   
      ```
      sudo dnf install git-cola
      ```
      
      It provides a nice way to stage changes by lines. For all other operations I use CLI.
      Git Cola was the best alternative for Sourcetree in my simple use case.
      Also add aliases
      
      ```bash
      alias cola='git-cola'
      alias stree='git-cola'  # old habits.. don't do this
      ```
  
* Install [z for jumping](https://github.com/rupa/z). I have it aliased as `j` for jumping to directories
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
   
   * Ctrl + Shift + L = add cursors to end of each selected line
   * Ctrl + X bindings might be useless, as I tried to get rid of Ctrl + X removing line without selection. I'm used to it only working as a clipboard shortcut.
   
   
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

### Python

* `sudo dnf install python3-devel.x86_64`
* Install pipenv for per-project installs instead of global https://docs.python-guide.org/dev/virtualenvs/#installing-pipenv


