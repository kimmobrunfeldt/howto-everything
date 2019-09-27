# Copy one PG database to another

The purpose is to copy for example QA environment database to your local postgres database.

Usage: `bash pg-copy.sh $(heroku config:get DATABASE_URL -a MY_APP) postgres://user:pass@127.0.0.1:5432/mydb`



```
#!/bin/bash

set -e

if [ -z "$1" ]
then
  echo "Error: missing the first database url"
  echo "Usage: ./pg-copy.sh postgres://user:pass@host/db1 postgres://user:pass@host/db2"
  exit 1
fi

if [ -z "$2" ]
then
echo "Error: missing the second database url"
  echo "Usage: ./pg-copy.sh postgres://user:pass@host/db1 postgres://user:pass@host/db2"
  exit 1
fi

DB1=$1
DB2=$2

echo "Copying data:"
echo -e "$DB1 -> $DB2\n"
echo "WARNING: THIS OPERATION WILL DELETE ALL DATA IN $DB2!"
read -p "Do you want to continue? [N/y] "
echo    # (optional) move to a new line
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
  # handle exits from shell or function but don't exit interactive shell
  [[ "$0" = "$BASH_SOURCE" ]] && exit 1 || return 1
fi

echo "Copying $DB1 -> $DB2 .. "

# pg_dump options explained:
# -F custom
#    Specify format of the archive. The archive is in the custom format of pg_dump.
# --no-acl
#    Prevent dumping of access privileges (grant/revoke commands).
#    This is needed when the dbs are owned by different named users,
#    which usually is the case
# --no-owner
#    Do not output commands to set ownership of objects to match the original database.
#    This is needed when the dbs are owned by different named users,
#    which usually is the case
#
# pg_restore options:
# --clean
#    Clean (drop) database objects before recreating them.
# --create
#   Create the database before restoring into it.
pg_dump --no-owner --no-acl -F custom "$DB1" | pg_restore -F custom --clean -d "$DB2"
```
