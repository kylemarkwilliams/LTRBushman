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

#ifndef _martibunke_h_
#define _martibunke_h_

#include <Magick++.h>
#include <vector>
#include <math.h>

class MartiBunke {
public:

    /* Constructor */
    MartiBunke ( Magick::Image * i );
    /* Destructor */
    ~MartiBunke ();

    /* Calculates the features */
    std::vector<double> getMartiBunke();

private:

    Magick::Image * image;
    Magick::Image * column;

    /* Private methods for calculating features */
    double getF1();
    double getF2();
    double getF3();
    double getF4();
    double getF5();
    double getF6(int f4f, int c);
    double getF7(int f5f, int c);
    double getF8();
    double getF9(int f4f, int f5f);

    /* Variables to store features */
    double f1;
    double f2;
    double f3;
    double f4;
    double f5;
    double f6;
    double f7;
    double f8;
    double f9;

};

#endif // _martibunke_h_
