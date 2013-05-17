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


#ifndef _dct_h_
#define _dct_h_

#include <Magick++.h>
#include <math.h>
#include <vector>
#include <fftw3.h>

class DCT {
public:

    /* Constructor */
    DCT ( Magick::Image * i );
    /* Destructor */
    ~DCT ();

    /* Calculates the DCT coefficients for use as features */
    std::vector<double> getDCT(int bh, int bw, int s, bool q);

private:

    double *in;
    double *out;
    Magick::Image * image;
    /* Quantizes the coefficients */
    void quantize(int b, int s);
    /* Gets the zig-zag order of the coefficients */
    std::vector<double> zigzag(int bh, int bw, int s);

};

#endif // _dct_h_ 

