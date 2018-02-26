# Automate Heroku environment installation

Terraform doesn't support Heroku that well. E.g. there were some issues with Pipelines and setting the dyno type. Heroku also provides app.json to describe the environment. Sometimes it's still ok to just use a bash script to install all needed parts. Heroku CLI is quite thorough and you can install almost everything with it.

Here's an example of a bash script which installs a few add-ons to an app:


```bash
#!/bin/bash

heroku addons:create --app myapp papertrail
heroku addons:create --app myapp heroku-postgresql
heroku addons:create --app myapp newrelic
heroku addons:create --app myapp heroku-redis
heroku addons:create --app myapp scheduler:standard
heroku addons:create --app myapp deployhooks:email \
    --recipient=team-email@company.com \
    --subject="Deployed: {{app}}, release: {{release}}" \
    --body="{{user}} deployed the app from commit {{head}}.\nDashboard: https://dashboard.heroku.com/apps/{{app}}/resources\n\nChanges since last deployment:\n{{git_log}}"
```
