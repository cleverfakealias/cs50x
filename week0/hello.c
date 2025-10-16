#include <cs50.h>
#include <stdio.h>

string get_name(void)
{
    return get_string("What is your name?\n");
}

int main(void)
{
    string name;
    name = get_name();
    printf("hello, %s\n", name);
}
