/*
 * Copyright (C) 2007 TODO
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
	long maxCacheSize
	):
		patchEnumerator_(patchEnumerator),
		ffTreshold_(ffTreshold),
		maxCacheSize_(maxCacheSize),
		cachedItems_(0)
{
	patchCount_ = patchEnumerator->count();
	
	// FIXME: Does this constructor zero vector's items?
	cache_ = new TCache(patchCount_, 0);
	
	// Create patch cache (priority) queue
	cacheQueue_ = new TQueue;

	ffe_ = new FormFactorEngine(patchEnumerator);
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

long int PatchCache::cacheRawSize() const {
	return
			static_cast<long int>(sizeof(TCache) + sizeof(TQueue)) +
			static_cast<long int>(sizeof(PatchCacheLine) + sizeof(TQueueItem)) * patchCount_ +
			static_cast<long int>(PatchCacheLine::itemSize()) *cachedItems_;
}

Color PatchCache::totalRadiosity(int destPatch) {	
	PatchCacheLine *&cacheLine = cache_->operator[](destPatch);
	if (0==cacheLine) {
		// Cache-line was not in cache --> create and fill
		cacheLine = new PatchCacheLine(patchEnumerator_, ffTreshold_);
		ffe_->fillCacheLine(destPatch, cacheLine);
		
		// Update metadata
		cachedItems_ += cacheLine->itemCount();
		cacheQueue_->push(&cacheLine);
	}
	
	// Use cache-line to cumpute total radiosity
	Color rad = cacheLine->totalRadiosity();
	
	if (this->cacheRawSize() >= maxCacheSize_) {
		// maxCacheSize exceed --> free the largest cache-line
		const TQueueItem &qi = cacheQueue_->top();
		PatchCacheLine *&topCL = qi.pCacheLine();
		cachedItems_ -= static_cast<int>(qi);
		delete topCL;
		topCL = 0;
		cacheQueue_->pop();
	}
	
	return rad;
}
