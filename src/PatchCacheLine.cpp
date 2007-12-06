#include "PatchCacheLine.h"
#include "PatchRandomAccessEnumerator.h"
#include <assert.h>

PatchCacheLine::PatchCacheLine(PatchRandomAccessEnumerator *patchEnumerator, float ffTreshold):
		patchEnumerator_(patchEnumerator),
		ffTreshold_(ffTreshold)
{
}

void PatchCacheLine::addPatch(int patch, float formFactor) {
	assert(patch>=0);
	assert(patch<patchEnumerator_->count());
	if (formFactor <= ffTreshold_)
		return;
	
	Triangle &t= patchEnumerator_->operator[](patch);
	Color &color= t.radiosityLast;
	container_.push_back(TCacheItem(&color, formFactor));
}

Color PatchCacheLine::totalRadiosity() {
	Color radiosity(0.0, 0.0, 0.0);
	TContainer::iterator iter;
	for(iter= container_.begin(); iter!= container_.end(); iter++) {
		TCacheItem &cacheItem = *iter;
		Color c = *(cacheItem.first);
		c *= cacheItem.second;
		radiosity += c;
	}
	return radiosity;
}

size_t PatchCacheLine::itemCount() {
	return container_.size();
}
