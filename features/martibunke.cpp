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

/*Features based on:
 *
 * U.-V. Marti. Off-line recognition of handwritten texts. PhD thesis, University of Bern, Switzerland, 2000.
 *
 * U.-V. Marti and H. Bunke. Using a statistical language model to improve the performance of an HMM-based
 * cursive handwriting recognition systems. In Hidden Markov models: applications in computer vision,
 * pages 65–90. World Scientiﬁc Publishing Co., Inc., 2002.
 */

/* This class is used to calculate the Marti & Bunke Feature set */

/* Implements the MartiBunke class */

#include "martibunke.h"
#include <iostream>

/* Constructor */
MartiBunke::MartiBunke ( Magick::Image* i ) {
    image = new Magick::Image();
    image = i;
}

/* Destructor */
MartiBunke::~MartiBunke() {

}

/* Method to calculate the Marti & Bunke features */
std::vector<double> MartiBunke::getMartiBunke() {

    /* Feature vector */
    std::vector<double> f;

    /* Feature variables */
    f1 = 0;
    f2 = 0;
    f3 = 0;
    f4 = 0;
    f5 = 0;
    f6 = 0;
    f7 = 0;
    f8 = 0;
    f9 = 0;
    int offset = 0;

    /* Loops through each column of the window since features are extracted using a sliding window */
    for ( unsigned int i = 0; i < image->columns()-1; i++ ) {

        /* Create a copy of the image to store the current column */
        column = new Magick::Image ( *image );

        /* Crop the image so that it is one column wide */
        column->page(Magick::Geometry(1,column->rows(),0,0));
        column->crop(Magick::Geometry(1,column->rows(),offset,0));

        /* Get all of the features */
        f1 = getF1();
        f2 = getF2();
        f3 = getF3();
        f4 = getF4();
        f5 = getF5();
        f6 = getF6((int)f4, i);
        f7 = getF7((int)f5, i);
        f8 = getF8();
        f9 = getF9((int)f4, (int)f5);

        /* Add the features to the feature vector */
        f.push_back(f1);
        f.push_back(f2);
        f.push_back(f3);
        f.push_back(f4);
        f.push_back(f5);
        f.push_back(f6);
        f.push_back(f7);
        f.push_back(f8);
        f.push_back(f9);

        /* Increase the offset for the next column segmentation */
        offset++;


    }

    /* Delete the column pointer and return the feature column */
    delete column;
    return f;

}

/* Calculates the F1 feature.
 * F1 feature is the weight of the window
 */
double MartiBunke::getF1() {

    double f1_t = 0;

    /* Loop through the column image and sum of the pixels */
    for (unsigned int i = 1; i < column->rows(); i++) {
        f1_t = f1_t + Magick::ColorGray(column->pixelColor(0,i)).shade();
    }

    /* Get the weight by dividing by total pixels in column window */
    f1_t = f1_t/column->rows();

    /* Return the feature */
    return f1_t;

}

/* Calculates the F2 feature.
 * F2 feature is the center of gravity of the window
 */
double MartiBunke::getF2() {

    double f2_t = 0;

    /* Loop through the column image and sum of the product of the pixels and the current row */
    for (unsigned int i = 1; i < column->rows(); i++) {
        f2_t = f2_t + (i*Magick::ColorGray(column->pixelColor(0,i)).shade());
    }

    /* Divide by the total pixels in the column window */
    f2_t = f2_t/column->rows();

    /* Return the feature */
    return f2_t;

}

/* Calculates the F3 feature.
 * F3 feature is the second order moment of the window
 */
double MartiBunke::getF3() {

    double f3_t = 0;

    /* Loop through the column image and sum the product of the pixels and squared row */
    for (unsigned int i = 1; i < column->rows(); i++) {
        f3_t = f3_t + (pow(i,2.0)*Magick::ColorGray(column->pixelColor(0,i)).shade());
    }

    /* Divide by the squared total pixels in the window */
    f3_t = f3_t/(pow(column->rows(),2.0));

    /* Return the feature */
    return f3_t;

}

/* Calculates the F4 feature.
 * F4 feature is the upper position of the contour of the window
 */
double MartiBunke::getF4() {

    /* Assume the upper contour is at the bottom */
    double f4_t = column->rows();
    /* Loop through the pixels and if a foreground pixel is found, change the upper contour and break */
    for (unsigned int i = 1; i < column->rows(); i++) {
        if (Magick::ColorGray(column->pixelColor(0,i)).shade() >= 0.5) {
            f4_t = i;
            break;
        }
    }

    /* Return the upper contour */
    return f4_t;

}

/* Calculates the F5 feature.
 * F5 feature is the lower position of the contour of the window
 */
double MartiBunke::getF5() {

    /* Assume the upper contour is at the bottom */
    double f5_t = 0;
    /* Loop through the pixels and if a foreground pixel is found, change the lower contour */
    for (unsigned int i = 1; i < column->rows(); i++) {
        if (Magick::ColorGray(column->pixelColor(0,i)).shade() >= 0.5) {
            f5_t = i;
        }
    }

    /* Return the lower contour */
    return f5_t;

}

/* Calculates the F6 feature.
 * F6 feature is the gradient of the upper contour of the window
 */
double MartiBunke::getF6(int f4f, int c) {

    /* Calculate the gradient based on: d/dx = p(i+1,j)-p(i,j) */
    double f6_t = Magick::ColorGray(image->pixelColor(c+1,f4f)).shade() - Magick::ColorGray(image->pixelColor(c,f4f)).shade();

    /* Return the gradient */
    return f6_t;

}

/* Calculates the F7 feature.
 * F7 feature is the gradient of the lower contour of the window
 */
double MartiBunke::getF7(int f5f, int c) {

    /* Calculate the gradient based on: d/dx = p(i+1,j)-p(i,j) */
    double f7_t = Magick::ColorGray(image->pixelColor(c+1,f5f)).shade() - Magick::ColorGray(image->pixelColor(c,f5f)).shade();

    /* Return the gradient */
    return f7_t;

}

/* Calculates the F8 feature.
 * F8 feature is the number of background-foreground transitions
 */
double MartiBunke::getF8() {

    double f8_t = 0;
    /* Assume starting with a background pixel */
    double last = 0.0;
    /* Loop through the column and if a chnage in pixel colour occurs,
     * increase the count and update the last pixel colour seen
     */
    for (unsigned int i = 1; i < column->rows(); i++) {
        if (Magick::ColorGray(column->pixelColor(0,i)).shade() != last) {
            f8_t++;
            last = Magick::ColorGray(column->pixelColor(0,i)).shade();
        }
    }

    /* Return the number of foreground-background transitions */
    return f8_t;

}

/* Calculates the F9 feature.
 * F9 feature is the number of foreground pixels in between the upper and lower contour divided
 * by the height of the contour.
 */
double MartiBunke::getF9(int f4f, int f5f) {

    double f9_t = 0;
    /* If the upper and lower contour are the same, assume no pixels between */
    if (f4f == f5f) {
        f9_t = 0;
    }
    else {
        /* Loop through the pixels in the upper and lower contour and sum the pixels */
        for (int i = f4f; i < f5f; i++) {
            f9_t = f9_t + Magick::ColorGray(column->pixelColor(0,i)).shade();
        }
        /* Divide pixel count by height of contour */
        f9_t = f9_t/(f5f-f4f);
    }

    /* Return the feature */
    return f9_t;

}
