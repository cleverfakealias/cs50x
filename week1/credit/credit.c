#include <cs50.h>
#include <stdio.h>

long get_input_number(void);
long get_card_number_length(long card_number);
long get_start_digits(long card_number, int wanted_digits);
bool compute_checksum_validity(long card_number);
string get_card_type(long card_nubmer);

int main(void)
{
    long card_number = get_input_number();
    // check the validity and return the card type
    printf("%s", get_card_type(card_number));
}

long get_input_number()
{
    // prompt user and return card number
    long card_number;
    do
    {
        // get long because card numbers are bigger than int can hold.
        card_number = get_long("Number: ");
    }
    while (card_number < 0);
    return card_number;
}

long get_card_number_length(long card_number)
{
    // C doesn't have any built-ins that let us do this easily.
    // Instead we are dividing the card number by 10 and counting up
    // the number of times we can do this until we reach zero.
    // TODO: Find package / library that can do this? DDB says maybe use math.h log10.
    long card_number_length = 0;
    while (card_number > 0)
    {
        card_number /= 10;
        card_number_length++;
    }
    return card_number_length;
}

long get_start_digits(long card_number, int wanted_digits)
{
    // return the wanted digits of the card nubmer so we can verify what type of card it is.
    // once the nubmer length is equal to the "wanted" we have our number
    while (get_card_number_length(card_number) != wanted_digits)
    {
        // remove the last digit
        card_number /= 10;
    }
    // make sure to cast back to int
    return (long) card_number;
}

bool compute_checksum_validity(long card_number)
{
    int checksum_total = 0;
    bool is_every_other_digit = false;
    // Multiply every other digit by 2, starting with the number’s
    //  second-to-last digit, and then add those products’ digits together.
    while (card_number > 0)
    {
        // get the current last digit
        int current_digit = card_number % 10;
        if (is_every_other_digit)
        {
            // if every other digit, multiply by 2
            current_digit *= 2;
            // if the result is 2 digits, we need to get the distinct numbers,
            // and update the total with them added together.
            checksum_total += (current_digit / 10 + current_digit % 10);
        }
        else
        {
            // this was not every other, just add the digit to the sum
            checksum_total += current_digit;
        }
        // move to the next digit
        card_number /= 10;
        // update the every other varable to alternate
        is_every_other_digit = !is_every_other_digit;
    }

    // If the total’s last digit is 0 (or, put more formally,
    // if the total modulo 10 is congruent to 0), the number is valid!
    return (checksum_total % 10 == 0) ? true : false;
}

string get_card_type(long card_number)
{
    string card_type = "INVALID\n";
    // get the total card number length
    long card_number_length = get_card_number_length(card_number);

    // get first digits.. probably cleaner way to do this but it works.
    long first_digit = get_start_digits(card_number, 1);
    long first_two_digits = get_start_digits(card_number, 2);

    if (compute_checksum_validity(card_number))
    {
        // American Express numbers are 15 length and start with 34 or 37
        if (card_number_length == 15 && (first_two_digits == 34 || first_two_digits == 37))
        {
            card_type = "AMEX\n";
        }
        // MasterCard numbers are 16 length and start with 51, 52, 53, 54, or 55
        else if (card_number_length == 16 && (first_two_digits >= 51 && first_two_digits <= 55))
        {
            card_type = "MASTERCARD\n";
        }
        // VISA numbers are 13 or 16 length and start with 4
        else if ((card_number_length == 13 || card_number_length == 16) && first_digit == 4)
        {
            card_type = "VISA\n";
        }
    }
    return card_type;
}
