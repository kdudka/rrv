/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>
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

#ifndef RADIOSITYRENDERER_H
#define RADIOSITYRENDERER_H

/**
 * @file RadiosityRenderer.h
 * @brief Class RadiosityRenderer - radiosity rendering engine.
 * @author xdudka00
 * @date 2007-11-18
 */

#include "Triangle.h"
#include "ProgressSubjectMultiStep.h"
#include "FormFactorHemicube.h"

class PatchSequenceEnumerator;
class PatchRandomAccessEnumerator;
class PatchCache;

/**
 * @brief Radiosity rendering engine.
 */
class RadiosityRenderer: public ProgressSubjectMultiStep {
    public:
        /**
         * @param patchEnumerator Instance of PatchSequenceEnumerator - can be @b deleted immediately ater construction.
         * @param stepCount Count of radiosity computation steps.
         * @param formFactorTreshold Pair of patches with smaller form factor than formFactorTreshold will be ignored.
         * @param maxCacheSize Maximum size of patch cache (in bytes).
         * @note Maximum cache size is raw size (estimated). The real cache size can be greater.
         * @param hcEdge hemicube edge length
         * @param hcMode hemicube form factor algorithm
         * @param hcNormalize hemicube normalization
         */
        RadiosityRenderer(PatchSequenceEnumerator *patchEnumerator, int stepCount, float formFactorTreshold, long maxCacheSize, int hcEdge = 256, FormFactorHemicube::Mode hcMode = FormFactorHemicube::Cohen1985, bool hcNormalize = true);
        ~RadiosityRenderer();

        /**
         * @brief Return total count of radiosity computation steps.
         */
        virtual int stepCount() const;

        /**
         * @brief Return number of current step - always in range <0, stepCount()-1>.
         */
        virtual int currentStep() const;

        /**
         * @brief Return total count of patches maintained by RadiosityRenderer.
         */
        virtual int patchCount() const;

        /**
         * @brief Return number of current patch computed by RadiosityRenderer - always in range <0, patchCount()-1>.
         */
        virtual int currentPatch() const;

        /**
         * @brief Initiate radiosity rendering progress.
         */
        void compute();

        /**
         * @brief Optional color normalization.
         * @attention This method can't be called before compute()!
         */
        void normalize();

        /**
         * @brief Return current patch cache size.
         * @copydoc PatchCache::cacheRawSize()
         */
        long int cacheRawSize() const;

    private:
        int stepCount_;
        int currentStep_;
        int currentPatch_;
        PatchRandomAccessEnumerator *patchEnumerator_;
        int patchCount_;
        PatchCache *patchCache_;
        FormFactorHemicube hemicube_;

    private:
        float colorPeak_;
        void updateColorPeak(const Color &c) {
            if (colorPeak_ < c.r)
                colorPeak_ = c.r;
            if (colorPeak_ < c.g)
                colorPeak_ = c.g;
            if (colorPeak_ < c.b)
                colorPeak_ = c.b;
        }
        void normalize(Color &dest) {
            float ratio = 1/colorPeak_;
            dest.r *= ratio;
            dest.g *= ratio;
            dest.b *= ratio;
        }
        void computeStep();
};

#endif // RADIOSITYRENDERER_H
