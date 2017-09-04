# Save Postgres backup from Heroku and encrypt it

Once in a while, I want to save Postgres backups to e.g. Google Drive for
extra measures. I'm using Heroku and GPG CLI tools to get the job done.

Installing GPG also requires a bit work, but it pays off as it's very convenient
way to encrypt random individual files.
To setup GPG, follow e.g. [DigitalOcean's guide](https://www.digitalocean.com/community/tutorials/how-to-use-gpg-to-encrypt-and-sign-messages-on-an-ubuntu-12-04-vps).
**Note: It says deprecated but the GPG setup should be still the same.**

In my Heroku hosted projects, I save the following scripts to the repository to save the scripts close to the project.

**./tools/take-backup.sh**

```bash
#!/bin/bash

set -x
set -e

APP_NAME=my-app
DESTINATION_DIR='/Users/username/Google Drive/backups'
DATE=$(date "+%Y-%m-%dT%H%M")

DESTINATION="$DESTINATION_DIR/$APP_NAME-pg-$DATE.dump"

echo "Taking postgres dump from prod .. "
pg_dump --no-owner --no-acl -Fc $(heroku config:get DATABASE_URL -a $APP_NAME) > "$DESTINATION"

echo "Encrypting file with GPG .. "
gpg -c "$DESTINATION"

echo "Removing unencrypted file .. "
rm "$DESTINATION"

echo "Done. Database dump saved at $DESTINATION"
```

**./tools/restore-backup.sh**

```bash
#!/bin/bash

DESTINATION_DB='postgresql://user:pass@localhost:5432/your_dbname'

if [ -z "$1" ]
then
  echo "Usage: ./restore-backup.sh <encrypted-dump-file>"
  exit 1
fi

if [ -n "$2" ]
then
  echo "Usage: ./restore-backup.sh <encrypted-dump-file>"
  exit 1
fi

echo "Decrypting file $1 .."
gpg --output decrypted.dump --decrypt "$1"

echo "Restoring PG dump .."
pg_restore --no-owner --verbose --clean --no-acl -Fc -d "$DESTINATION_DB" decrypted.dump

echo "Removing temporary decrypted file .."
rm decrypted.dump

echo "Done. Backup restored to $DESTINATION_DB"
```
