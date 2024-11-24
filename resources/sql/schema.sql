CREATE TABLE IF NOT EXISTS users (
    user_id	INTEGER NOT NULL UNIQUE,
    first_name	TEXT NOT NULL,
    last_name	TEXT NOT NULL,
    sex TEXT NOT NULL,
    weight INTEGER NOT NULL,
    height INTEGER NOT NULL,
    date_of_birth DATE NOT NULL,
    email	TEXT NOT NULL UNIQUE,
    password_hash	TEXT NOT NULL,
    created_on	DATE DEFAULT current_date,
    PRIMARY KEY(user_id AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS profile (
    profile_id	INTEGER NOT NULL,
    user_id	INTEGER NOT NULL,
    name	TEXT NOT NULL,
    description	TEXT,
    created_on	DATE DEFAULT current_date,
    FOREIGN KEY(user_id) REFERENCES user(user_id),
    PRIMARY KEY(profile_id AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS scan (
    scan_id	INTEGER NOT NULL,
    profile_id	INTEGER NOT NULL,
    name	TEXT,
    h1_lung	INTEGER NOT NULL,
    h2_heart_constrictor	INTEGER NOT NULL,
    h3_heart	INTEGER NOT NULL,
    h4_small_intestine	INTEGER NOT NULL,
    h5_triple_heater	INTEGER NOT NULL,
    h6_large_intestine	INTEGER NOT NULL,
    f1_spleen	INTEGER NOT NULL,
    f2_liver	INTEGER NOT NULL,
    f3_kidney	INTEGER NOT NULL,
    f4_urinary_bladder	INTEGER NOT NULL,
    f5_gull_bladder	INTEGER NOT NULL,
    f6_stomach	INTEGER NOT NULL,
    body_temp	INTEGER,
    blood_pressure	INTEGER,
    heart_rate	INTEGER,
    sleeping_time	INTEGER,
    current_weight	INTEGER,
    emotional_state	INTEGER,
    overall_feeling	INTEGER,
    notes	TEXT,
    created_on	DATE DEFAULT current_date,
    FOREIGN KEY(profile_id) REFERENCES profile(profile_id),
    PRIMARY KEY(scan_id AUTOINCREMENT)
);



