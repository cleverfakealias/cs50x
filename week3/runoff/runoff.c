#include <cs50.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // start looping on all the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // if the current candidate matches the name we are voting for, update the preferences for
        // that voter
        if (strcmp(candidates[i].name, name) == 0)
        {
            // update the voter preferences
            preferences[voter][rank] = i;
            return true;
        }
    }
    // no candidate found
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop on the voters to start applying votes
    for (int voter_index = 0; voter_index < voter_count; voter_index++)
    {
        // loop on the candidates so we can update the correct preferences by their index
        for (int candidate_index = 0; candidate_index < candidate_count; candidate_index++)
        {
            // get the candidates rank from the voter preferences
            int voter_candidate_rank = preferences[voter_index][candidate_index];
            // check if they've been eliminated
            if (!candidates[voter_candidate_rank].eliminated)
            {
                // if not, increment the vote
                candidates[voter_candidate_rank].votes++;
                break;
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // start by looping on the candidates
    for (int candidate_index = 0; candidate_index < candidate_count; candidate_index++)
    {
        candidate current_candidate = candidates[candidate_index];
        // make sure they haven't been eliminated and if they have over half the votes, print them
        // immediately
        if (!current_candidate.eliminated && current_candidate.votes > voter_count / 2)
        {
            printf("%s\n", current_candidate.name);
            return true;
        }
    }
    // No one has won yet
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // per duck, we can start from the max int using "limits.h". Then we can update this value based
    // on the actual votes.
    int min_votes = INT_MAX;
    // loop on the candidates
    for (int candidate_index = 0; candidate_index < candidate_count; candidate_index++)
    {
        // check if they have been eliminated then check if the vote count is less than the
        // min_votes
        if (!candidates[candidate_index].eliminated &&
            candidates[candidate_index].votes < min_votes)
        {
            // update min_votes
            min_votes = candidates[candidate_index].votes;
        }
    }
    // Once done return the minimum
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min_votes)
{
    for (int candidate_index = 0; candidate_index < candidate_count; candidate_index++)
    {
        // compare all candidate votes to the min and their votes do not equal the minimum to win
        if (!candidates[candidate_index].eliminated &&
            candidates[candidate_index].votes != min_votes)
        {
            // at least one did not equal the minimum
            return false;
        }
    }
    // if we got through the loop, we have a tie.
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min_votes)
{
    for (int candidate_index = 0; candidate_index < candidate_count; candidate_index++)
    {
        // if the candidates vote count is the minimum votes, eliminate them
        if (!candidates[candidate_index].eliminated &&
            candidates[candidate_index].votes == min_votes)
        {
            // they can be eliminated because they have the least votes
            candidates[candidate_index].eliminated = true;
        }
    }
}
