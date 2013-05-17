/*
    Learning to Read Bushman - Handwriting Recognition for Bushman Languages
    Copyright (C) 2010 Kyle Williams <kwilliams@cs.uct.ac.za>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/* Features based on:
 *
 * N. Ahmed, T. Natarajan, and K.R. Rao. Discrete cosine transfom. Computers, IEEE Transactions on, C-23(1):90 – 93, jan. 1974.
 *
 * T. D. Bui D. Nguyen. On the problem of classifying vietnamese online handwritten characters. pages 803–808.
 *
 * J.H. AlKhateeb, Jinchang Ren, Jianmin Jiang, S.S. Ipson, and H. El Abed. Word-based handwritten arabic scripts recognition
 * using dct features and neural network classiﬁer. pages 1 –5, jul. 2008.
 *
 * Gerhard Rigoll Anja Brakensiek. A comparison of character n-grams and dictionaries used for script recognition.
 *
 */

/* This class is used to compute the DCT feature set. Performs transform, quantization and zig-zag ordering
 *
 * Makes use of the FFTW3 library in order to perform the DCT transform
 */

#include "dct.h"
#include <iostream>

/* Constructor */
DCT::DCT ( Magick::Image * i ) {
    image = new Magick::Image();
    image = i;
}

/* Destrcutor */
DCT::~DCT() {

}

/* Computes the DCT coefficients.
 * @b the block size
 * @s the number of output coefficients
 * @q quantize the coefficients
 */
std::vector<double> DCT::getDCT(int bh, int bw, int s, bool q) {

    /* The feature vector */
    std::vector<double> f;

    /* Initialize the plan and set the block size */
    fftw_plan p;
    int block_width = bw;
    int block_height = bh;

    /* Initialise the input and output arrays and create the plan */
    in = ( double* ) fftw_malloc ( block_height*block_width * sizeof ( double ) );
    out = ( double* ) fftw_malloc ( block_height*block_width * sizeof ( double ) );
    p = fftw_plan_r2r_2d ( block_height, block_width, in, out, FFTW_REDFT10, FFTW_REDFT10, FFTW_MEASURE );

    /* Loop through the blocks */
    for ( unsigned int i = 0; i < image->rows(); i+=block_height ) {
        for ( unsigned int j = 0; j < image->columns(); j+=block_width ) {

            /* Create the input array for the current block */
            int y = 0;
            for ( unsigned  int k = i; k < i+block_height; k++ ) {
                int x = 0;
                for ( unsigned int l = j; l < j+block_width; l++ ) {
                    in[y*block_width+x] = Magick::ColorGray ( image->pixelColor ( l,k ) ).shade();
                    x++;
                }
                y++;
            }

            /* Execute the plan to perform the DCT-II for the current block */
            fftw_execute ( p );

            /* Quantize the block coefficients if necessary */
            if (q == true) {
                quantize(bw, s);
            }

            /* Extract the block coefficients in a zig-zag order */
            std::vector<double> zz = zigzag(block_height, block_width, s);

            double max = 1;
            /* Normalise based on size */
            for (unsigned int z = 1; z < zz.size(); z++) {
                if (( abs(zz.at(z))) > max) {
                    max = abs(zz.at(z));
                }
            }

            /* Add the block coeeficients to the feature matrix.
            * Coefficients are rounded off to the nearest integer.
            * Ignore the first coefficient
            */
            for (int z = 0; z < s; z++) {
                f.push_back( zz.at(z));
            }

        }
    }

    /* Destroy the plan and free the arrays */
    fftw_destroy_plan ( p );
    fftw_free ( out );
    fftw_free ( in );

    /* Return the fearure vector */
    return f;

}

/* Quantizes the feature vector using a popular quantization matrix (taken from Wikipedia)
 * In the case where the block is greater than 8 and more than 8 coefficients are wanted, no quantization is performed
 *
 * @b the block size
 * @s the size of the output coefficient vector
 */

