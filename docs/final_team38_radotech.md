# RaDoTech
> COMP 3004 <br>
> Team 38 <br> 
> Members: Eric Hobson, Olu Ogunmeru, Abdulmalik Umar, Andrew Wallace
<hr>

## Overview
RaDoTech is a health tracker and monitoring system designed to measure, track, and analyze the health of your 12 main vital organs. The system utilizes Japanese Ryodoraku technology to measure electrical currents running through meridian points on the human body. Using these measurements, RaDoTech can calculate various health indicators to give you valuable insights on your current health. This project was designed to simulate the RaDoTech device and application.

## Ryodoraku
RaDoTech utilizes Ryodoraku technology, which is a form of electroacupuncture developed by Yoshio Nakatani, MD, PhD. He found a series of low electrical resistance points that run up and down the body, called meridian points. 
There are 12 main paired meridians on the body (24 in total). The meridians have a naming convention where H1 - H6 correspond to the meridian points on the upper limbs (wrists and hands) and F1-F6 correspond to the meridian points on the lower limbs (ankles and foot). Please see Figure 1 for the exact location of the points.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/8ed4eafd-2477-4bf3-9234-19d9ee3e7ba1" />
    <p><em>Figure 1: 12 Meridian Points</em></p>
</div>
Each of these points are measured using the RaDoTech device and stored in the app. Each of the meridian points correspond to a particular organ (as seen in Table 1), and based on the measurements, we can determine whether a particular organ is functioning below normal, above normal, or normal. We can also determine the symptoms one may be experiencing using the meridian point measurements as a guide (as seen in Table 1). We will discuss how we use these measurements in a later section. 
<br/>
Table 1: Meridian Point Correlations
<br/>

| Meridians | Common Symptoms of High Conductivity | Common Symptoms of Low Conductivity |
| --- | --- | --- |
| **H1 - Lung (LU)** | Stiff shoulder, Back disorder, Blood rushing to head, Anal disorder, Asthma | Chilly or numb sensation of extremities, Headache, Respiration disorder, Dizziness |
| **H2 - Heart constrictor (HC)** | Stiff shoulder | Palpitation, Headache |
| **H3 - Heart (HT)** | Full sensation of stomach, Constipation, Shoulder pain | Palpitation, Nausea |
| **H4 - Small intestine (SI)** | Headache, Weakness of extremities, Disorder of lower abdomen, Shoulder pain, Rheumatism | Headache, Disorder of lower abdomen |
| **H5 - Triple heater (TH)** | Disorder of urination, Tinnitus | Respiration disorder, Nausea |
| **H6 - Large intestine (LI)** | Stiff shoulder | Stiff shoulder |
| **F1 - Spleen (Pancreas) (SP)** | Weak stomach, Disorder of joint | Weak stomach, Disorder of skin, Constipation |
| **F2 - Liver (LV)** | Lumbago, Insomnia, Dizziness, Disorder of menstruation | Chilly sensation of lower extremities, Dizziness, Impotence, Mental depression |
| **F3 - Kidney (KI)** | Malaise, Nausea | General debility, Chilly sensation of lower extremities, Impotence |
| **F4 - Urinary bladder (BL)** | Stiffness of neck, Disorder of lower extremities | Stiffness of nape, Heavy and weak sensation of extremities, Disorder of back |
| **F5 - Gallbladder (GB)** | Bitter taste, Disorder of throat | Eye disorder, Dizziness |
| **F6 - Stomach (ST)** | Stiffness of nape, Disorder of joints | Stiffness of shoulder, Constipation, Full sensation of stomach, Mental depression |

