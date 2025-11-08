#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTE_BLOCK_SIZE 512

// Check the first 4 bytes to see if they are a JPEG Header
int is_jpeg_file(uint8_t *buffer)
{
    // Check the first 3 are 0xff 0xd8 0xff
    // the fourth byte’s first four bits are 1110.
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
    {
        // is a JPEG
        return 1;
    }
    // is not JPEG
    return 0;
}

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        exit(1);
    }

    // Open the memory input_file
    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file)
    {
        printf("Error opening FILE\n");
        exit(2);
    }

    // Create a buffer for a block of data
    uint8_t buffer[BYTE_BLOCK_SIZE];
    // create a file to write the JPEG into, start with null so we can write back into it for each
    // image
    FILE *output_file = NULL;
    // counter for the file names
    int image_count = 0;
    // place to stoare the file name
    char filename[10];

    // While there's still data left to read from the memory input_file
    while (fread(buffer, sizeof(uint8_t), BYTE_BLOCK_SIZE, input_file) == BYTE_BLOCK_SIZE)
    {
        // Create JPEGs from the data
        if (is_jpeg_file(buffer))
        {
            if (output_file)
            {
                // Reset the output file so we can process the next image
                fclose(output_file);
                output_file = NULL;
            }
            // The files you generate should each be named ###.jpg, where ### is a three-digit
            // decimal number, starting with 000 for the first image and counting up. Construct a
            // file name based on the index of the image
            sprintf(filename, "%03d.jpg", image_count);
            printf("%s\n", filename);

            // setup the output_file
            output_file = fopen(filename, "wb");
            if (!output_file)
            {
                // error opening file to write, print and exit
                printf("Error opening %s with fopen 'w'\n", filename);
                // close the input file
                fclose(input_file);
                exit(3);
            }
            // make sure we increment the image counter so we create all the filenames correctly
            image_count++;
        }

        if (output_file)
        {

            // We have a JPG, start writing
            size_t written = fwrite(buffer, sizeof(uint8_t), BYTE_BLOCK_SIZE, output_file);
            if (written != BYTE_BLOCK_SIZE)
            {
                // something went wrong in the write, log an error, close the open files, and
                // exit
                printf("Written bytes does not equal block size %i\n", BYTE_BLOCK_SIZE);
                fclose(input_file);
                fclose(output_file);
                exit(4);
            }
        }
    }
    // Things are done. Close the files and exit.
    fclose(input_file);
    if (output_file)
    {
        fclose(output_file);
    }
    exit(0);
}
