#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
string get_input_string(string prompt_string);
float compute_coleman_laiu_index(int letters, int words, int sentences);
void print_recommended_grade_level(float index);

int main(void)
{
    // Prompt the user for some text
    string text = get_input_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate the index value
    float coleman_laiu_index = compute_coleman_laiu_index(letters, words, sentences);

    // Print the result
    print_recommended_grade_level(coleman_laiu_index);
}

int count_letters(string text)
{
    int count = 0;
    // Count everything but the end of string marker.
    for (int i = 0; text[i] != '\0'; i++)
    {
        // count if alpha char
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    // presumes the final space is followed by a word, instructions indicate they will not start or end with a space.
    int count = 1;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isspace(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_sentences(string text)
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        // Per instructions, all sentences will end in . ! or ?
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}

string get_input_string(string)
{
    return get_string(prompt_string);
}

float compute_coleman_laiu_index(int letters, int words, int sentences)
{
    // Compute the Coleman-Liau index
    float L = (float)letters / words * 100;
    float S = (float)sentences / words * 100;
    // Return the final calculation
    return 0.0588 * L - 0.296 * S - 15.8;
}

void print_recommended_grade_level(float coleman_laiu_index)
{
    if (coleman_laiu_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (coleman_laiu_index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", round(coleman_laiu_index));
    }
}
