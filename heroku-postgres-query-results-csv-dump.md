Get query results as a CSV from Heroku postgres:

Bash: `heroku pg:psql --app <your-app> DATABASE_URL`
SQL: `\copy (SELECT * from users WHERE role = 'admin') TO dump.csv CSV HEADER;`
