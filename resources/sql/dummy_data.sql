-- Insert test user if it doesn't exist
INSERT INTO users (first_name, last_name, email, password_hash)
SELECT 'Test', 'User', 'test@mail.com', '5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8'
WHERE NOT EXISTS (SELECT 1 FROM users);

-- Insert profiles for user if the user does not already have profiles
INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth)
SELECT 1, 'Test Profile 1', 'Test profile 1 desc', 'female', 77, 125, '1999-09-09'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1);

INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth)
SELECT 1, 'Test Profile 2', 'Test profile 2 desc', 'male', 55, 186, '2000-01-01'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 2');

INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth)
SELECT 1, 'Test Profile 3', 'Test profile 3 desc', 'male', 88, 144, '2002-02-02'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 3');

INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth)
SELECT 1, 'Test Profile 4', 'Test profile 4 desc', 'female', 65, 200, '1967-07-07'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 4');

INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth)
SELECT 1, 'Test Profile 5', 'Test profile 5 desc', 'male', 45, 100, '1901-03-03'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 5');

-- Insert scan records only if they don't already exist for the given profile
INSERT INTO scan (profile_id, name,
    h1_lung, h2_heart_constrictor, h3_heart, h4_small_intestine, h5_triple_heater, h6_large_intestine,
    f1_spleen, f2_liver, f3_kidney, f4_urinary_bladder, f5_gall_bladder, f6_stomach,

    h1_lung_r, h2_heart_constrictor_r, h3_heart_r, h4_small_intestine_r, h5_triple_heater_r, h6_large_intestine_r,
    f1_spleen_r, f2_liver_r, f3_kidney_r, f4_urinary_bladder_r, f5_gall_bladder_r, f6_stomach_r,

    body_temp, blood_pressure, heart_rate, sleeping_time, current_weight, emotional_state, overall_feeling, notes
)
SELECT 1, 'Afternoon Scan',
    125.0, 135.3, 112.25, 102.22, 118.33, 108.1, -- h1-h6
    128.2, 138.4, 133.12, 123.3, 118.3, 113.1, --f1-f6

    115.3, 125.2, 113.6, 112.3, 188.25, 128.33, -- h1-h6 (right)
    118.2, 134.12, 123.5, 173.8, 115.9, 123.78, --f1-f6 (right)

    365, 12585, 74, 7, 72, 3, 4, 'Slightly tired'
WHERE NOT EXISTS (SELECT 1 FROM scan WHERE profile_id = 1 AND name = 'Afternoon Scan');
