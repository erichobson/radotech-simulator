DELETE FROM scan;
DELETE FROM profile;
DELETE FROM user;

INSERT INTO user ('name', 'username', 'email', 'password_hash') VALUES ('Andrew', 'andrew', 'andrew@mail.com', 'password');
INSERT INTO profile ('user_id', 'name', 'description') VALUES (1, 'Andrews Profile', 'A really descriptive description');
INSERT INTO scan (
    profile_id,
    name,
    h1_lung,
    h2_heart_constrictor,
    h3_heart,
    h4_small_intestine,
    h5_triple_heater,
    h6_large_intestine,
    f1_spleen,
    f2_liver,
    f3_kidney,
    f4_urinary_bladder,
    f5_gull_bladder,
    f6_stomach,
    body_temp,
    blood_pressure,
    heart_rate,
    sleeping_time,
    current_weight,
    emotional_state,
    overall_feeling,
    notes
)
VALUES
    (1, 'Morning Scan', 120, 130, 110, 100, 115, 105, 130, 140, 135, 125, 120, 115, 370, 12080, 72, 8, 70, 4, 5, 'Feeling good'),
    (1, 'Afternoon Scan', 125, 135, 112, 102, 118, 108, 128, 138, 133, 123, 118, 113, 365, 12585, 74, 7, 72, 3, 4, 'Slightly tired'),
    (1, 'Evening Scan', 118, 132, 109, 98, 112, 102, 126, 136, 132, 120, 115, 110, 368, 11978, 70, 6, 68, 5, 5, 'Relaxed')
;

SELECT * from user;
SELECT * from profile;
SELECT * from scan;
