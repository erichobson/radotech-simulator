-- Insert test user if it doesn't exist 
INSERT INTO users (name, username, email, password_hash)
SELECT 'Test', 'test', 'test@mail.com', 'password'
WHERE NOT EXISTS (SELECT 1 FROM users);

-- Insert profiles for user if the user does not already have profiles
INSERT INTO profile (user_id, name, description)
SELECT 1, 'Test Profile 1', 'Test profile 1 desc'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1);

INSERT INTO profile (user_id, name, description)
SELECT 1, 'Test Profile 2', 'Test profile 2 desc'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 2');

INSERT INTO profile (user_id, name, description)
SELECT 1, 'Test Profile 3', 'Test profile 3 desc'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 3');

INSERT INTO profile (user_id, name, description)
SELECT 1, 'Test Profile 4', 'Test profile 4 desc'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 4');

INSERT INTO profile (user_id, name, description)
SELECT 1, 'Test Profile 5', 'Test profile 5 desc'
WHERE NOT EXISTS (SELECT 1 FROM profile WHERE user_id = 1 AND name = 'Test Profile 5');

-- Insert scan records only if they don't already exist for the given profile
INSERT INTO scan (profile_id, name, h1_lung, h2_heart_constrictor, h3_heart, h4_small_intestine, 
    h5_triple_heater, h6_large_intestine, f1_spleen, f2_liver, f3_kidney, f4_urinary_bladder, 
    f5_gull_bladder, f6_stomach, body_temp, blood_pressure, heart_rate, sleeping_time, current_weight, 
    emotional_state, overall_feeling, notes)
SELECT 1, 'Morning Scan', 120, 130, 110, 100, 115, 105, 130, 140, 135, 125, 120, 115, 370, 12080, 72, 8, 70, 4, 5, 'Feeling good'
WHERE NOT EXISTS (SELECT 1 FROM scan WHERE profile_id = 1 AND name = 'Morning Scan');

INSERT INTO scan (profile_id, name, h1_lung, h2_heart_constrictor, h3_heart, h4_small_intestine, 
    h5_triple_heater, h6_large_intestine, f1_spleen, f2_liver, f3_kidney, f4_urinary_bladder, 
    f5_gull_bladder, f6_stomach, body_temp, blood_pressure, heart_rate, sleeping_time, current_weight, 
    emotional_state, overall_feeling, notes)
SELECT 1, 'Afternoon Scan', 125, 135, 112, 102, 118, 108, 128, 138, 133, 123, 118, 113, 365, 12585, 74, 7, 72, 3, 4, 'Slightly tired'
WHERE NOT EXISTS (SELECT 1 FROM scan WHERE profile_id = 1 AND name = 'Afternoon Scan');

INSERT INTO scan (profile_id, name, h1_lung, h2_heart_constrictor, h3_heart, h4_small_intestine, 
    h5_triple_heater, h6_large_intestine, f1_spleen, f2_liver, f3_kidney, f4_urinary_bladder, 
    f5_gull_bladder, f6_stomach, body_temp, blood_pressure, heart_rate, sleeping_time, current_weight, 
    emotional_state, overall_feeling, notes)
SELECT 1, 'Evening Scan', 118, 132, 109, 98, 112, 102, 126, 136, 132, 120, 115, 110, 368, 11978, 70, 6, 68, 5, 5, 'Relaxed'
WHERE NOT EXISTS (SELECT 1 FROM scan WHERE profile_id = 1 AND name = 'Evening Scan');

