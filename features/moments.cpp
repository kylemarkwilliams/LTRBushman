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
 * W. Clocksin and P. Fernando, “Towards Automatic Transcription of Syriac Handwriting,” Image Analysis and Processing,
 * International Conference on,  Los Alamitos, CA, USA: IEEE Computer Society, 2003, p. 664.
 */

/* Implements the Moments class */

/* This program calculates the statistical moments to be used as features. The Feature class calculates the Hu
 * features based on the moments produced by this class
 */

#include "moments.h"

/* Constructor */
Moments::Moments ( Magick::Image * i ) {
    image = new Magick::Image();
    image = i;

}

/* Destructor */
Moments::~Moments() {

}

/* Gets the center of mass about the x-axis */
double Moments::getXBar() {

    /* Calculate the intermediate moments */
    calculateIntermediaries();

    /* If there are only white pixels the division by zero will be undefined, so instead set it to 0 */
    if ( m00 == 0.0 ) {
        x_c = 0;
        y_c = 0;
    }
    /* Otherwise calculate x-bar and y-bar */
    else {
        x_c = m10/m00;
        y_c = m01/m00;
    }

    /* Return x-bar */
    return x_c;
}

/* Gets the center of mass about the y-axis */
double Moments::getYBar() {

    /* Calculate the intermediate moments */
    calculateIntermediaries();

    /* If there are only white pixels the division by zero will be undefined, so instead set it to 0 */
    if ( m00 == 0.0 ) {
        x_c = 0;
        y_c = 0;
    }
    /* Otherwise calculate x-bar and y-bar */
    else {
        x_c = m10/m00;
        y_c = m01/m00;
    }

    /* Return y-bar */
    return y_c;
}

/* Calculates the (p+q)th centralised moment normalised with respect to the center of mass.
 * @p - p-th order
 * @q - q-th order
 */

double Moments::getCM ( int p, int q ) {

    /* Calculate the intermediate moments */
    calculateIntermediaries();

    /* If there are only white pixels the division by zero will be undefined, so instead set it to 0 */
    if ( m00 == 0.0 ) {
        x_c = 0;
        y_c = 0;
    }
    /* Otherwise calculate x-bar and y-bar */
    else {
        x_c = m10/m00;
        y_c = m01/m00;
    }

    /* Calculate the (p+q)th central moment */
    for ( unsigned int i = 0; i < image->columns(); i++ ) {
        for ( unsigned int j = 0; j < image->rows(); j++ ) {
            cmom += pow ( ( i-x_c ),p ) *pow ( ( j-y_c ),q ) *Magick::ColorGray ( image->pixelColor ( i,j ) ).shade();
        }
    }

    /* Return the central momeny */
    return cmom;

}

/* Calculates the (p+q)th normalised central moment.
 * @p - p-th order
 * @q - q-th order
 */

double Moments::getNCM ( int p, int q ) {

    /* Initialise the normalised central moment */

    ncmom = 0;

    /* Calculate gamma y */
    double y = ( ( p+q ) /2 ) + 1;

    /* Get centralised moment 0,0 and set to 1 if necessary to prevent division by zero */
    double b = getCM ( 0,0 );
    if ( b == 0.0 ) {
        b = 1.0;
    }

    /* Calculate the normalised central moment and return it */
    ncmom = getCM ( p,q ) / ( pow ( b,y ) );
    return ncmom;

}

/* Calculates the intermediate values which are used in the calculation of moments */
void Moments::calculateIntermediaries() {

    /* Initialise the intermediate moments */
    cmom = 0;
    m00 = 0;
    m10 = 0;
    m01 = 0;

    /* Calculate intermidiate moments */
    for ( unsigned int i = 0; i < image->columns(); i++ ) {
        for ( unsigned int j = 0; j < image->rows(); j++ ) {
            m00 += Magick::ColorGray ( image->pixelColor ( i,j ) ).shade();
            m10 += i*Magick::ColorGray ( image->pixelColor ( i,j ) ).shade();
            m01 += j*Magick::ColorGray ( image->pixelColor ( i,j ) ).shade();
        }
    }
}
