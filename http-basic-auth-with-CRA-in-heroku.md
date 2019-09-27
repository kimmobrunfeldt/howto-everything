Need a HTTP Basic Auth for Create-React-App that is running on Heroku? Look no further!


## Special buildpack
Use this buildpack for your Heroku app:
https://github.com/substantial/create-react-app-buildpack.git

## Configuration

- `BASIC_AUTH_USERNAME` environment variable sets the required user name. Plain text is OK
- `BASIC_AUTH_PASSWORD` environment variable holds the password. It needs to be hashed in the same format as you'd do fin [`.htpasswd`](http://www.htaccesstools.com/articles/htpasswd/)-file
