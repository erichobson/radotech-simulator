CREATE TABLE IF NOT EXISTS users (
    user_id	INTEGER NOT NULL UNIQUE,
    first_name	TEXT NOT NULL,
    last_name	TEXT NOT NULL,
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
    sex TEXT,
    weight INTEGER,
    height INTEGER ,
    date_of_birth DATE,
    created_on	DATE DEFAULT current_date,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    PRIMARY KEY(profile_id AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS scan (
    scan_id	INTEGER NOT NULL,
    profile_id	INTEGER NOT NULL,
    name	TEXT,
    h1_lung	REAL NOT NULL,
    h1_lung_r	REAL NOT NULL,
    h2_heart_constrictor	REAL NOT NULL,
    h2_heart_constrictor_r	REAL NOT NULL,
    h3_heart	REAL NOT NULL,
    h3_heart_r	REAL NOT NULL,
    h4_small_intestine	REAL NOT NULL,
    h4_small_intestine_r	REAL NOT NULL,
    h5_triple_heater	REAL NOT NULL,
    h5_triple_heater_r	REAL NOT NULL,
    h6_large_intestine	REAL NOT NULL,
    h6_large_intestine_r	REAL NOT NULL,
    f1_spleen	REAL NOT NULL,
    f1_spleen_r	REAL NOT NULL,
    f2_liver	REAL NOT NULL,
    f2_liver_r	REAL NOT NULL,
    f3_kidney	REAL NOT NULL,
    f3_kidney_r	REAL NOT NULL,
    f4_urinary_bladder	REAL NOT NULL,
    f4_urinary_bladder_r	REAL NOT NULL,
    f5_gall_bladder	REAL NOT NULL,
    f5_gall_bladder_r	REAL NOT NULL,
    f6_stomach	REAL NOT NULL,
    f6_stomach_r	REAL NOT NULL,
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



