#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop the rows
    for (int row_index = 0; row_index < height; row_index++)
    {
        // loop the pixels
        for (int pixel_index = 0; pixel_index < width; pixel_index++)
        {
            // Dereference the individual pixel RGBTRIPLE from the image
            RGBTRIPLE *pixel = &image[row_index][pixel_index];
            // calculate the average for the pixel
            int pixel_average =
                (int) (round((pixel->rgbtRed + pixel->rgbtGreen + pixel->rgbtBlue) / 3.0));
            // cast the int to an 8 bit integer so we can apply it back to the RGB values
            uint8_t grey = (uint8_t) pixel_average;
            pixel->rgbtRed = grey;
            pixel->rgbtGreen = grey;
            pixel->rgbtBlue = grey;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop the rows
    for (int row_index = 0; row_index < height; row_index++)
    {
        // loop through half of the pixels
        for (int pixel_index = 0; pixel_index < width; pixel_index++)
        {
            RGBTRIPLE original_pixel = image[row_index][pixel_index];
            // do our math on the pixel colors
            double sepia_red_calc = .393 * original_pixel.rgbtRed +
                                    .769 * original_pixel.rgbtGreen +
                                    .189 * original_pixel.rgbtBlue;
            double sepia_green_calc = .349 * original_pixel.rgbtRed +
                                      .686 * original_pixel.rgbtGreen +
                                      .168 * original_pixel.rgbtBlue;
            double sepia_blue_calc = .272 * original_pixel.rgbtRed +
                                     .534 * original_pixel.rgbtGreen +
                                     .131 * original_pixel.rgbtBlue;

            // round our calculations
            int sepia_red = (int) round(sepia_red_calc);
            int sepia_green = (int) round(sepia_green_calc);
            int sepia_blue = (int) round(sepia_blue_calc);
            if (sepia_red > 255)
            {
                sepia_red = 255;
            }
            if (sepia_green > 255)
            {
                sepia_green = 255;
            }
            if (sepia_blue > 255)
            {
                sepia_blue = 255;
            }

            // set the colors on the image
            image[row_index][pixel_index].rgbtRed = (uint8_t) sepia_red;
            image[row_index][pixel_index].rgbtGreen = (uint8_t) sepia_green;
            image[row_index][pixel_index].rgbtBlue = (uint8_t) sepia_blue;
        }
    }
}

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop the rows
    for (int row_index = 0; row_index < height; row_index++)
    {
        // loop through half of the pixels
        for (int pixel_index = 0; pixel_index < width / 2; pixel_index++)
        {
            // Get the mirror pixel index working backward from the end
            int mirror_pixel_index = width - 1 - pixel_index;
            // temp swap variable
            RGBTRIPLE temp_pixel = image[row_index][pixel_index];
            // Start swapping
            // first set the current to the value of the mirror pixel
            image[row_index][pixel_index] = image[row_index][mirror_pixel_index];
            // then set the mirror pixel to the value of the current
            image[row_index][mirror_pixel_index] = temp_pixel;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temp buffer we can use to store all the new calculated pixels in
    RGBTRIPLE temp_buffer[height][width];

    // loop the rows
    for (int current_row_index = 0; current_row_index < height; current_row_index++)
    {
        // loop through the pixels
        for (int current_pixel_index = 0; current_pixel_index < width; current_pixel_index++)
        {
            // add up all the RGB values
            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;
            int neighbor_pixel_count = 0;

            // loop again to calculate indexes of the neigboring pixels to the current.
            // so we can visit the 3x3 area around the current pixel column.
            for (int neighbor_row_offset = -1; neighbor_row_offset <= 1; neighbor_row_offset++)
            {
                for (int neighbor_pixel_offset = -1; neighbor_pixel_offset <= 1;
                     neighbor_pixel_offset++)
                {
                    // start processing the neighbor pixels
                    int neighbor_row = current_row_index + neighbor_row_offset;
                    int neighbor_pixel = current_pixel_index + neighbor_pixel_offset;
                    if (neighbor_row < 0 || neighbor_row >= height || neighbor_pixel < 0 ||
                        neighbor_pixel >= width)
                    {
                        // no op, this is outside the bounds of our 3x3 grid
                        continue;
                    }
                    // Once we are done checking the neighbor pixels we can do our average
                    // Add to the sums so we can average them after we finish this loop
                    sum_red += image[neighbor_row][neighbor_pixel].rgbtRed;
                    sum_green += image[neighbor_row][neighbor_pixel].rgbtGreen;
                    sum_blue += image[neighbor_row][neighbor_pixel].rgbtBlue;
                    neighbor_pixel_count++;
                }
            }
            // Now we are back outside our neighbors loop, we can do our averages and store them in
            // the temp buffer
            temp_buffer[current_row_index][current_pixel_index].rgbtRed =
                (uint8_t) round((double) sum_red / neighbor_pixel_count);
            temp_buffer[current_row_index][current_pixel_index].rgbtGreen =
                (uint8_t) round((double) sum_green / neighbor_pixel_count);
            temp_buffer[current_row_index][current_pixel_index].rgbtBlue =
                (uint8_t) round((double) sum_blue / neighbor_pixel_count);
        }
    }
    // Now we've processed the whole image into the temp buffer, we can push the blurred pixels back
    // into the original loop the rows
    for (int current_row_index = 0; current_row_index < height; current_row_index++)
    {
        // loop through the pixels
        for (int current_pixel_index = 0; current_pixel_index < width; current_pixel_index++)
        {
            image[current_row_index][current_pixel_index] =
                temp_buffer[current_row_index][current_pixel_index];
        }
    }
}
