/*
 * Copyright (C) 2015 Claude Heiland-Allen <claude@mathr.co.uk>
 *
 * This file is part of rrv (Radiosity Renderer and Visualizer).
 *
 * rrv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * rrv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rrv.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FORMFACTORHEMICUBE_H
#define FORMFACTORHEMICUBE_H

/**
 * @file FormFactorHemicube.h
 * @brief Class FormFactorHemicube - compute per-pixel form factors for a hemicube.
 * @author Claude Heiland-Allen
 * @date 2015-08-02
 */

/**
 * @brief Compute per-pixel form factors for a hemicube.
 */
class FormFactorHemicube {
    public:

        /**
         * @brief Choice of algorithms to calculate the form factors.
         */
        enum Mode
        {
            /**
             * @brief RRV's original implementation.
             */
            RRV2007,
            /**
             * @brief Cohen and Greenberg's implementation, 1985.
             * The Hemi-cube: a Radiosity Solution for Complex Environments,
             * Michael F. Cohen and Donald P. Greenberg,
             * ACM SIGGRAPH Volume 19, Number 3, 1985.
             */
            Cohen1985
        };

        /**
         * @brief Constructor, allocates memory and calculates form factors.
         * @param edge The edge length of the hemicube in pixels.
         * @param mode Which algorithm to use.
         * @param normalize Whether to ensure that the sum of form factors is 1.
         */
        FormFactorHemicube (int edge, Mode mode, bool normalize);

        /**
         * @brief Destructor, frees memory.
         */
        ~FormFactorHemicube ();

        /**
         * @brief Retrieve the form factor for a pixel on the hemicube.
         * The top (forward) direction is a square in [edge/2 .. edge/2 + edge),
         * with the other sides in the rectangles - the corners are set
         * to zero.  Outside the bounds [0 .. 2*edge), results are undefined.
         * Use of odd edge lengths is untested.
         */
        double ff (int i, int j) const;

        /**
         * @brief Retrieve the hemicube edge length.
         */
        int edge () const;

    private:
        int edge_;
        double **ffcoefs;
};

#endif // FORMFACTORHEMICUBE_H
