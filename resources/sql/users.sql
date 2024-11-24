CREATE TABLE "users" (
    "user_id"	INTEGER NOT NULL UNIQUE,
    "name"	TEXT NOT NULL,
    "username"	TEXT NOT NULL,
    "email"	TEXT NOT NULL UNIQUE,
    "password_hash"	TEXT NOT NULL,
    "created_on"	DATE DEFAULT current_date,
    PRIMARY KEY("user_id" AUTOINCREMENT)
);
