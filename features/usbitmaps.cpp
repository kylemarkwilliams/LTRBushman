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
* Oliveira, Mello, Silva, and Alves, “Optical Digit Recognition for Images of Handwritten Historical Documents,”  Ribeirao Preto, Brazil: , pp. 166-171.
*/

/*
 * The undersampled bitmap feature divides an image into regions and then calculated the normalised number of pixels in each region
 */

#include "usbitmaps.h"
#include <iostream>
/* Constructor
 * Create a copy of the image so as not to modify the original.
 */
USBitmaps::USBitmaps ( Magick::Image * i ) {
    image = new Magick::Image();
    image = i;
}

/* Destructor */
USBitmaps::~USBitmaps() {

}

/* Calculates the features
 * @r the number of regions to divide the image into
 */
std::vector<double> USBitmaps::getUSBitmaps ( int h, int w ) {

    std::vector<double> features;

    int pcount = 0;
    double pmax = 0;
    /*Loop through the image, going through one region at a time */
    for ( unsigned int i = image->columns() /w; i <= image->columns(); i+= (image->columns()/w >= 1 ? image->columns()/w : 1)) {

        for ( unsigned int j = image->rows() /h; j <= image->rows(); j+=image->rows() /h ) {

            /*For the current region count the number of foreground pixels */
            for ( unsigned int k = i- ( image->columns() /w ); k < i; k++ ) {
                for ( unsigned int l = j- ( image->rows() /h ); l < j; l++ ) {
                    if ( Magick::ColorGray ( image->pixelColor ( k,l ) ).shade() >= 0.5 ) {
                        pcount++;
                    }
                }
            }
            /* Add the number of foreground pixels to the feature vector and check for max */
            features.push_back ( pcount );
            if ( pcount > pmax ) {
                pmax = pcount;
            }
            pcount = 0;
        }
    }

    /* Normalise all features by dividing by the max */
    for ( unsigned int i = 0; i < features.size(); i++ ) {
        if (pmax > 0){
          features.at ( i ) = features.at ( i ) /pmax;
        }
        else {
          features.at ( i ) = features.at ( i ) /1;
        }
    }

    /* Return the feature vector */
    return features;

}
