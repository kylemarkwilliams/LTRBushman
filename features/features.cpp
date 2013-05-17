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

/* Implements to feature class */

/* This is the feature class. It is the general class which provides access to all of the features implemented in this system.
 * To add a new feature, create the feature class, create a function to call it in this class, and then call that function
 * from the main class
 */

#include "features.h"

/* Constructor
 * Makes a new image based on the pointer so as to not modify the original image.
 *
 * @i pointer to image for which features should be extracted
 * @fname the path of the image for which features should be extracted - this used for cases where external programs extract
 * features and need access to the original image.
 */

Features::Features ( Magick::Image * i, std::string fname ) {
    image = new Magick::Image();
    image = i;
    filename = fname;
}

/* Constructor
 *
 * @fname the path of the image for which features should be extracted - this used for cases where external programs extract
 * features and need access to the original image.
 */

Features::Features (std::string fname ) {
    filename = fname;
}

/* Destructor deletes the pointer - should only be done at the end of the program */
Features::~Features() {
}

/* Returns the holistic features set.
 * I don't think this will make it into the final project
 */

std::vector<int> Features::getHolistic() {

    std::vector<int> f; //Create a vector to hold the features

    /* Because more than one set of features might be extracted from the image
     * a copy is made to prevent any changes affecting other features
     */

    Magick::Image * copy = new Magick::Image ( *image );
    Holistic holistic ( copy );

    /* Gets the holistic features. See the holistic.cpp for details */
    std::vector<int> pp = holistic.getProjectionProfile ( 0, image->rows() ); //Whole height of image
    std::vector<int> upp = holistic.getProjectionProfile ( 0, image->rows() /2 ); // Top half
    std::vector<int> lpp = holistic.getProjectionProfile ( image->rows() /2, image->rows() ); //Bottom half
    std::vector<int> up = holistic.getProfile ( false ); //Upper profile
    std::vector<int> lp = holistic.getProfile ( true ); //Lower profile
    std::vector<int> t = holistic.getTransitions();

    /* Adds the features to the vector */
    for ( unsigned int i = 0; i < pp.size(); i++ ) {
        f.push_back ( pp.at ( i ) );
        f.push_back ( upp.at ( i ) );
        f.push_back ( lpp.at ( i ) );
        f.push_back ( up.at ( i ) );
        f.push_back ( lp.at ( i ) );
        f.push_back ( t.at ( i ) );
    }

    /* Delete the copy image and return the vector */
    delete copy;
    return f;

}

/* Returns the Histogram of Oriented Gradients (HoG0 feature set.
 * @g the grid size for normalisation
 * @ch the cell height
 * @cw the cell width
 * @c - number of histogram channels
 * @si - if the histograms are signed or not
 */
std::vector<double> Features::getHoG ( int g, int ch, int cw, int c, bool si ) {

    /* Create the copy image to prevent changes to original image */
    Magick::Image * copy = new Magick::Image ( *image );
    HoG hog ( copy );
    /* Get the features */
    std::vector<double> f = hog.getHistogram ( g,ch,cw,c,si );
    /* Delete the copy and return the feature vector */
    delete copy;
    return f;

}

/* Gets the undersampled bitmaps feature set.
 * @r - the number of regions to divide each dimension into
 */
std::vector<double> Features::getUSBitmaps ( int h, int w ) {

    /* Create the copy */
    Magick::Image * copy = new Magick::Image ( *image );
    USBitmaps usb ( copy );
    /* Get the feature vector */
    std::vector<double> f = usb.getUSBitmaps ( h, w );
    /* Delete the copy and return the feature vector */
    delete copy;
    return f;

}

/* Calculates the Gabor filter features.
 *
 * @fname the path to the file which is parsed to the Octave program
 * @sx variance along x-axis (shape of Gaussian)
 * @sy variance along y-axis (shape of Gaussian)
 * @f vector of frequencies
 * @theta vector of orientations
 * @bh block height
 * @bw block width
 */