## Ryodoraku Calculations - Organ Health
The measurements can be used to calculate organ health and health indicators. The process for determining a meridian point outlier is as follows [[1]](#1): 
_Please note each measurement is taken in micro amperes (µA)._
1. Take measurements for all 24 points. 
2. Take the mean of the measurements.
3. Plot the mean and each measurement (H1-H6 and F1-F6) in a bar chart.
4. Any measurement that deviates from the mean by 1.4cm from the mid-line is considered abnormal

Since we did not have the ability to plot and measure 1.4cm in the application, we used the 20% rule as referenced in the paper "Circadian variations in electric current responses at ryodoraku points"[[2]](#2). That is, any point that is +/- 20% of the mean is considered an outlier.
This means that for any meridian point measurement below 20% of the mean, the corresponding organ will be considered below normal. Similarly, for any meridian point measurement above 20% of the mean, the corresponding organ will be considered above normal.

## Ryodoraku Calculations - Indicator Health
We also use the measurements to calculate metrics for the following health indicators: 
1. Energy level - calculated by taking the average (mean) value of all the measurements
2. Immune system - calculated by dividing the absolute difference between the upper and lower values by 2
3. Metabolism - calculated by taking a weighted sum of the left values and dividing them by a weighted sum of the right values
4. Psycho-emotional state - calculated by dividing the sum of upper values by the sum of lower values
5. Musculoskeletal system - calculated by divding the sum of the left values by the sum of the right values

For each of these calculations, if they fall below the range specified in Table 2, the are considered below normal. If they are above the range, they are considered above normal. Otherwise, they are considered normal.
<br>
**Table 2: Ryodoraku Indicator Correlations**
| Indicator | Normal Range |
| --- | --- |
| Energy Level | 25-55 |
| Immune system | 47-57 |
| Metabolism | 1.1-1.2 |
| Psycho-emotional state | 0.8-1.1 (woman), 0.9-1.2 (man) |
| Musculoskeletal system | 0.9-1.2 |

## Use Cases
### Use Case 1: RaDoTech Scan

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the scan completes successfully and is tracked 

RaDoTech company → the device works as expected and no failures occur

RaDoTech device → completes the scan successfully

RaDoTech system → completes and records the scan successfully

**Precondition**: The device is turned on and connected to the application (via Bluetooth). Location services turned on if you’re on android.

**Minimal guarantee**: The user is notified about scan success or failure

**Success guarantee**: The user successfully completes the scan

**Main success scenario:**

1. The user starts the scan through the application
2. The user sprays a bit of water on their wrists, ankles, and top of feet (meridian point areas)
3. The user selects their desired profile
4. The user uses the device to scan one of the meridian points, holding contact to the point until an indicator notifies them to remove the device from contact immediately 
5. The user does this for all 24 points, completing the scan
6. The user can add notes and information about their current state before saving
7. The user saves the scan
8. The user is presented with a success message & results of the scan
9. The user is returned to the home page of the application

**Extensions**:

2a. The user doesn’t spray water on the meridian point areas

2a1. Conduct the scan as usual

3a. The user doesn’t select a profile

3a1. The user will have an option to stop the scan at all times in which they will be returned to the home page

4a. The user scans a non-meridian point

4a1. The device will do it’s best to accurately scan the point

4b. The user removes the device before the indication

4b1. The scan does not move on to the next point

4b2. The app prompts the user to scan the same point again

4b3. The app shows a text message telling the user to wait for the indicator before removing the device from the point 

4c. The device malfunctions

4c1. *Use Case X*

5a. The user does not complete all 24 points

5a1. The scan does not complete

5a2. The app notifies the user to complete the remaining scans

5a3. The user can stop the scan at any point

5b. There is an error reading one of the meridian points

5b1. The user is notified about the error and is prompted to stop the scan

5c. The device runs out of battery

5c1. The scan stops and returns the user to the start screen

5c2. The user is notified about the battery outage

6a. The user does not add any notes

6a1. The user can still save the scan

7a. The user doesn’t want to save the scan

7a1. The user can stop the scan at any point

7b. There is an error saving the scan

7b1. The user is presented with a error message in step 8

### Use Case 2a: Scan Results - Organ Health

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the results show the users health metrics

RaDoTech company → the application successfully shows the health metrics

RaDoTech system → correctly calculates and displays the health metrics in a visual format

**Precondition**: The application is functional and the user is on the home page. The user has selected their desired profile.

**Minimal guarantee**: The user is notified about scan success or failure

**Success guarantee**: The user can view their organ health from the scan

**Main success scenario:**

1. The user navigates to the “history” section of the app
2. The user selects a particular scan
3. The user is routed to the health metrics section of the application 
4. The user navigates to the “organ health” section where they are presented a visual representation of their organ health (whether an organ is above normal, below normal, or normal) from advanced calculations.

**Extensions**:

1a. There are no scans for that profile

1a1. The user is notified about the lack of scans

4a. There is an error calculating the organ health metrics for that scan

4a1. The user is notified about the error

### Use Case 2b: Scan Results - Indicators

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the results show the users health metrics

RaDoTech company → the application successfully shows the health metrics

RaDoTech system → correctly calculates and displays the health metrics in a visual format

**Precondition**: The application is functional and the user is on the home page. The user has selected their desired profile.

**Success guarantee**: The user can view their overall health indicators from the scan

**Main success scenario:**

1. The user navigates to the “history” section of the app
2. The user selects a particular scan
3. The user is routed to the health metrics section of the application 
4. The user navigates to the “indicators” section where they are presented a visual representation of their overall system health (i.e., Energy level, Immune system, Metabolism, Psycho-emotional state, and Musculoskeletal system) from a calculation.

**Extensions**:

1a. There are no scans for that profile

1a1. The user is notified about the lack of scans

4a. There is an error calculating the health indicators for that scan

4a1. The user is notified about the error

### Use Case 2c: Scan Results - Recommendations

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the results show the users health metrics

RaDoTech company → the application successfully shows the health metrics

RaDoTech system → correctly calculates and displays the health metrics in a visual format

**Precondition**: The application is functional and the user is on the home page

**Success guarantee**: The user can view their recommendations from the scan

**Main success scenario:**

1. The user navigates to the “history” section of the app
2. The user selects a particular scan
3. The user is routed to the health metrics section of the application 
4. The user navigates to the “recommendations” section where they are presented with recommendations for their health

**Extensions**:

4a. There is an error retrieving/formulating the recommendations for that scan

4a1. The user is notified about the error

4b. There are no recommendations for that scan

4b1. The user is notified that there are no recommendations

### Use Case 3a: Profile Creation

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the user can create multiple profiles (up to 5)

RaDoTech company → the application successfully stores the user profiles

RaDoTech system → correctly stores the user profiles, creating records in the necessary tables

**Precondition**: The application is functional and the user is on the home page

**Success guarantee**: The user can create a profile

**Main success scenario:**

1. The user navigates to the “profiles” section of the app
2. The user selects the “new profile” option
3. The user is presented with a form to add their name, sex, weight, height, and date of birth for the profile
4. The user adds the name, sex, weight, height, and date of birth of the profile
5. The user clicks the “save” button
6. The user is navigated to their list of profiles, displaying the newly created one

**Extensions**:

4a. The user doesn’t add a name

4a1. This is a required field, so they cannot create a profile without it

4a2. The user is presented with an error message indicating that the name is a required field when trying to save

4b. The user doesn’t add their sex, weight, height, and date of birth

4b1. These are required, so they cannot create a profile without it

4b2. The user is presented with an error message indicating that the name is a required field when trying to save

5a. The user doesn’t want to save the profile at this time

5a1. The user has an option to cancel the profile creation at any point via the “cancel” button

5b. There is an error saving the profile

5b1. The user is presented with an error message, notifying them of the failed save

### Use Case 3b: Profile Update

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the user can update a profile

RaDoTech company → the application successfully updates the profile

RaDoTech system → correctly stores the updated user profile, updating records in the necessary tables

**Precondition**: The application is functional and the user is on the home page

**Success guarantee**: The user can update a profile

**Main success scenario:**

1. The user navigates to the “profiles” section of the app
2. The user selects a profile
3. The user is presented with a form of the profile’s name, sex, weight, height, and date of birth
4. The user updates the information
5. The user clicks the “save” button
6. The user is returned to the list of their profiles

**Extensions**:

4a. The user removes their name

4a1. This is a required field, so they cannot update a profile without it

4a2. The user is presented with an error message indicating that the name is a required field when trying to save

4b. The user removes their sex, weight, height, or date of birth

4b1. These are required, so they cannot update a profile without it

4b2. The user is presented with an error message indicating that the name is a required field when trying to save

5a. The user doesn’t want to update the profile at this time

5a1. The user has an option to cancel the profile update at any point via the “cancel” button

5b. There is an error saving the profile

5b1. The user is presented with an error message, notifying them of the failed save

### Use Case 3c: Profile Deletion

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the user can delete a profile

RaDoTech company → the application successfully deletes the profile

RaDoTech system → correctly removes the profile, updating records in the necessary tables

**Precondition**: The application is functional and the user is on the home page. The user has more than one profile

**Success guarantee**: The user can delete a profile

**Main success scenario:**

1. The user navigates to the “profiles” section of the app
2. The user selects a profile
3. The user is presented with a form of the profile’s name, sex, weight, height, date of birth, and the delete button
4. The user clicks the “delete” button
5. The user is returned to the list of their profiles, with the profile deleted

**Extensions**:

4a. The user deletes their only profile

4a1. Users require at least one profile, so it is not deleted

4a2. The user is returned to their list of profiles

5a. There is an error deleting the profile

5a1. The user is presented with an error message, notifying them of the failed delete

### Use Case 4: User Registration

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the user can register

RaDoTech company → the application successfully registers the user

RaDoTech system → correctly registers a user, updating records in the necessary tables

**Precondition**: The application is functional and the user is on the login/registration page.

**Success guarantee**: The user can register

**Main success scenario:**

1. The user navigates to the “registration” page
2. The user is presented with a form to enter their first name, last name, sex, weight, height, date of birth, email, password, and confirm password
3. The user enters their first name, last name, sex, weight, height, date of birth, email, password, and confirm password
4. The user clicks the “save and continue” button
5. The user is presented with the home page of the application, with their default profile created and selected

**Extensions**:

3a. The user doesn’t enter all the information

3a1. All fields are required, so they receive an error notification

3a2. The user cannot register until all the information is provided

3b. The user enters an invalid email

3b1. A valid email is required, so they are presented with an error notification

3c. The user enters passwords that don’t match

3c1. Passwords must match, so they are presented with an error notification

5a. There is an error registering the user

5a1. The user is presented with an error message, notifying them of the failed registration

5b. There is an error creating the user profile

5b1. The user is presented with an error message, notifying them of the failed profile creation

### Use Case 5: User Login

**Primary Actor**: User

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the user can login

RaDoTech company → the application successfully logs in the user

RaDoTech system → correctly logs in a user

**Precondition**: The application is functional and the user is on the login/registration page.

**Success guarantee**: The user can login

**Main success scenario:**

1. The user navigates to the “login” page
2. The user is presented with a form to enter their email and password
3. The user enters their email and password
4. The user clicks the “login” button
5. The user is presented with the home page of the application, with their default profile selected

**Extensions**:

3a. The user doesn’t enter all the information

3a1. All fields are required, so they receive an error notification

3a2. The user cannot register until all the information is provided

3b. The user enters an invalid email

3b1. A valid email is required, so they are presented with an error notification

3c. The user enters an incorrect password

3c1. They are presented with an error notification

3d. The user doesn’t exist

3d1. They are presented with an error notification

5a. There is an error logging in the user

5a1. The user is presented with an error message, notifying them of the failed login

### Use Case 6: Low Battery to Shutdown

**Primary Actor**: The RaDoTech Device

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

User → the user is notified of the low battery

RaDoTech company → the device correctly shuts down with no malfunctions

RaDoTech system → correctly notifies user of the device’s low battery to shutdown

**Precondition**: The RaDoTech device is near low battery capacity

**Success guarantee**: The device shuts down

**Main success scenario:**

1. The devices enters a low battery state
2. The device emits a visual message about the low battery
3. After some time, the device’s battery is depleted and emits a visual message about the shutdown
4. The device correctly shuts down
5. If the device was previously connected to the application, the application shows a message notifying a user about the shutdown

**Extensions**:

4a. The device malfunctions and does not correctly shutdown

4a1. *Use Case X*

5a. The device was not previously connected to the application

5a1. Continue as normal

### Use Case X: Device Malfunction

**Primary Actor**: The RaDoTech Device

**Scope**: The RaDoTech System

**Level**: User goal

**Stakeholders**:

RaDoTech company → the device shuts down safely

RaDoTech device → shuts down safely and does not cause any external damage

RaDoTech system → handles the error of device malfunction

**Precondition**: None

**Success guarantee**: The user is successfully notified about the device malfunction, with the next steps

**Main success scenario:**

1. The device enters a malfunctioned state
2. If the device was previously connected to the application, it sends a signal to it notifying it about the malfunction state
3. The device emits an audio message and flashing red marker on the device to signal an error
4. If the application receives a signal from the device, it displays an error message about the malfunction with the next steps

**Extensions**:

2a. The device was not previously connected to the application

2a1. Continue to the next step

4a. The application does not receive a signal from a device

4a1. Continue as normal

### Use Case Diagram
Below is the use case diagram covering the two major actors and their interactions with the use cases.

<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/0d0ecbc9-bf9f-4768-966d-fd4e24cc5618" />
    <p><em>Figure 2: Use Case Diagram</em></p>
</div>

## Sequence Diagrams

### Sequence Diagram: Profile Creation
Below is the sequence of creating a profile.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/6f96a5c5-1e4b-424c-a2e4-a2b55fcdcfcc" />
    <p><em>Figure 3: Sequence Diagram - Profile Creation</em></p>
</div>
<br>

### Sequence Diagram: Scan Measurement & Creation
Below is the sequence of running and creating a scan.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/0a92cccf-6c93-42e1-b782-cf5d667bbc7c" />
    <p><em>Figure 4: Sequence Diagram - Scan Measurements & Creation</em></p>
</div>

### Sequence Diagram: Device Not on Skin Long Enough
Below is the sequence of the device not being on skin long enough during a scan.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/9a458eca-06a2-49dd-9997-c3eeb99b5fe9" />
    <p><em>Figure 5: Sequence Diagram - Device Not on Skin Long Enough</em></p>
</div>

### Sequence Diagram: Device Shutdown Due to Low Battery
Below is the sequence of the device shutting down due to low battery.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/ded78382-1e4c-4218-9a85-9f580d643827" />
    <p><em>Figure 6: Sequence Diagram - Device Shutdown Due to Low Battery</em></p>
</div>

### Sequence Diagram: Scan Results - Organ Health
Below is the sequence of the scan results for organ health.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/eb279f1a-99fb-472d-924c-942a9c9a4240" />
    <p><em>Figure 6: Sequence Diagram - Scan Results: Organ Health</em></p>
</div>

## Class Diagram
Below is the class diagram for the application. We used the model view controller architecture for this system. The main controllers (`UserController`, `ScanController`, and `UserProfileController`) all use the `DatabaseManager` to do create, read, update, and delete operations with the database. The controllers are used throughout the application to retrieve user information, create profiles, store scans, etc.

The main models (`ScanModel`, `ProfileModel`, and `UserModel`) are used to organize the data going in and coming out of the database. It allows the controllers to more easily create their corresponding records in the database, and allows the ui to better interact with the data passed to it. 

The view (ui) utilizes the observer pattern that Qt provides through it's slots and signals functionality. The `MainWindow` is responsible for initializing, connecting the core functionality and control flow, and handling the core logic throughout the application. It is responsible for initializing and controlling the other widgets that the user will interact with. The following widgets have the most user interaction: 
1. LoginWidget: Provides the user with login and register capabilities, utilizing the user controller
2. ProfilesWidget: Provides the user a list of their profiles
3. ProfileEditWidget: Provides the user edit capabilities on a particular profile
4. HomeWidget: Provides the user a landing page and the ability to select their profile
5. MeasureNowWidget: Provides the user the ability to start and complete a scan
6. HistoryWidget: Provides the user a list of their past scans
7. ResultsWidget: Provides the user the result of a selected scan

Lastly, we have the two main utility classes `DatabaseManager` and `HealthMetricCalculator`. The database manager serves as the main database connection, while the health metric calculator provides utility functions for calculating organ health metrics and health indicators as discussed in the Ryodoraku Calculation sections.

<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/826cc794-1bca-4cf4-9274-13c40d015e07" />
    <p><em>Figure x: Class Diagram</em></p>
</div>

## Database Schema
Below is the database schema used in the application. It is a simple sqlite database with three tables (users, profile, and scan). The users has a one-to-many relationship with the profile, and the profile has a zero-to-many relationship with the scan.

<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/09d7d94b-f2c6-4b96-9f31-f437024bbfd5" />
    <p><em>Figure x: Database Schema</em></p>
</div>

## State Machine Diagram
### State Machine Diagram: RaDoTech Device
Below is the state machine diagram of the RaDoTech device.
<div style="text-align: left;">
    <img src="https://github.com/user-attachments/assets/eebdfbaa-485e-4c92-a918-bfc6bec7822d" />
    <p><em>Figure x: State Machine Diagram - RaDoTech Device</em></p>
</div>

## Traceability Matrix
Below is the requirements traceability matrix for the project. <br>
| ID | Requirement | Implemented by | Test | Description | Tested by |
| --- | --- | --- | --- | --- | --- |
| 1 | A user can register | UserController, LoginWidget, MainWindow | Register using the registration page | The login widget performs basic validation, emits a signal to the main window who uses the user controller to create the user. | Registering and verifying user creation in the database |
| 2 | A user can login | UserController, LoginWidget, MainWindow | Login using the login page | The login widget performs basic validation, emits a signal to the main window who uses the user controller to validate the user. | Logging in and verifying user arrives at the home page |
| 3 | A user can create a profile | UserProfileController, ProfileEditWidget, ProfilesWidget | Create a profile in the edit profile page | The edit profile widget performs basic validation, emits a signal to the ProfilesWidget who uses the user profile controller to create the profile | Creating a profile and verifying in the database |
| 4 | A user can update a profile | UserProfileController, ProfileEditWidget, ProfilesWidget | Update a profile in the edit profile page | The edit profile widget performs basic validation, emits a signal to the ProfilesWidget who uses the user profile controller to update the profile | Updating a profile and verifying in the database |
| 5 | A user can delete a profile | UserProfileController, ProfileEditWidget, ProfilesWidget | Delete a profile in the edit profile page | The edit profile widget performs basic validation, emits a signal to the ProfilesWidget who uses the user profile controller to delete the profile | Deleting a profile and verifying in the database |
| 6 | A user can create up to 5 profiles | UserProfileController, ProfileEditWidget, ProfilesWidget | Run id #5 five times  | n/a | Verifying in the database |
| 7 | A user can collect health data | ScanController, MeasureNowWidget | Start and complete the scan in the “measure now” section | The measure now widget loads pages for each scan. The user uses the device to complete the scan. Once all pages have been run through and the results page is about to be reached, it utilizes the ScanController to create the scan. | Start and complete the scan, verify the scan has been created in the database |
| 8 | The system processes raw data using algorithms based on ryodoraku technology to generate health metrics | HealthMetricCalculator | Unit testing on the HealthMetric Calculator | n/a | Unit testing the individual functions in the HealthMetricCalculator, ensuring results are as expected |
| 9 | A user can view their health metrics in an easy to understand visual format | HealthMetricCalculator, ResultsWidget | Complete a scan and view the results in the results widget | After completing a scan, the results page utilizes the health metric calculator to generate the calculations from the scan | Viewing the results page and verifying that the calculations are correct. |
| 10 | The system provides placeholders for specialists’ recommendations | TODO |  |  |  |
| 11 | A user can access historical health data  | UserProfileController, HistoryWidget | Navigate to the history section and view history of scans | History widget loads the scans for a given user profile (by using the user profile controller) | Viewing the history page and verifying that the previous scans  are correctly being displayed |
| 12 | The device’s charge depletes | DeviceController,  DeviceImageWidget | Turn on the device by clicking the power button and observe the battery level | Device image widget utilizes the device controller to turn on/off the device and emits signal to main window to update battery percentage ui | Turning on the device by clicking the power button and observing that the battery level decreases in the ui. |
| 13 | The device notifies when there is lower power | DeviceController,  DeviceImageWidget | Turn on the device by clicking the power button and observe the battery level reaching a low level | Device image widget utilizes the device controller to turn on/off the device and emits signal to main window to update battery percentage ui | Turning on the device by clicking the power button and observing that the battery level decreases until zero in the ui. Verifying that the battery changes color when low. |
| 14 | The device shuts down gracefully | DeviceController,  DeviceImageWidget | Turn on the device by clicking the power button and observe the battery level reaching zero | Device image widget utilizes the device controller to turn on/off the device and emits signal to main window to update battery percentage ui | Turning on the device by clicking the power button and observing that the battery level decreases to zero in the ui. Verifying that the battery changes color when at zero and the device turns off. |
| 15 | The device can be recharged | DeviceController,  DeviceImageWidget | Turn on the device by clicking the power button and observe the battery level reaching zero | Device image widget utilizes the device controller to manage it’s battery | Turning on the device by clicking the power button and observing that the battery level decreases to zero in the ui. Verifying that after clicking the battery level, the device begins to recharge |
| 16 | The system can handle a growing number of users | UserController, Database | Add multiple users | Users stored in the database allows for scaling with a  growing number of users  | Adding multiple users and verifying in the database  |
| 17 | The system is intuitive and user friendly | Entire UI | Navigate and interact with UI | n/a | Navigating and interacting with the UI |


## References
<a id="1">[1]</a> 
Yoshio NAKATANI MD,PhD, Tetsuo OISO MD,PhD. 2018. A Guide for Application of Ryodoraku Autonomous Nerve Regulatory Therapy. *Ryodoraku Medicine and Stimulus Therapy*, http://iarms.org/journal/rmst_v1_1_20.pdf
<br/>
<a id="2">[2]</a> 
Althof, S. E., McMahon, C. G., Waldinger, M. D., Serefoglu, E. C., Shindel, A. W., Adaikan, P. G., Becher, E. F., Dean, J., Giuliano, F., Hellstrom, W. J. G., Incrocci, L., & Jannini, E. A. (2014). An update on the treatment of premature ejaculation: A systematic review and meta-analysis. The Journal of Sexual Medicine, 11(6), 1506-1529. https://doi.org/10.1111/jsm.12560
