CREATE TABLE "profile" (
    "profile_id"	INTEGER NOT NULL,
    "user_id"	INTEGER NOT NULL,
    "name"	TEXT NOT NULL,
    "description"	TEXT,
    "created_on"	DATE DEFAULT current_date,
    FOREIGN KEY("user_id") REFERENCES "user"("user_id"),
    PRIMARY KEY("profile_id" AUTOINCREMENT)
);
