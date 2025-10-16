#include <cs50.h>
#include <stdio.h>

int get_height(void);
void print_row(int spaces, int bricks);
void print_spaces(int spaces);
void print_bricks(int bricks);

void print_row(int spaces, int bricks);

int main(void)
{
    // prompt the user
    int height = get_height();
    // start looping on the int provided starting at one so we print the proper number of lines
    for (int row_count = 1; row_count <= height; row_count++)
    {
        // calculate how many empty spaces we need
        int empty_spaces = height - row_count;
        // not really needed, could pass row count directly but this makes it clearer
        int bricks = row_count;
        // run it
        print_row(empty_spaces, bricks);
    }
}

int get_height(void)
{
    // refactored out into it's own method
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1);
    return height;
}

void print_row(int spaces, int bricks)
{
    // orchestrator method that performs the actions and adds new line
    print_spaces(spaces);
    print_bricks(bricks);
    printf("\n");
}

void print_spaces(int spaces)
{
    // simple loop to print spaces
    for (int i = 0; i < spaces; i++)
    {
        printf(" ");
    }
}

void print_bricks(int bricks)
{
    // simple loop to print the blocks
    for (int i = 0; i < bricks; i++)
    {
        printf("#");
    }
}
