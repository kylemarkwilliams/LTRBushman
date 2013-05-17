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
 *  Jin Chen, Huaigu Cao, Rohit Prasad, Anurag Bhardwaj, and Prem Natarajan. Gabor features for offline
 * arabic handwriting recognition. In DAS ’10: Proceedings of the 9th IAPR International Workshop on
 * Document Analysis Systems, pages 53–58, New York, NY, USA, 2010. ACM.
 *
 *  Soﬁene Haboubi, Samia Maddouri, Noureddine Ellouze, and Hailkal El-Abed. Invariant primitives for
 * handwritten arabic script: A contrastive study of four feature sets. In ICDAR ’09: Proceedings of the
 * 2009 10th International Conference on Document Analysis and Recognition, pages 691–697,
 * Washington, DC, USA, 2009. IEEE Computer Society.
 *
 * Cheng-Lin Liu, Masashi Koga, and Hiromichi Fujisawa. Gabor feature extraction for character recognition:
 * Comparison with gradient feature. In ICDAR ’05: Proceedings of the Eighth International Conference on
 * Document Analysis and Recognition, pages 121–125, Washington, DC, USA, 2005. IEEE Computer Society.
 *
 */

/* This class implements the Histogram of Ofiented Gradients feature */

/* Makes use of Octave for feature calculation */


#ifndef _gabor_h_
#define _gabor_h_

#include <Magick++.h>
#include <vector>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <octave/parse.h>

#define PI 3.14159265

class Gabor
{

public:

    /* Constructor */
    Gabor(std::string fname);
    /* Destructor */
    ~Gabor();
    /* Gets the features */
    std::vector<double> getGabor(double sxt, double syt, std::vector<double> ft, std::vector<double> thetat, int bh, int bw);

private:

    Magick::Image * image;
    std::string filename;
    double sx;
    double sy;
    std::vector<double> f;
    std::vector<double> theta;
    int bheight;
    int bwidth;

};

#endif // _gabor_h_
