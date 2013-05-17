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

#ifndef _usbitmaps_h_
#define _usbitmaps_h_

#include <Magick++.h>
#include <math.h>
#include <vector>

class USBitmaps {
    public:

        /* Constructor */
        USBitmaps ( Magick::Image * i );
        ~USBitmaps ();

        /* Calculates the features */
        std::vector<double> getUSBitmaps ( int h, int w );

    private:

        Magick::Image * image;
        int regions;

};

#endif // _usbitmaps_h_ 
