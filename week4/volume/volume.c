// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        exit(1);
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        exit(1);
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        // we don't need the input if we don't have the output
        fclose(input);
        printf("Could not open file.\n");
        exit(1);
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    // First 44 byes are the header of the file
    // initialize the array we will store the header in
    uint8_t header[HEADER_SIZE];
    // read from the file to memory. Fail if it does not complete properly
    if (fread(header, 1, HEADER_SIZE, input) != HEADER_SIZE)
    {
        // something went wrong, we can close these and exit
        fclose(input);
        fclose(output);
        printf("Failed reading file header\n");
        exit(1);
    }

    // write the header to the output file
    if (fwrite(header, 1, HEADER_SIZE, output) != HEADER_SIZE)
    {
        // something went wrong, we can close these and exit
        fclose(input);
        fclose(output);
        printf("Failed writing header data to file\n");
        exit(1);
    }

    // // TODO: Read samples from input file and write updated data to output file
    // after the header are 2 byte samples of audio
    // to change the volume, need to multiply the value by a number eg .5 for down, or 2 is up

    // Create a buffer for a single sample
    int16_t buffer;

    // Read single sample from input into buffer while there are samples left to read
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // Update volume of sample
        buffer *= factor;

        // Write updated sample to new file one buffer at a time
        if (fwrite(&buffer, sizeof(int16_t), 1, output) != 1)
        {
            // something went wrong, we can close these and exit
            fclose(input);
            fclose(output);
            printf("Failed writing audio data to file\n");
            exit(1);
        }
    }

    // Close files
    fclose(input);
    fclose(output);
}
