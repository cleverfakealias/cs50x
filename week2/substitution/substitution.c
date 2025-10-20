#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int CIPHER_REQUIRED_LENGTH = 26;

string get_user_input(string prompt);
bool validate_cipher_key(string cipher_key);
void encrypt_with_cipher_key(string cipher_key, string plaintext);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("You must include a cipher key!\n");
        return 1;
    }
    // cipher will be in pos 1 of the array
    string cipher_key = argv[1];
    if (!validate_cipher_key(cipher_key))
    {
        return 1;
    }

    // prompt the user for the plaintext they want to encrypt
    string user_input = get_user_input("plaintext: ");
    // run it
    encrypt_with_cipher_key(cipher_key, user_input);
    return 0;
}

bool validate_cipher_key(string cipher_key)
{
    // fail fast if the cipher is less than 26
    if (strlen(cipher_key) != CIPHER_REQUIRED_LENGTH)
    {
        printf("%s", "Cipher must be 26 characters.\n");
        return false;
    }

    // use an array of length 26 which can be used to track if we encountered already
    bool seen_chars[26] = {false};

    for (int i = 0; i < CIPHER_REQUIRED_LENGTH; i++)
    {
        char current_char = cipher_key[i];
        // must be alpha numeric
        if (!isalpha(current_char))
        {
            printf("%s", "Cipher can only contain alpha characters.\n");
            return false;
        }
        // this one is funky but apparently correct syntax
        // toupper(current_char) - 'A' calculates the position of the character in the alphabet
        int char_index = toupper(current_char) - 'A';
        // check if we've encountered this character
        if (seen_chars[char_index])
        {
            // this is a duplicate log and return false
            printf("Character can only be used once: %c\n", current_char);
            return false;
        }
        // we didn't encounter yet, set the array index position to true.
        seen_chars[char_index] = true;
    }
    // we got through all the validations.
    return true;
}

string get_user_input(string prompt)
{
    // prompt user and return input
    string user_input;
    do
    {
        user_input = get_string("%s", prompt);
    } while (user_input < 0);
    return user_input;
}

void encrypt_with_cipher_key(string cipher_key, string plaintext)
{
    // we want to iterate over the length of plaintext so we can convert the characters using the
    // cipher
    printf("ciphertext: ");
    int plaintext_length = strlen(plaintext);
    for (int i = 0; i < plaintext_length; i++)
    {
        char current_plaintext_char = plaintext[i];
        if (isupper(current_plaintext_char))
        {
            int char_index = current_plaintext_char - 'A';
            // get the upper version of the cipher character
            char cipher_substitute = toupper(cipher_key[char_index]);
            printf("%c", cipher_substitute);
        }
        else if (islower(current_plaintext_char))
        {
            int char_index = current_plaintext_char - 'a';
            // get the lower version of the cipher character
            char cipher_substitute = tolower(cipher_key[char_index]);
            printf("%c", cipher_substitute);
        }
        else
        {
            // print non-alpha chars
            printf("%c", current_plaintext_char);
        }
    }
    // print new line after we are done
    printf("\n");
}
