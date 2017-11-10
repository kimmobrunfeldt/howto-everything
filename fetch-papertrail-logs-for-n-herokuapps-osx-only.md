# Fetch Papertrail archives

Fetches Papertrail archives N days to the past for multiple Heroku apps. Requires access to the Heroku apps and working heroku CLI.

```bash
#!/bin/bash
# http://help.papertrailapp.com/kb/how-it-works/permanent-log-archives/#downloading-multiple-archives
# THIS WORKS ONLY IN OS X, SEE THE ABOVE HELP PAGE FOR LINUX COMMMAND
# `date` COMMAND IS DIFFERENT

# How many days back we should fetch the archives
DAYS_BACK=7

# Declare all apps here
declare -a arr=(
  "my-herokuapp1"
  "my-herokuapp2"
  "my-herokuapp3"
)

for APP in "${arr[@]}"
do
  echo "heroku config:get PAPERTRAIL_API_TOKEN -a $APP .."
  TOKEN=$(heroku config:get PAPERTRAIL_API_TOKEN -a $APP)

  echo "Downloading papertrail archives $DAYS_BACK days back .."
  seq 0 "$DAYS_BACK" | xargs -I {} date -u -v-{}d +%Y-%m-%d | \
      xargs -I {} curl --progress-bar -f --no-include -o "$APP-{}.tsv.gz" \
      -L -H "X-Papertrail-Token: $TOKEN" https://papertrailapp.com/api/v1/archives/{}/download
done
```
