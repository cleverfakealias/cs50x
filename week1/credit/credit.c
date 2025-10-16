#include <cs50.h>
#include <stdio.h>


int get_input(void);
boolean is_valid(long digits);

int main(void)
{
    // prompt the user
}


int get_input(void)
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


boolean is_valid(long digits)
{
    // implement logic
    return true;
}


