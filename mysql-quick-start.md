Create simple MySQL database
----------------------------

Run

    mysql -u root --password

And run the following SQL commands

    CREATE DATABASE {{project}};

    CREATE USER '{{project}}'@'localhost' IDENTIFIED BY 'password';
    GRANT ALL PRIVILEGES ON {{project}}.* TO '{{project}}'@'localhost';
    FLUSH PRIVILEGES;

    USE {{project}};
    CREATE TABLE {{project}} (
        id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
        name TEXT,
        mail TEXT,
        allergies TEXT,
        answer TEXT
    );

Give rights to connect from all hosts
-------------------------------------

You must also create user for localhost and grant privileges for that user.

http://stackoverflow.com/questions/10236000/allow-all-remote-connections-mysql

    CREATE USER '{{project}}'@'localhost' IDENTIFIED BY 'password';
    GRANT ALL PRIVILEGES ON {{project}}.* TO '{{project}}'@'localhost';

    CREATE USER '{{project}}'@'%' IDENTIFIED BY 'password';
    GRANT ALL PRIVILEGES ON {{project}}.* TO '{{project}}'@'%';

    FLUSH privileges;
