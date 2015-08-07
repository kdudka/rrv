/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>
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

#ifndef PATCHCACHELINE_H
#define PATCHCACHELINE_H

/**
 * @file PatchCacheLine.h
 * @brief Class PatchCacheLine - low-level cache for radiosity renderer.
 * @author xdudka00
 * @date 2007-11-18
 */

#include "Triangle.h"
#include "LinearAlgebra.h"

#include <cstddef>              // for size_t
#include <vector>

class PatchRandomAccessEnumerator;

/**
 * The one and only communication way between RadiosityRenderer and FormFactorEngine classes.
 * @brief Low-level cache for radiosity renderer.
 */
class PatchCacheLine {
    public:
        /**
         * @param patchEnumerator @b Shared instance of PatchRandomAccessEnumerator.
         * @param ffTreshold Pair of patches with smaller form factor than formFactorTreshold will be ignored.
         */
        PatchCacheLine(PatchRandomAccessEnumerator *patchEnumerator, float ffTreshold);
        ~PatchCacheLine();

        /**
         * @brief Add patches to cache-line.
         * @param formFactors Source form factor vector for this patch.
         * @note There is no need to specify destination patch, because destination patch is cache-line specific.
         * @note The input is copied to a CompactVector.
         */
        void addPatches(const DenseVector<float> &formFactors);

        /**
         * This computation respectes cached form factor for each patch.
         * @brief @return Return radiosity summarized from all patches saved in cache-line.
         */
        Color totalRadiosity(const DenseVector<Color> &sceneRadiosity) const;

        /**
         * @brief Return raw allocation size of cache line contents.
         */
        size_t size() const;

    private:
        PatchRandomAccessEnumerator *patchEnumerator_;
        float ffTreshold_;
        CompactVector<float> *formFactors_;
};

#endif // PATCHCACHELINE_H
