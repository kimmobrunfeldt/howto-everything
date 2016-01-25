*.env infomercial – Why have env vars in the first place? Read: http://12factor.net/config.*

# Handling environment variables

![fail gif](img/env-vars-fail.gif)

**Do you have this many environment variables?**


## Solution

* `.env` local configuration with secrets, and it is ignored in git
* `.env-sample` like `.env` but all secrets are left blank, it should be in git
* `.env-test` configuration for testing without secrets, it should be in git
* Install [autoenv](https://github.com/kennethreitz/autoenv) to make your life much easier

Step by step guide below. Assuming your project is a NodeJS project and called "booyah".


### 1. Create `.env-sample`

Create the file in the root of your project with +x permissions

```bash
#!/bin/bash
# Node app as an example, these are your develop environment settings

# Secrets, fill these
export AWS_ACCESS_KEY_ID=
export AWS_SECRET_ACCESS_KEY=

# Predefined configuration
export NODE_ENV=development
export LOG_LEVEL=debug

# You can locally use dummy credentials
# It's convenient to have a real name for your local database if you have
# multiple projects with databases in your local computer
export DATABASE_URL=postgres://booyah:booyah@localhost:5432/booyah

# Set prompt prefix, convenience feature
PREFIX="[booyah] "
if [[ $PS1 != *"$PREFIX"* ]]
then
    export PS1="$PREFIX $PS1"
    echo "Environment variables set!"
else
    echo "Environment variables updated!"
fi
```


### 2. Create `.env-test`

Create the file in the root of your project with +x permissions.
Copy non-secret config from `.env-sample` to new file `.env-test`.
This configuration is used when testing.

The file would have these variables:

```bash
export NODE_ENV=test
export LOG_LEVEL=silly
# Have a separate database for tests, it makes your life easier.
# You can then run tests aside while running local development server
export DATABASE_URL=postgres://booyah:booyah@localhost:5432/booyah_test
```

but **not** the secret ones.


### 3. Create `.env` and add it to `.gitignore`

**⚠️ This file should NOT be in version control!**

Copy `cp .env-sample .env` and fill in the secrets.

### 4. Install autoenv

[Autoenv](https://github.com/kennethreitz/autoenv) is a tool which automatically
reads and setups configuration from a `.env` file when you `cd` to a directory.


### 5. Start using the environment variables

Autoenv makes sure that secrets are read from `.env`. When running tests, you
can just e.g. `source .env-test && npm test`.

New members in project can just `cp .env-sample .env`, fill in the blanks, and
start coding.
