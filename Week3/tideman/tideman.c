#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include </root/sandbox/tideman_folder/utilities/isEqual_string.h>
#include </root/sandbox/tideman_folder/utilities/isElementin_integer.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
int locked[MAX][MAX];
// Each pair has a winner, loser
struct pair
{
    int winner;
    int loser;
    int victory_strength;
};

// Array of candidates
string candidates[MAX];
struct pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        // printArray(ranks, candidate_count);
        record_preferences(ranks);
        printf("\n");
    }

    printf("Printing preferences... \n");
    for (int i=0; i < candidate_count; i++)
    {
        for (int j=0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // check if name belongs to a candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // register the rank of this ballot to ranks array.
            ranks[rank] = i;
            // printf("Place %d is taken by candidate index %i\n", rank + 1, i);
            return true;
        }
    }
    printf("Candidate - %s does not exist...\n", name);
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i=0; i < candidate_count - 1; i++)
    {
        int row = ranks[i];
        for (int j=i; j < candidate_count; j++)
        {
            int column = ranks[j];
            if (row != column)
            {
                preferences[row][column]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int row=1; row < candidate_count; row++)
    {
        for (int column=0; column < row; column++)
        {
            if (preferences[row][column] > preferences[column][row])
            {
                // printf("i: %i and j: %i\n", i, j);
                pairs[pair_count].winner = row;
                pairs[pair_count].loser = column;
                pairs[pair_count].victory_strength = preferences[row][column] - preferences[column][row];
                pair_count++;
            }
            else if (preferences[row][column] < preferences[column][row])
            {
                pairs[pair_count].winner = column;
                pairs[pair_count].loser = row;
                pairs[pair_count].victory_strength = preferences[column][row] - preferences[row][column];
                pair_count++;
            }
            else
            {
                printf("TIE! between %s and %s\n\n", candidates[row], candidates[column]);
            }
        }
    }
    
   return;
}

void swap(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}
  
// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    printf("Pairs before sorting...\n");
    for (int k=0; k < pair_count; k++)
    {
        printf("Winner: %i - Loser: %i - Victory strength: %i\n", pairs[k].winner, pairs[k].loser, pairs[k].victory_strength);
    }
    int i, j, temp;
    for(i = 0; i < pair_count - 1; i++) // to keep track of number of cycles
    {
        for(j = 0; j < pair_count - i - 1; j++) // to compare the elements within the particular cycle
        {
            // swap if one element is greater than its adjacent element
            if(pairs[j].victory_strength < pairs[j+1].victory_strength) 
            {
                swap(&pairs[j].winner, &pairs[j+1].winner);
                swap(&pairs[j].loser, &pairs[j+1].loser); 
                swap(&pairs[j].victory_strength, &pairs[j+1].victory_strength); 
            }
        }
    }
    printf("\nSorted by strength of the victory: ");
    for(i = 0; i < pair_count; i++)
    {
        printf("Winner: %i - Loser: %i - Victory strength: %i\n", pairs[i].winner, pairs[i].loser, pairs[i].victory_strength);
    }
    printf("\n\n");
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i=0; i < pair_count; i++)
    {
        int row = pairs[i].winner;
        int column = pairs[i].loser;
        // printf("row: %i column: %i\n", row, column);
        locked[row][column] = 1;
        locked[column][row] = -1;
    }

    printf("\nPrinting locked: ...\n");
    for (int i=0; i < candidate_count; i++)
    {
        for (int j=0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Print the winner of the election
void print_winner(void)
{
    int losers[MAX];
    int loser_count=0;
    // printf("Size of losers is %lu---\n", sizeof(losers)/sizeof(losers[0]));
    for (int row=0; row < candidate_count; row++)
    {
        for (int column=0; column < candidate_count; column++)
        {
            if (isElementin_integer(locked[row], candidate_count, -1) == false)
            {
                printf("Winner is %s! \n", candidates[row]);
                return;
                // break;
            }
            else if (isElementin_integer(locked[row], candidate_count, -1))
            {
                // printf("Row %i: ...\n", row);
                if (isElementin_integer(losers, loser_count, row) == false)
                {
                    // printf("Column: %i\n", column);
                    // printf("Loser count is: %i ...\n", loser_count);
                    // printf("Adding %i to losers...\n", row);
                    losers[loser_count] = row;
                    loser_count++;
                }
            }
        }
    }
    int temp_loser[MAX];
    int temp_loser_count=0;
    for (int i=0; i < pair_count; i++)
    {
        if (isElementin_integer(temp_loser, temp_loser_count, pairs[i].loser) == false)
        {
            temp_loser[i] = pairs[i].loser;
            temp_loser_count++;
        }
        if (temp_loser_count == loser_count)
        {
            printf("Winner is %s...!\n", candidates[temp_loser[temp_loser_count-1]]);
            return;
        }
    }
    // for (int i=0; i < loser_count; i++)
    // {
    //     printf("Losers are %i ...\n", losers[i]);
    // }
}
