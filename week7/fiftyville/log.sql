-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Get the SCHEMA
.schema
-- get list of the tables
.tables
-- see what descriptions look LIKE
SELECT description
  FROM crime_scene_reports;

-- look for descriptions that include the word theft to see what we can find,
-- just getting whole row so i can see the rest of the data
-- ordered by the primary key descending, assuming its in order via sequence

 * FROM crime_scene_reports WHERE  order by id desc;

-- get the exact event based ON the date AND street
SELECT * FROM crime_scene_reports
WHERE year = 2024
AND month = 7
AND day = 28;

-- combine above 2 queries
SELECT * FROM crime_scene_reports
WHERE year = 2024
AND month = 7
AND day = 28
AND description LIKE '%theft%';


-- get the transcripts mentioning bakery FROM the same day as the event

SELECT transcript FROM interviews
WHERE year = 2024
AND month = 7
AND day = 28
AND LOWER(transcript) LIKE '%bakery%';


-- start building up some LEFT JOINs ON this data since they have some shared information
SELECT distinct peeps.*, pc.*, fl.*
FROM people peeps
LEFT JOIN bakery_security_logs bsl ON bsl.license_plate = peeps.license_plate
LEFT JOIN bank_accounts ba ON ba.person_id = peeps.id
LEFT JOIN atm_transactions at ON at.account_number = ba.account_number
LEFT JOIN phone_calls pc ON pc.caller = peeps.phone_number
LEFT JOIN passengers pass ON pass.passport_number = peeps.passport_number
LEFT JOIN flights fl ON fl.id = pass.flight_id
LEFT JOIN airports ap ON ap.id = fl.origin_airport_id
-- start building up our filters
-- date of the incident
WHERE at.year = 2024
AND at.month = 7
AND at.day = 28

-- to get license plates leaving
AND bsl.year = 2024
AND bsl.month = 7
AND bsl.day = 28
AND bsl.activity = 'exit'
-- within 10 minutes of the theft
AND (
    (bsl.hour = 10 AND bsl.minute BETWEEN 15 AND 25)
  )
-- call details, same day
AND pc.year = 2024
AND pc.month = 7
AND pc.day = 28
-- call duration was less than one minute
AND pc.duration < 60
-- suspect was withdrawing money FROM atm ON legget street
AND at.transaction_type = 'withdraw'
AND at.atm_location = 'Leggett Street'
-- flight id for the earliest flight the following day
AND fl.id = (
    SELECT id
    FROM flights fl2
    -- flights ON the next day
    WHERE fl2.year = 2024
    AND fl2.day = 29
    -- leaving FROM Fiftyville
    AND fl2.origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
    -- earliest flight out
    order by fl2.hour, fl2.minute asc
    limit 1
);

-- get the accomplice by phone number

SELECT * FROM people WHERE phone_number = '(375) 555-8161';

-- get the destination airport
SELECT * FROM airports WHERE id = 4;


-- Bruce
-- Robin
-- LaGuardia Airport - New York City


-- final query including the final SELECT and proper accomplice / airport city
-- also fix the formatting so this reads nicer.
SELECT DISTINCT suspect.name as SUSPECT, destap.city as DESTINATION_CITY, accomplice.name as ACCOMPLICE
  FROM people suspect
    LEFT JOIN bakery_security_logs bsl
    ON bsl.license_plate = suspect.license_plate
    LEFT JOIN bank_accounts ba
    ON ba.person_id = suspect.id
    LEFT JOIN atm_transactions at
    ON at.account_number = ba.account_number
    LEFT JOIN phone_calls pc
    ON pc.caller = suspect.phone_number
    LEFT JOIN passengers pass
    ON pass.passport_number = suspect.passport_number
    LEFT JOIN flights fl
    ON fl.id = pass.flight_id
    -- add in the airports table so we can get the city name
    LEFT JOIN airports ap
    ON ap.id = fl.origin_airport_id
    -- this one is for the destination city
    LEFT JOIN airports destap
    ON destap.id = fl.destination_airport_id
    -- adding in the final LEFT JOINs to get the accomiplice LEFT JOINing ON the receiver phone number
    LEFT JOIN people accomplice
    ON accomplice.phone_number = pc.receiver
    -- start building up our filters
    -- date of the incident
    WHERE at.year = 2024
      AND at.month = 7
      AND at.day = 28
      -- to get license plates leaving
      AND bsl.year = 2024
      AND bsl.month = 7
      AND bsl.day = 28
      AND bsl.activity = 'exit'
      -- within 10 minutes of the theft
      AND (
          (bsl.hour = 10 AND bsl.minute BETWEEN 15 AND 25)
        )
      -- call details, same day
      AND pc.year = 2024
      AND pc.month = 7
      AND pc.day = 28
      -- call duration was less than one minute
      AND pc.duration < 60
      -- suspect was withdrawing money FROM atm ON legget street
      AND at.transaction_type = 'withdraw'
      AND at.atm_location = 'Leggett Street'
      -- flight id for the earliest flight the following day
      AND fl.id = (
          SELECT id
            FROM flights fl2
              -- flights ON the next day
              WHERE fl2.year = 2024
                AND fl2.day = 29
                -- leaving FROM Fiftyville
                AND fl2.origin_airport_id = (
                  SELECT id FROM airports WHERE city = 'Fiftyville'
                  )
                -- earliest flight out
              ORDER BY fl2.hour, fl2.minute ASC
              LIMIT 1
      );
