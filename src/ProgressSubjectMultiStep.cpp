#include "ProgressSubjectMultiStep.h"
#include "ProgressObserverMultiStep.h"

#include <algorithm>

#ifndef NDEBUG
#	include <iostream>
#endif

/**
 * @param  observer
 * @param  perStepFreq
 */
void ProgressSubjectMultiStep::attach (ProgressObserverMultiStep* observer, int perStepFreq) {
	TItem item;
	item.ratio = patchCount()/perStepFreq;
	item.offset = patchCount()%perStepFreq;
	item.last = 0;
	item.observer = observer;
	
	observerList_.push_back(item);
}
	
class ObserverFinder {
	ProgressObserverMultiStep *observer_;
public:
	ObserverFinder(ProgressObserverMultiStep *observer):
			observer_(observer)
	{	}
	bool operator()(const ProgressSubjectMultiStep::TItem &item) {
		return item.observer == observer_;
	}
};
/**
 * @param  observer
 */
void ProgressSubjectMultiStep::detach (ProgressObserverMultiStep* observer) {
	observerList_.remove_if(ObserverFinder(observer));
}

void ProgressSubjectMultiStep::notifyStep() {
	// Must be copy of working list to avoid race condition!!
	TList listCopy(observerList_);
	TList::iterator i;
	for(i= listCopy.begin(); i!= listCopy.end(); i++)
		i->observer->updateStep();
}

void ProgressSubjectMultiStep::notifyPerStepProgress() {
	// Must be copy of working list to avoid race condition!!
	TList listCopy(observerList_);
	TList::iterator i;
	for(i= listCopy.begin(); i!= listCopy.end(); i++) {
		// Must be original item (not copy)
		TList::iterator originalItem = std::find_if(
				observerList_.begin(),
				observerList_.end(),
				ObserverFinder(i->observer));
		TItem &item = *originalItem;
		
		int count = currentPatch()-item.offset+1;
		if (count < 0)
			count = 0;
		
		int n = (item.ratio)?count/item.ratio:(item.last+1);
		if (n != item.last) {
			item.last = n;
			item.observer->updatePerStepProgress();
		}
	}
}
