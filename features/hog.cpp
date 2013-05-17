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

/* Implements the HoG Class */

/* Features based on:
 * N. Dalal and B. Triggs, “Histograms of Oriented Gradients for Human Detection,” Proceedings of the 2005 IEEE Computer Society
 * Conference on Computer Vision and Pattern Recognition (CVPR'05) - Volume 1 - Volume 01, IEEE Computer Society, 2005, pp. 886-893.
 */

/* This class calculates this the Histogram of Ofiented Gradients feature set */

#include "hog.h"
#include <iostream>

/* Constructor
 * Creates a new image so that the original is not overwritten
 */
HoG::HoG ( Magick::Image * i ) {
    image = new Magick::Image();
    image = i;
}

/* Destructor */
HoG::~HoG() {

}

/* Gets the Histogram of Oriented Gradients.
 * @grid the size of the grid for normalisation
 * @cellheight the height of the cells
 * @cellwidth the width of the cells
 * @channels - the number of channels to appear in the histogram
 * @sign - the sign of the orients
 */
std::vector<double> HoG::getHistogram ( int grid, int cellheight, int cellwidth, int channels, bool sign ) {

    /* Create the feature vector */
    std::vector<double> f;

    /* Un-normalised features are stored in a vector of vectors */
    std::vector< std::vector<double> > h;

    /* Create a copy of the image so as not to overwrite the original */
    Magick::Image * copy;
    copy = new Magick::Image ( *image );

    /* Create the historgram with the correct amount of channels */
    std::vector<double> hgram ( channels );

    /* Loop through each of the cells */
    if ( cellheight != 0 ) {
        for ( unsigned int i = 1; i < image->rows()-1; i+=cellheight ) {
            for ( unsigned int j = 1; j < image->columns()-1; j+=cellwidth ) {

                /* Initialise all histogran channels for the current cell */
                for ( int x = 0; x < channels; x++ ) {
                    hgram.at ( x ) = 0.0;
                }

                /* Loop through cell */
                for ( unsigned int k = i; k < i+cellheight; k++ ) {
                    for ( unsigned int l = j; l < j+cellwidth; l++ ) {

                        /* Calculate the values of the pixels in the [-1;0;1] sobel operators */
                        int x1;
                        int x2;
                        int y1;
                        int y2;

                        /* >=0.5 = foreground, <=0.5 = background */
                        if ( Magick::ColorGray ( image->pixelColor ( l-1,k ) ).shade() >= 0.5 ) {
                            x1 = 1;
                        } else {
                            x1 = 0;
                        }
                        if ( Magick::ColorGray ( image->pixelColor ( l+1,k ) ).shade() >= 0.5 ) {
                            x2 = 1.0;
                        } else {
                            x2 = 0.0;
                        }
                        if ( Magick::ColorGray ( image->pixelColor ( l,k-1 ) ).shade() >= 0.5 ) {
                            y1 = 1.0;
                        } else {
                            y1 = 0.0;
                        }
                        if ( Magick::ColorGray ( image->pixelColor ( l,k+1 ) ).shade() >= 0.5 ) {
                            y2 = 1.0;
                        } else {
                            y2 = 0.0;
                        }

                        /* Calculate the derivatives */
                        int dx = ( ( -1.0*x1 )
                                   + ( 1.0*x2 ) );
                        int dy = ( ( -1.0*y1 )
                                   + ( 1.0*y2 ) );

                        /* Calculate the magnitude of the pixel */
                        double mag = abs ( sqrt ( ( pow ( dx,2.0 ) ) + ( pow ( dy,2.0 ) ) ) );
                        /* Calculate the orientation of the pixel */
                        double orientation = atan2 ( dy,dx ) *180/PI;

                        /* This is where the historgrams are calculated. For each cell, there is a histogram of magnitudes for each channel */

                        /* For unsigned gradients, transform gradients to [0;180] range */
                        if ( sign == false ) {
                            if ( ( int ) orientation < 0 ) {
                                orientation = orientation+180.0;
                            }
                            /* Add the gradient to the correct channel */
                            for ( int z = 0; z < channels; z++ ) {
                                if ( ( int ) orientation <= ( ( 180/channels ) * ( z+1 ) ) ) {
                                    hgram.at ( z ) += mag;
                                    break;
                                }
                            }
                        }
                        /* For signed gradients, transform to [0; 360] range */
                        else {
                            if ( ( int ) orientation < 0 ) {
                                orientation = orientation+360;
                            }
                            /* Add the gradient to the correct channel */
                            for ( int z = 0; z < channels; z++ ) {
                                if ( ( int ) orientation <= ( ( 360/channels ) * ( z+1 ) ) ) {
                                    hgram.at ( z ) += mag;
                                    break;
                                }
                            }
                        }

                    }
                } //Complete HoG for cell

                /* Push back vector of features for the cell */
                h.push_back ( hgram );

            }
        }
    }//Complete HoG for all cells

    /* Normalise feature vector - only if grid size and cell size allow for it */
    std::vector< std::vector<double> > nfv;
    if ((image->rows()/cellheight)%grid == 0){
      nfv = normaliseFeatures ( grid, h );
    }else{
      nfv = normaliseFeatures ( 0, h );
    }

    /* Linearise the feature vector */
    for ( unsigned int i = 0; i < nfv.size(); i++ ) {
        for ( int p = 0; p < 9; p++ ) {
            f.push_back ( nfv.at ( i ).at ( p ) );
        }
    }

    /* Return the feature vector */
    return f;

}

/* Normalises the feature vector by performing block normalisation.
 * Does not do overlapping blocks - perhaps later.
 * @g - the grid size, ie. number of cells in grid
 * @in - the vector of feature vectors
 */

std::vector< std::vector<double> > HoG::normaliseFeatures ( int g, std::vector< std::vector<double> > in ) {

    /* Can set g to 0 to prevent normalisation */
    if ( g != 0 ) {
        /* Loop through the feature vector as if it is a square array */
        for ( int i = 0; i < sqrt ( in.size() ); i+=g ) {
            for ( int j = 0; j < sqrt ( in.size() ); j+=g ) {

                /*Variable for the norm */
                double v_norm = 0;

                /* Loop through the cells in the grid */
                for ( int k = i; k < i+g; k++ ) {
                    for ( int l = j; l < j+g; l++ ) {
                        /* Loop through the magnitudes in each histogram channel */
                        for ( int p = 0; p < 9; p++ ) {
                            /* Sum the square magnitudes for each channel */
                            v_norm = v_norm + pow ( in.at ( k*sqrt ( in.size() ) +l ).at ( p ), 2.0 );
                        }
                    }
                }

                /* Get the square root of the norm */
                v_norm = sqrt ( v_norm );
                /* Get the normalising factor */
                double f = sqrt ( pow ( v_norm, 2.0 ) + pow ( pow ( 10.0, -6 ), 2.0 ) );

                /* Loop through all features in the feature vector and normalise */
                for ( int k = i; k < i+g; k++ ) {
                    for ( int l = j; l < j+g; l++ ) {
                        for ( int p = 0; p < 9; p++ ) {
                            in.at ( k*sqrt ( in.size() ) +l ).at ( p ) = in.at ( k*sqrt ( in.size() ) +l ).at ( p ) /f;
                        }

                    }
                }

            }
        }
    }

    /* Return the normalised feature vector */
    return in;

}
