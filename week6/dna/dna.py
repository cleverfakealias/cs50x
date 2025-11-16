import csv
import sys


DatabaseRow = dict[str, str]
Database = list[DatabaseRow]

AGAT = "AGAT"


def check_command_line_args():
    if len(sys.argv) != 3:
        print("Invalid usage: python dna.py data.csv sequence.txt")
        sys.exit(1)


def get_database_rows_from_file(database_file_path) -> Database:
    try:
        with open(database_file_path, 'r') as csv_file:
            # return all the rows in the database
            return [row for row in csv.DictReader(csv_file)]
    except (OSError):
        print("Error opening files")
        sys.exit(2)


def get_sequence_string_from_file(sequence_file_path) -> str:
    try:
        with open(sequence_file_path, 'r') as sequence_file:
            return sequence_file.read()
    except (Exception):
        print("Error opening files")
        sys.exit(2)


def get_sequence_short_tandem_repeat_counts(short_tandem_repeat_list, sequence):
    sequence_short_tandem_repeat_counts = {}
    for short_tandem_repeat in short_tandem_repeat_list:
        sequence_short_tandem_repeat_counts[short_tandem_repeat] = longest_match(
            sequence, short_tandem_repeat)
    return sequence_short_tandem_repeat_counts


def check_matching_profiles(database_rows, sequence_short_tandem_repeat_counts):
    # Loop the DB roows
    for db_row in database_rows:
        person_name = db_row.get('name')
        # dictionary comprehension to get just the k:v pairs we care about. Converting the string to int here for comparison
        person_str_counts = {k: int(v) for k, v in db_row.items() if k != 'name'}
        # if it matches return the name
        if person_str_counts == sequence_short_tandem_repeat_counts:
            return person_name
    # if we got here, no one matched.
    return "No match"


def main():

    # Check for command-line usage
    check_command_line_args()
    # load the files
    database_rows: Database = get_database_rows_from_file(sys.argv[1])
    sequence: str = get_sequence_string_from_file(sys.argv[2])
    # simple list of just the STRs we care about
    short_tandem_repeat_list = list(database_rows[0].keys())[1:]
    # object with STR: Count
    sequence_short_tandem_repeat_counts = get_sequence_short_tandem_repeat_counts(
        short_tandem_repeat_list, sequence)
    # Check database for matching profiles
    matching_person = check_matching_profiles(database_rows, sequence_short_tandem_repeat_counts)
    print(matching_person)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