std::vector<double> Features::getGabor(std::string fname, double sx, double sy, std::vector<double> f, std::vector<double> theta, int bh, int bw) {

    /* Copy image is needed on disk so that the Octave function can find the image */
    Magick::Image * copy = new Magick::Image ( *image );
    copy->write("gabor.png");

    /* Create the Gabor object */
    Gabor gabor ("gabor.png");

    /* Get the feature vector */
    std::vector<double> feat = gabor.getGabor(sx, sy, f, theta, bh, bw);

    /* Return the feature vector and delete the copy */
    delete copy;
    return feat;

}

/* Gets the DCT feature set.
 * @bh the block height
 * @bw the block width
 * @s the number of coefficients to be output
 * @q quantize the coefficients
 */
std::vector<double> Features::getDCT(int bh, int bw, int s, bool q) {

    /* Create the copy */
    Magick::Image * copy = new Magick::Image ( *image );
    DCT dct ( copy );
    /* Get the DCT feature set */
    std::vector<double> f = dct.getDCT(bh, bw, s, q);
    /* Delete the copy and return the feature set */
    delete copy;
    return f;

}

/* Gets the statistical moments feature set
 * @xybar return x-bar and y-bar as features
 * @m1 return the first moment
 * @m2 return the second moment
 * @m3 return the third moment
 * @m4 return the fourth moment
 * @bh the height of a cell
 * @bw the width of a cell
 * @o overlap
 * */
std::vector<double> Features::getMoments ( bool xybar, bool m1, bool m2, bool m3, bool m4, int bh, int bw, int o ) {

    /* Create the feature vector */
    std::vector<double> f;

    /* Going to loop through the cells in the image based on the cell size and overlap.
     * Image is separated into sub-images here and features are extracted for each sub0image
     */
    for ( unsigned int i = 0; i < image->columns()-o; i+=bw-o ) {
        for ( unsigned int j = 0; j < image->rows()-o; j+=bh-o ) {

            Magick::Image * copy = new Magick::Image ( *image );

            /* Crop whole image into individual cell */
            int offset_x = i;
            int offset_y = j;
            copy->page ( Magick::Geometry ( bw,bh,0,0 ) );
            copy->crop ( Magick::Geometry ( bw,bh,offset_x,offset_y ) );

            /* Create the Moments object for extrating features */
            Moments m ( copy );
            double mo;

            /* Add x-bar and y-bar if wanted */
            if ( xybar == true ) {
                f.push_back ( m.getXBar() );
                f.push_back ( m.getYBar() );
            }
            
            double ncm20 = m.getNCM(2,0);
            double ncm02 = m.getNCM(0,2);
            double ncm11 = m.getNCM(1,1);
            double ncm30 = m.getNCM(3,0);
            double ncm12 = m.getNCM(1,2);
            double ncm21 = m.getNCM(2,1);
            double ncm03 = m.getNCM(0,3);
            
            

            /* If the first moment was wanted */
            if ( m1==true ) {
                mo = ncm20 + ncm02;
                f.push_back ( mo );
            }

            /* If the second moment was wanted */
            if ( m2==true ) {
                mo = pow ( ( ncm20-ncm02),2 ) + 4* ( pow ( ncm11,2 ) );
                f.push_back ( mo );
            }

            /* If the third moment was wanted */
            if ( m3==true ) {
                mo = pow ( ( ncm30- ( 3* ( ncm12 ) ) ),2 ) + pow ( ( ( 3* ( ncm21 ) )-ncm03 ),2 );
                f.push_back ( mo );
            }

            /* If the fourth moment was wanted */
            if ( m4==true ) {
                mo = pow ( ( ncm30 +ncm12 ),2 ) + pow ( ( ncm21-ncm03 ),2 );
                f.push_back ( mo );
            }

            /* Delete the cell image */
            delete copy;

        }

    }

    /* Return the feature vector */
    return f;

}

/* Gets the Marti & Bunke feature set */
std::vector< double > Features::getMartiBunke() {

    /* Create a copy so as not to write over the original */
    Magick::Image * copy = new Magick::Image ( *image );
    MartiBunke mb ( copy );

    /* Get the features */
    std::vector<double> f = mb.getMartiBunke();

    /* Delete the copy and return the feature vector */
    delete copy;
    return f;

}
