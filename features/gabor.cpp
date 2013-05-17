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
 *
 *  Jin Chen, Huaigu Cao, Rohit Prasad, Anurag Bhardwaj, and Prem Natarajan. Gabor features for offline
 * arabic handwriting recognition. In DAS ’10: Proceedings of the 9th IAPR International Workshop on
 * Document Analysis Systems, pages 53–58, New York, NY, USA, 2010. ACM.
 *
 *  Soﬁene Haboubi, Samia Maddouri, Noureddine Ellouze, and Hailkal El-Abed. Invariant primitives for
 * handwritten arabic script: A contrastive study of four feature sets. In ICDAR ’09: Proceedings of the
 * 2009 10th International Conference on Document Analysis and Recognition, pages 691–697,
 * Washington, DC, USA, 2009. IEEE Computer Society.
 *
 * Cheng-Lin Liu, Masashi Koga, and Hiromichi Fujisawa. Gabor feature extraction for character recognition:
 * Comparison with gradient feature. In ICDAR ’05: Proceedings of the Eighth International Conference on
 * Document Analysis and Recognition, pages 121–125, Washington, DC, USA, 2005. IEEE Computer Society.
 *
 */

/* Implements the Gabor class */

/* Makes use of Octave for feature calculation */

#include "gabor.h"

/* Default contructor.
 * Takes a string filename as an argument as that is passed to the Octave function which then loads the image.
 *
 * @fname the filename of the image
 */
Gabor::Gabor(std::string fname) {
    filename = fname;
}

/* Destructor */
Gabor::~Gabor() {

}

/* Calculates the Gabor filter features.
 *
 * Gabor filters are created for multiple frequencies and orientations.
 * Process for extracting features after convolution is based on Chen et al (2010).
 *
 * @sxt variance along x-axis (shape of Gaussian)
 * @syt variance along y-axis (shape of Gaussian)
 * @ft vector of frequencies
 * @thetat vector of orientations
 * @bh block height
 * @bw block width
 */
std::vector<double> Gabor::getGabor(double sxt, double syt, std::vector<double> ft, std::vector<double> thetat, int bh, int bw) {

    sx = sxt;
    sy = syt;
    f = ft;
    theta = thetat;
    bheight = bh;
    bwidth = bw;

    /* Create the feature vector */
    std::vector<double> fv;

    /* Initialise octave */
    // Done in source which is executed, otherwise program crashes

    /* Stores the arguments for the octave function */
    octave_value_list functionArguments;

    /* Creates a Gabor filter for each pair of frequencies and orientations and extracts features */
    for (unsigned int i = 0; i < f.size(); i++) {
        for (unsigned int j = 0; j < theta.size(); j++) {

            /* Get the function arguments */
            functionArguments (0) = filename;
            functionArguments (1) = sx;
            functionArguments (2) = sy;
            functionArguments (3) = f.at(i);
            functionArguments (4) = theta.at(j);

            /* Pass the arguments to the Octave function */
            const octave_value_list result = feval ("gaborfilter1", functionArguments, 1);

            /* Get the result from the Gabor filter */
            Matrix gabor = result (1).matrix_value ();

            /* Convert filtered image to features as follows:
             * 1. Find the mean response of the Gabor filter, M.
             * 2. Calculate how many responses in the entire image exceed the mean response, N.
             * 3. Partition the image into blocks.
             * 4. Calculate how many responses in each block exceed the mean response for the whole image, Nb.
             * 5. Feature is given by (Nb/N) and added to the feature vector.
             */

            /* Calculate the mean */
            double mean = 0;
            for (int k = 0; k < gabor.rows(); k++) {
                for (int l = 0; l < gabor.columns(); l++) {
                    mean = mean+gabor(k,l);
                }
            }
            mean = mean/(gabor.rows()*gabor.columns());

            /* Count number of responses which exceed mean */
            double N = 0;
            for (int k = 0; k < gabor.rows(); k++) {
                for (int l = 0; l < gabor.columns(); l++) {
                    if (gabor(k,l) > mean) {
                        N++;
                    }
                }
            }
            if (N == 0) {
                N = 1;
            }

            /* Partitioni into blocks, count how many responses exceed mean, and calculate feature */
            for (int k = 0; k < gabor.rows(); k+=bh) {
                for (int l = 0; l < gabor.columns(); l+=bw) {

                    double Nb = 0;
                    for (int x = k; x < k+bh; x++) {
                        for (int y = l; y < l+bw; y++) {
                            if (gabor(x,y) > mean) {
                                Nb++;
                            }
                        }
                    }

                    /* Add the feature to the vector */
                    fv.push_back(Nb/N);

                }
            }

        }
    }


    /* Return the feature vector */
    return fv;

}

