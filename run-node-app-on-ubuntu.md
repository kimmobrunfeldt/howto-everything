## Run node web app on Ubuntu server

Short version of: http://stackoverflow.com/questions/18687850/setting-up-node-js-on-webfaction

### Install steps

**Remember: You should run node process as an unprivileged user!**

* Install node and npm

        sudo apt-get install node npm

* Create `web` user

* Link node to nodejs to make `node` and `nodejs` available in bash

        sudo ln -s /usr/bin/nodejs /usr/bin/node

* Install [forever](https://github.com/nodejitsu/forever). Which will keep the node app script running.

* Place `start-nodeapp.sh`, `stop-nodeapp.sh` and `restart-nodeapp.sh` to `/home/web`. Scripts are below. **WARNING:** Running start script twice will spawn two processes! You should always run stop before start.

* Setup starting of the node process when server starts

    Setup crontab as `web` user to make sure the nodejs does not have too much rights

    ```bash
    export EDITOR=nano
    crontab -e
    ```

    And add line: `@reboot bash /home/web/start-nodeapp.sh`

**start-nodeapp.sh**

```bash
#!/bin/bash
HOME="/home/web"
DIRECTORY="/home/web/nodeapp"
export NODE_ENV="production"

forever start --uid "nodeapp" --minUptime 5000 --spinSleepTime 2000 --fifo --sourceDir $DIRECTORY/dist --workingDir $DIRECTORY/dist --append -l $HOME/nodeapp.forever.log -o $HOME/nodeapp.stdout.log -e $HOME/nodeapp.stderr.log server/app.js
```

**stop-nodeapp.sh**

```bash
#!/bin/bash

forever stop nodeapp
```

**restart-nodeapp.sh**

```bash
#!/bin/bash

forever restartall
```
