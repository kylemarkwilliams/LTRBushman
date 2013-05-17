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

#ifndef _holistic_h_
#define _holistic_h_

#include <Magick++.h>
#include <math.h>
#include <vector>

class Holistic {
public:

    /* Constructor */
    Holistic ( Magick::Image * i );
    ~Holistic ();
    /* Gets the different features */
    std::vector<int> getProjectionProfile ( int start, int end );
    std::vector<int> getProfile ( bool bottom );
    std::vector<int> getTransitions();

private:

    Magick::Image * image;

};

#endif // _holistic_h_ 
