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
