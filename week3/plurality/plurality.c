#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Object representing each candidate and their vote count so we can increment.
typedef struct
{
    string name;
    int votes;
} candidate;

#define MAXIMUM_CANDIDATES 9              // maximum number of candidates we can have
candidate candidates[MAXIMUM_CANDIDATES]; // initialize an array of the struct objects
int candidate_count = 0;                  // set the total count at the top so we can use in other methods

// Start Prototypes
void validate_candidate_count();
void update_candidates(int candidates_count, string candidates[]);
int get_voters_count(void);
bool vote(string name);
void process_votes(int voter_count);
int get_max_vote(void);
void print_winner(void);

// End Prototypes

int main(int argc, string argv[])
{
    // argc/argv[0] represents the program name.
    // The real count we care about for our loop. -1 for the program name.
    candidate_count = argc - 1;
    // Start validating the inputs, we need at least one candidate, but not more than 9.
    validate_candidate_count();

    // new concept form Duck. Use a pointer to the first element to update the candidates list
    string *candidates_names = &argv[1];
    update_candidates(candidate_count, candidates_names);
    process_votes(get_voters_count());
    print_winner();
}

// Check the input to make sure it's not more than MAXIMUM_CANDIDATES
void validate_candidate_count()
{
    if (candidate_count == 0)
    {
        printf("You must provide at least one candidate. Usage: plurality [candidate_name ...] \n");
        // Using exit from stdlib to cleanly close the app on error
        exit(1);
    }
    else if (candidate_count > MAXIMUM_CANDIDATES)
    {
        printf("Maxium number of candidate is %i\n", MAXIMUM_CANDIDATES);
        // using a different code to differentiate the errors
        exit(2);
    }
}

void update_candidates(int argc, string candidates_names[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name =
            candidates_names[i]; // use the pointer reference to update the underlying arrary.
        candidates[i].votes = 0; // initialize with 0 start
    }
}

int get_voters_count(void)
{
    return get_int("Number of voters: ");
}

void process_votes(int voter_count)
{
    for (int i = 0; i < voter_count; i++)
    {
        string candidate_name = get_string("Vote: ");
        // Process the vote
        if (!vote(candidate_name))
        {
            printf("Invalid vote.\n");
        }
    }
}

// Increment the vote count if the name matches.
bool vote(string name)
{
    // Iterate over each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if candidate's name matches given name
        // strcmp returns 0 if they are the same
        if (strcmp(candidates[i].name, name) == 0)
        {
            // If yes, increment candidate's votes and return true
            candidates[i].votes++;
            return true;
        }
    }
    // If no match, return false
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Find the maximum number of votes
    int max_votes = get_max_vote();
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max_votes)
        {
            // Print the candidate (or candidates) with maximum votes
            printf("%s\n", candidates[i].name);
        }
    }
}

int get_max_vote(void)
{
    // Calculate the maximum number of votes received by a candidate
    int max_vote = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max_vote)
        {
            max_vote = candidates[i].votes;
        }
    }
    return max_vote;
}