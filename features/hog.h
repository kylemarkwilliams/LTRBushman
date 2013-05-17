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
 * N. Dalal and B. Triggs, “Histograms of Oriented Gradients for Human Detection,” Proceedings of the 2005 IEEE Computer Society
 * Conference on Computer Vision and Pattern Recognition (CVPR'05) - Volume 1 - Volume 01, IEEE Computer Society, 2005, pp. 886-893.
 */

/* This class implements the Histogram of Ofiented Gradients feature */


#ifndef _hog_h_
#define _hog_h_

#define PI 3.14159265

#include <Magick++.h>
#include <math.h>
#include <vector>

class HoG {
public:

    /* Constructor */
    HoG ( Magick::Image * i );
    /* Destructor */
    ~HoG ();
    /* Calculates the features */
    std::vector<double> getHistogram ( int g, int ch, int cw, int c, bool si );


private:

    Magick::Image * image;
    /* Normalises the features if requested */
    std::vector< std::vector<double> > normaliseFeatures ( int g, std::vector< std::vector<double> > in );

};

#endif // _hog_h_
