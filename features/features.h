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

/* This is the feature class. It is the general class which provides access to all of the features implemented in this system.
 * To add a new feature, create the feature class, create a function to call it in this class, and then call that function
 * from the main class
 */

#ifndef _features_h_
#define _features_h_

#include <Magick++.h>
#include <vector>
#include <math.h>
#include "moments.h"
#include "hog.h"
#include "usbitmaps.h"
#include "holistic.h"
#include "dct.h"
#include "martibunke.h"
#include "gabor.h"

class Features {

public:

    /* Constructor takes a pointer to an image as input */
    Features ( Magick::Image * i, std::string fname );
    Features (std::string fname);
    Features();
    ~Features();

    /* Methods to get all features and return a vector for each feature */
    std::vector<double> getMoments ( bool xybar, bool m1, bool m2, bool m3, bool m4, int bh, int bw, int o );
    std::vector<double> getHoG ( int g, int ch, int cw, int c, bool si );
    std::vector<double> getUSBitmaps ( int h, int w );
    std::vector<int> getHolistic();
    std::vector<double> getGabor(std::string fname, double sx, double sy, std::vector<double> f, std::vector<double> theta, int bh, int bw);
    std::vector<double> getDCT(int bh, int bw, int s, bool q);
    std::vector<double> getMartiBunke();


private:

    Magick::Image * image;
    std::string filename;

};

#endif // _features_h_ 