void DCT::quantize(int b, int s) {

    /* If block size is greater than 8 and more than 8 coefficients are wanted then do not perform quantization since quantization uses an 8x8 matrix
     * Instead, just normalise with respect to the largest value excluding the DC value
     */
    if ((b > 8) && (s > 8)) {

        /* Find the maximum coefficient for the block (excluding DC) */
        double max = 0;
        for (int k = 0; k < b; k++ ) {
            for (int l = 0; l < b; l++ ) {
                if ((k==0) && (l==0)) {

                }
                else {
                    if (out[k*b+l] > max) {
                        max = out[k*b+l];
                    }
                }
            }
        }

        /* Divide all coefficients by the maximum */
        for (int k = 0; k < b; k++ ) {
            for (int l = 0; l < b; l++ ) {
                out[k*b+l] = out[k*b+l]/max;
            }

        }

    }


    /* Otherwise, perform quantization */
    else {

        /* Initialize the quantization matrix values - taken from Wikipedia page http://en.wikipedia.org/wiki/Quantization_%28image_processing%29 */
        double q[8][8];
        q[0][0] = 16;
        q[0][1] = 11;
        q[0][2] = 10;
        q[0][3] = 16;
        q[0][4] = 24;
        q[0][5] = 40;
        q[0][6] = 51;
        q[0][7] = 61;
        q[1][0] = 12;
        q[1][1] = 12;
        q[1][2] = 14;
        q[1][3] = 19;
        q[1][4] = 26;
        q[1][5] = 58;
        q[1][6] = 60;
        q[1][7] = 55;
        q[2][0] = 14;
        q[2][1] = 13;
        q[2][2] = 16;
        q[2][3] = 24;
        q[2][4] = 40;
        q[2][5] = 57;
        q[2][6] = 69;
        q[2][7] = 56;
        q[3][0] = 14;
        q[3][1] = 17;
        q[3][2] = 22;
        q[3][3] = 29;
        q[3][4] = 51;
        q[3][5] = 87;
        q[3][6] = 80;
        q[3][7] = 62;
        q[4][0] = 18;
        q[4][1] = 22;
        q[4][2] = 37;
        q[4][3] = 56;
        q[4][4] = 68;
        q[4][5] = 109;
        q[4][6] = 103;
        q[4][7] = 77;
        q[5][0] = 24;
        q[5][1] = 35;
        q[5][2] = 55;
        q[5][3] = 64;
        q[5][4] = 81;
        q[5][5] = 104;
        q[5][6] = 113;
        q[5][7] = 92;
        q[6][0] = 49;
        q[6][1] = 64;
        q[6][2] = 78;
        q[6][3] = 87;
        q[6][4] = 103;
        q[6][5] = 121;
        q[6][6] = 120;
        q[6][7] = 101;
        q[7][0] = 72;
        q[7][1] = 92;
        q[7][2] = 95;
        q[7][3] = 98;
        q[7][4] = 112;
        q[7][5] = 100;
        q[7][6] = 103;
        q[7][7] = 99;

        /* Perform quantization */
        for (int k = 0; k < b; k++ ) {
            for (int l = 0; l < b; l++) {
                out[k*b+l] = out[k*b+l]/q[k][l];
            }
        }

    }
}

/* Reorders the coefficients in a zig-zag order.
 * Code taken from http://refactormycode.com/codes/451-zig-zag-ordering-of-array - LICENSE unclear so will rewrite
 */
std::vector<double> DCT::zigzag(int bh, int bw, int s) {

    /* Vector for storing zigzag coefficients */
    std::vector<double> zigzag;

    /* Temporary arrays */
    double temp[bh*bw];
    double temp2[bh*bw];

    /* Convert 2D input array to 1D array */
    int p = 0;
    for (int i = 0; i < bh; i++) {
        for (int j =  0; j < bw; j++) {
            temp[p] = out[i*bw+j];
            p++;
        }
    }

    /* The maximym dimensions are the block size */
    int xmax = bh;
    int ymax = bw;

    int x = 1;
    int y = 1;

    /* Loop through the array */
    for (int t = 0; t < (bh*bw); t++) {
        /* Set the value for the temporary vector */
        temp2[t] = temp[(y-1)*xmax+(x-1)];
        /* Even zigzag stripes */
        if ((x + y) % 2 == 0) {

            if (x < xmax) {
                x++;
            } else {
                y+= 2;
            }
            if (y > 1) {
                y--;
            }
        }
        /* Odd zigzag stripes */
        else {
            if (y < ymax) {
                y++;
            } else {
                x+= 2;
            }
            if (x > 1) {
                x--;
            }
        }

    }

    /* Add the zigzag coefficients to the vector based on the requested output size */
    for (int i = 0; i < s; i++) {
        zigzag.push_back(temp2[i]);
    }

    /* Return the zigzag array */
    return zigzag;

}
