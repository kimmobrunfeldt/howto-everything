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

