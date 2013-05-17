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

/* This program calculates the statistical moments to be used as features. The Feature class calculates the Hu
 * features based on the moments produced by this class
 */

#ifndef _moments_h_
#define _moments_h_

#include <Magick++.h>
#include <math.h>

class Moments {
public:

    /* Constructor */
    Moments ( Magick::Image * i );
    ~Moments ();

    /*Gets the central moment pq */
    double getCM ( int p, int q );
    /* Gets the normalised central moment pq */
    double getNCM ( int p, int q );
    /* Gets the center of gravity about each axis */
    double getXBar();
    double getYBar();

private:

    Magick::Image * image;

    /* Needed to calculate intermediate values */
    void calculateIntermediaries();

    /* Variables used in feature calculation */
    double m00;
    double m10;
    double m01;
    double x_c;
    double y_c;
    double cmom;
    double ncmom;

};

#endif // _moments_h_
