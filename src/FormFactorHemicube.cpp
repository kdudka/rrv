/*
 * Copyright (C) 2007 David Barina <DaBler@gmail.com>
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

#include "FormFactorHemicube.h"

#include <cassert>
#include <cmath>
#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

FormFactorHemicube::FormFactorHemicube (int edge, FormFactorHemicube::Mode mode, bool normalize):
    edge_(edge)
{
    assert(edge > 0);

    int EDGE_1 = edge;
    int EDGE_2 = 2 * edge;
    int EDGE_1_2 = edge / 2;
    int EDGE_3_2 = edge / 2 + edge;

    ffcoefs = new double*[EDGE_2];
    for(int i=0; i<EDGE_2; i++)
        ffcoefs[i] = new double[EDGE_2];

    for(int i=0; i<EDGE_2; i++)
        for(int j=0; j<EDGE_2; j++)
            if( (i < EDGE_1_2 || EDGE_3_2 <= i) && (j < EDGE_1_2 || EDGE_3_2 <= j) )
                ffcoefs[i][j] = 0; // for normalization
            else
                switch(mode)
                {
                default:
                case RRV2007:
                    {
                        int tw = -EDGE_1 + i;
                        int th = -EDGE_1 + j;
                        double R = EDGE_2;
                        double cw = cos( M_PI * tw/R );
                        double ch = cos( M_PI * th/R );
                        double S = EDGE_1 * EDGE_1 + 4 * EDGE_1 * EDGE_1_2;
                        ffcoefs[i][j] = 2 * cw*ch / S;
                    }
                    break;
                case Cohen1985:
                    {
                        double x = (i + 0.5 - EDGE_1) / EDGE_1_2;
                        double y = (j + 0.5 - EDGE_1) / EDGE_1_2;
                        double z = 1;
                        if (x < -1)
                        {
                            z = 2 + x;
                            x = -1;
                        }
                        if (x > 1)
                        {
                            z = 2 - x;
                            x = 1;
                        }
                        if (y < -1)
                        {
                            z = 2 + y;
                            y = -1;
                        }
                        if (y > 1)
                        {
                            z = 2 - y;
                            y = 1;
                        }
                        double S = EDGE_1 * EDGE_1 + 4 * EDGE_1 * EDGE_1_2;
                        ffcoefs[i][j] = 8 * z / (M_PI * (x * x + y * y + z * z)) / S;
                    }
                    break;
                }

    if(normalize)
    {
        double sum = 0;
        for(int i=0; i<EDGE_2; i++)
            for(int j=0; j<EDGE_2; j++)
                sum += ffcoefs[i][j];
        for(int i=0; i<EDGE_2; i++)
            for(int j=0; j<EDGE_2; j++)
                ffcoefs[i][j] /= sum;
    }
}

FormFactorHemicube::~FormFactorHemicube ()
{
    if(ffcoefs)
    {
        int EDGE_2 = 2 * edge_;
        for(int i=0; i<EDGE_2; i++)
            if(ffcoefs[i])
                delete[] ffcoefs[i];
        delete[] ffcoefs;
    }
}

double FormFactorHemicube::ff (int i, int j) const
{
    assert(0 <= i);
    assert(i < 2 * edge_);
    assert(0 <= j);
    assert(j < 2 * edge_);
    return ffcoefs[i][j];
}

int FormFactorHemicube::edge () const
{
    return edge_;
}
