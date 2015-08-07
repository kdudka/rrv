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

#include "PatchCache.h"
#include "PatchRandomAccessEnumerator.h"
#include "FormFactorEngine.h"

PatchCache::PatchCache(
        PatchRandomAccessEnumerator *patchEnumerator,
        float ffTreshold,
        size_t maxCacheSize,
        const FormFactorHemicube &hemicube
        ):
    patchEnumerator_(patchEnumerator),
    ffTreshold_(ffTreshold),
    maxCacheSize_(maxCacheSize),
    hemicube_(hemicube),
    cachedSize_(0)
{
    patchCount_ = patchEnumerator->count();

    // FIXME: Does this constructor zero vector's items?
    cache_ = new TCache(patchCount_, 0);

    // Create patch cache (priority) queue
    cacheQueue_ = new TQueue;

    ffe_ = new FormFactorEngine(patchEnumerator, hemicube_);
}

PatchCache::~PatchCache() {
    delete ffe_;
    delete cacheQueue_;

    // Destroy patch cache
    TCache::iterator i;
    for(i= cache_->begin(); i!= cache_->end(); i++)
        delete *i;
    delete cache_;
}

size_t PatchCache::cacheRawSize() const {
    return cachedSize_;
}

Color PatchCache::totalRadiosity(int destPatch, const DenseVector<Color> &sceneRadiosity) {
    // master branche
    PatchCacheLine *&cacheLine = cache_->operator[](destPatch);
    if (0==cacheLine) {
        // Cache-line was not in cache --> create and fill
        cacheLine = new PatchCacheLine(patchEnumerator_, ffTreshold_);
        ffe_->fillCacheLine(destPatch, cacheLine);

        // Update metadata
        cachedSize_ += cacheLine->size();
        cacheQueue_->push(&cacheLine);
    }

    // Use cache-line to cumpute total radiosity
    Color rad = cacheLine->totalRadiosity(sceneRadiosity);

    // master branche
    if (this->cacheRawSize() >= maxCacheSize_) {
        // maxCacheSize exceed --> free the largest cache-line
        const TQueueItem &qi = cacheQueue_->top();
        PatchCacheLine *&topCL = qi.pCacheLine();
        cachedSize_ -= topCL->size();
        delete topCL;
        topCL = 0;
        cacheQueue_->pop();
    }

    return rad;
}
