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
 * T.M. Rath and R. Manmatha, “Features for Word Spotting in Historical Manuscripts,”
 * Proceedings of the Seventh International Conference on Document Analysis and Recognition - Volume 1, IEEE Computer Society, 2003, p. 218.
 */

#include "holistic.h"

/* Constructor
 * Creates a new image so that the original is not overwritten
 */
Holistic::Holistic ( Magick::Image * i ) {
    image = new Magick::Image();
    image = i;
}

/* Destructor */
Holistic::~Holistic() {

}

/* Gets the projection profile features
 * @start the rows to start at
 * @end the row to end at
 */
std::vector<int> Holistic::getProjectionProfile ( int start, int end ) {

    /* Vector for storing projection profiles */
    std::vector<int> pp;
    /* Loop through all of the columns and get the projection profile */
    for ( unsigned int i = 0; i < image->columns(); i++ ) {
        int col = 0;
        for ( int j = start; j < end; j++ ) {
            col += Magick::ColorGray ( image->pixelColor ( i,j ) ).shade();
        }
        pp.push_back ( col );
    }
    /* Return the projection profile feature */
    return pp;
}

/* Gets the profile of the image
 * @bottom whether we're starting at the bottom or not for upper or lower profile
 */

std::vector<int> Holistic::getProfile ( bool bottom ) {
    std::vector<int> p;
    /* Loop through all the columsn of the image */
    for ( unsigned int i = 0; i < image->columns(); i++ ) {
        int col = 0;
        /* The upper profile */
        if ( bottom == false ) {
            for ( unsigned int j = 0; j < image->rows(); j++ ) {
                if ( Magick::ColorGray ( image->pixelColor ( i,j ) ).shade() >= 0.5 ) {
                    col = j;
                    break;
                }
            }
        }
        /* The lower profile */
        else if ( bottom == true ) {
            for ( unsigned int j = image->rows(); j > 0; j-- ) {
                if ( Magick::ColorGray ( image->pixelColor ( i,j ) ).shade() >= 0.5 ) {
                    col = j;
                    break;
                }
            }
        }
        // Add column feature to vector */
        p.push_back ( col );
    }
    /* Return feature vector */
    return p;
}

/* Gets the number of foreground-background transitions in each column */
std::vector<int> Holistic::getTransitions() {
    std::vector<int> t;
    /* Loop through the columns */
    for ( unsigned int i = 0; i < image->columns(); i++ ) {
        double last = 0;
        int transitions = 0;
        /* For each row count the transitions */
        for ( unsigned int j = 0; j < image->rows(); j++ ) {
            double cur = Magick::ColorGray ( image->pixelColor ( i,j ) ).shade();
            if ( cur != last ) {
                transitions++;
            }
            last = cur;
        }
        /* Add number of transitions for each column to feature vector */
        t.push_back ( transitions );
    }
    /* Return the feature vector */
    return t;
}
