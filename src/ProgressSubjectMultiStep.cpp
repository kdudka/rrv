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

void ProgressSubjectMultiStep::notifyPatchCountChanged() {
    // Must be copy of working list to avoid race condition!!
    TList listCopy(observerList_);
    TList::iterator i;
    for(i= listCopy.begin(); i!= listCopy.end(); i++)
        i->observer->updatePatchCount();
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
