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

#ifndef PATCHCACHELINE_H
#define PATCHCACHELINE_H

/**
 * @file PatchCacheLine.h
 * @brief Class PatchCacheLine - low-level cache for radiosity renderer.
 * @author xdudka00
 * @date 2007-11-18
 */

#include "Triangle.h"

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

        /**
         * @brief Add patch to cache-line (and save form factor of pair of patches).
         * @param srcPatch Source patch from a pair of patches.
         * @param formFactor Form factor of a pair of patches.
         * @note There is no need to specify destination patch, because destination patch is cache-line specific.
         */
        void addPatch(int srcPatch, float formFactor);

        /**
         * This computation respectes cached form factor for each patch.
         * @brief @return Return radiosity summarized from all patches saved in cache-line.
         */
        Color totalRadiosity();

        /**
         * @brief Return total count of patch saved in cache-line.
         */
        size_t itemCount();

        /**
         * @brief Return raw allocation size of one cache item.
         */
        static size_t itemSize() {
            return sizeof(TCacheItem);
        }

    private:
        PatchRandomAccessEnumerator *patchEnumerator_;
        float ffTreshold_;
        typedef std::pair<Color *, float> TCacheItem;
        typedef std::vector<TCacheItem> TContainer;
        TContainer container_;
};

#endif // PATCHCACHELINE_H
