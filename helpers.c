#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through all pixels; find average RGB of specific pixel, set pixel to
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE current_pixel = image[i][j];
            RGBTRIPLE *pCurrent_pixel = &image[i][j];

            int avg_rgb = round((current_pixel.rgbtRed + current_pixel.rgbtGreen + current_pixel.rgbtBlue) / 3.00);

            RGBTRIPLE new_color;
            new_color.rgbtRed = avg_rgb;
            new_color.rgbtGreen = avg_rgb;
            new_color.rgbtBlue = avg_rgb;

            *pCurrent_pixel = new_color;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through all pixels; find average RGB of specific pixel, set pixel to
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE current_pixel = image[i][j];
            RGBTRIPLE *pCurrent_pixel = &image[i][j];

            int sepiaRed = round(.393 * current_pixel.rgbtRed + .769 * current_pixel.rgbtGreen + .189 * current_pixel.rgbtBlue);
            int sepiaGreen = round(.349 * current_pixel.rgbtRed + .686 * current_pixel.rgbtGreen + .168 * current_pixel.rgbtBlue);
            int sepiaBlue = round(.272 * current_pixel.rgbtRed + .534 * current_pixel.rgbtGreen + .131 * current_pixel.rgbtBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            RGBTRIPLE new_color;
            new_color.rgbtRed = sepiaRed;
            new_color.rgbtGreen = sepiaGreen;
            new_color.rgbtBlue = sepiaBlue;

            *pCurrent_pixel = new_color;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // copy pixels into image copy
            RGBTRIPLE current_pixel = image[i][j];
            RGBTRIPLE *pCopy_pixel = &copy[i][j];

            *pCopy_pixel = current_pixel;
        }
    }

    // Iterate through all pixels; copy opposite side pixel from image copy to actual image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = (width - 1); j < width; j++, k--)
        {
            RGBTRIPLE current_pixel = copy[i][k];
            RGBTRIPLE *pCurrent_pixel = &image[i][j];

            *pCurrent_pixel = current_pixel;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // copy pixels into image copy
            RGBTRIPLE current_pixel = image[i][j];
            RGBTRIPLE *pCopy_pixel = &copy[i][j];

            *pCopy_pixel = current_pixel;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // establish box pixels; define all 9 pixels, check for edge cases (corner = 4 pixels, edge = 6, middle = 9)
            // TODO alt way to determine edge cases; if pixels aren't there pass, else add them into the total.
            // Maybe reverse process: start with 4 pixels and add each after checking for existence
            // or better nested ifs

            int num_of_pixels = 9;
            RGBTRIPLE current_pixel = copy[i][j];
            RGBTRIPLE *pCurrent_pixel = &image[i][j];
            RGBTRIPLE zero_pixel;
            zero_pixel.rgbtRed = 0;
            zero_pixel.rgbtGreen = 0;
            zero_pixel.rgbtBlue = 0;

            // Declare each of the box pixels

            RGBTRIPLE tl_pixel;
            RGBTRIPLE tc_pixel;
            RGBTRIPLE tr_pixel;

            RGBTRIPLE ml_pixel;
            RGBTRIPLE mc_pixel = current_pixel;
            RGBTRIPLE mr_pixel;

            RGBTRIPLE bl_pixel;
            RGBTRIPLE bc_pixel;
            RGBTRIPLE br_pixel;

            // tl corner
            if (i == 0 && j == 0)
            {
                num_of_pixels = 4;
                tl_pixel = zero_pixel;
                tc_pixel = zero_pixel;
                tr_pixel = zero_pixel;

                ml_pixel = zero_pixel;
                mr_pixel = copy[i][j + 1];

                bl_pixel = zero_pixel;
                bc_pixel = copy[i + 1][j];
                br_pixel = copy[i + 1][j + 1];
            }

            // top edge
            else if (i == 0 && j != 0 && j != (width - 1))
            {
                num_of_pixels = 6;
                tl_pixel = zero_pixel;
                tc_pixel = zero_pixel;
                tr_pixel = zero_pixel;

                ml_pixel = copy[i][j - 1];
                mr_pixel = copy[i][j + 1];

                bl_pixel = copy[i + 1][j - 1];
                bc_pixel = copy[i + 1][j];
                br_pixel = copy[i + 1][j + 1];
            }

            // tr corner
            else if (i == 0 && j == (width - 1))
            {
                num_of_pixels = 4;
                tl_pixel = zero_pixel;
                tc_pixel = zero_pixel;
                tr_pixel = zero_pixel;

                ml_pixel = copy[i][j - 1];
                mr_pixel = zero_pixel;

                bl_pixel = copy[i + 1][j - 1];
                bc_pixel = copy[i + 1][j];
                br_pixel = zero_pixel;
            }

            // right edge
            else if (i != 0 && i != (height - 1) && j == (width - 1))
            {
                num_of_pixels = 6;
                tl_pixel = copy[i - 1][j - 1];
                tc_pixel = copy[i - 1][j];
                tr_pixel = zero_pixel;

                ml_pixel = copy[i][j - 1];
                mr_pixel = zero_pixel;

                bl_pixel = copy[i + 1][j - 1];
                bc_pixel = copy[i + 1][j];
                br_pixel = zero_pixel;
            }

            // br corner
            else if (i == (height - 1) && j == (width - 1))
            {
                num_of_pixels = 4;
                tl_pixel = copy[i - 1][j - 1];
                tc_pixel = copy[i - 1][j];
                tr_pixel = zero_pixel;

                ml_pixel = copy[i][j - 1];
                mr_pixel = zero_pixel;

                bl_pixel = zero_pixel;
                bc_pixel = zero_pixel;
                br_pixel = zero_pixel;
            }

            // bottom edge
            else if (i == (height - 1) && j != 0 && j != (width - 1))
            {
                num_of_pixels = 6;
                tl_pixel = copy[i - 1][j - 1];
                tc_pixel = copy[i - 1][j];
                tr_pixel = copy[i - 1][j + 1];

                ml_pixel = copy[i][j - 1];
                mr_pixel = copy[i][j + 1];

                bl_pixel = zero_pixel;
                bc_pixel = zero_pixel;
                br_pixel = zero_pixel;
            }

            // bl corner
            else if (i == (height - 1) && j == 0)
            {
                num_of_pixels = 4;
                tl_pixel = zero_pixel;
                tc_pixel = copy[i - 1][j];
                tr_pixel = copy[i - 1][j + 1];

                ml_pixel = zero_pixel;
                mr_pixel = copy[i][j + 1];

                bl_pixel = zero_pixel;
                bc_pixel = zero_pixel;
                br_pixel = zero_pixel;
            }

            // left edge
            else if (i != 0 && i != (height - 1) && j == 0)
            {
                num_of_pixels = 6;
                tl_pixel = zero_pixel;
                tc_pixel = copy[i - 1][j];
                tr_pixel = copy[i - 1][j + 1];

                ml_pixel = zero_pixel;
                mr_pixel = copy[i][j + 1];

                bl_pixel = zero_pixel;
                bc_pixel = copy[i + 1][j];
                br_pixel = copy[i + 1][j + 1];
            }

            // center pixels
            else
            {
                num_of_pixels = 9;
                tl_pixel = copy[i - 1][j - 1];
                tc_pixel = copy[i - 1][j];
                tr_pixel = copy[i - 1][j + 1];

                ml_pixel = copy[i][j - 1];
                mr_pixel = copy[i][j + 1];

                bl_pixel = copy[i + 1][j - 1];
                bc_pixel = copy[i + 1][j];
                br_pixel = copy[i + 1][j + 1];
            }

            // // TESTING
            // if (num_of_pixels != 10)
            // {
            //     printf("Pixel #: [%i, %i]\nNumber of pixels: %i\n", i, j, num_of_pixels);
            // }

            // Calculate avg color for each blur box
            int avg_pixel_r = round((tl_pixel.rgbtRed + tc_pixel.rgbtRed + tr_pixel.rgbtRed + ml_pixel.rgbtRed + mc_pixel.rgbtRed +
                                     mr_pixel.rgbtRed + bl_pixel.rgbtRed + bc_pixel.rgbtRed + br_pixel.rgbtRed) /
                                    (float) num_of_pixels);
            int avg_pixel_g =
                round((tl_pixel.rgbtGreen + tc_pixel.rgbtGreen + tr_pixel.rgbtGreen + ml_pixel.rgbtGreen + mc_pixel.rgbtGreen +
                       mr_pixel.rgbtGreen + bl_pixel.rgbtGreen + bc_pixel.rgbtGreen + br_pixel.rgbtGreen) /
                      (float) num_of_pixels);
            int avg_pixel_b =
                round((tl_pixel.rgbtBlue + tc_pixel.rgbtBlue + tr_pixel.rgbtBlue + ml_pixel.rgbtBlue + mc_pixel.rgbtBlue +
                       mr_pixel.rgbtBlue + bl_pixel.rgbtBlue + bc_pixel.rgbtBlue + br_pixel.rgbtBlue) /
                      (float) num_of_pixels);

            // Create an avg color pixel to write to file
            RGBTRIPLE overall_avg;
            overall_avg.rgbtRed = avg_pixel_r;
            overall_avg.rgbtGreen = avg_pixel_g;
            overall_avg.rgbtBlue = avg_pixel_b;

            *pCurrent_pixel = overall_avg;
        }
    }
}
