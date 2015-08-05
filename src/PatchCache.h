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

#ifndef PATCHCACHE_H
#define PATCHCACHE_H

/**
 * @file PatchCache.h
 * @brief Class PatchCache - cache for radiosity renderer.
 * @author xdudka00
 * @date 2007-11-18
 */

#include "PatchCacheLine.h"

#include <vector>
#include <queue>
class FormFactorEngine;
class PatchRandomAccessEnumerator;

/**
 * Now using priority queue to optimal usage of given memory.
 * @brief Cache for radiosity renderer.
 */
class PatchCache {
    public:
        /**
         * @param patchEnumerator @b Shared instance of PatchRandomAccessEnumerator.
         * @param ffTreshold Pair of patches with smaller form factor than formFactorTreshold will be ignored.
         * @param maxCacheSize Maximum size of patch cache (in bytes).
         * @note Maximum cache size is raw size (estimated). The real cache size can be greater.
         */
        PatchCache(PatchRandomAccessEnumerator *patchEnumerator, float ffTreshold, long maxCacheSize);
        ~PatchCache();

        /**
         * This computation respectes form factor for each patch.
         * @brief @return Return radiosity summarized from all patches.
         * @param destPatch Destination patch to compute radiosity for.
         */
        Color totalRadiosity(int destPatch);

        /**
         * @brief Return current patch cache size.
         * @note This is raw size (estimated). The real cache size can be greater.
         */
        long int cacheRawSize() const;

    private:
        PatchRandomAccessEnumerator *patchEnumerator_;
        float ffTreshold_;
        long maxCacheSize_;
        size_t patchCount_;
        FormFactorEngine *ffe_;

        // Cache container (containing cache-lines)
        typedef std::vector<PatchCacheLine*> TCache;
        TCache *cache_;
        long int cachedItems_;

        // Priority queue used to optimize memory usage
        class TQueueItem {
            PatchCacheLine **cl_;
            public:
            TQueueItem(PatchCacheLine **cl): cl_(cl) { }
            // Key of priority queue ordering
            operator int() const {
                return (*cl_)->itemCount();
            }
            // Return reference to pointer to cache-line
            PatchCacheLine*& pCacheLine() const {
                return (*cl_);
            }
        };
        typedef std::priority_queue<TQueueItem> TQueue;
        TQueue *cacheQueue_;
};

#endif // PATCHCACHE_H
