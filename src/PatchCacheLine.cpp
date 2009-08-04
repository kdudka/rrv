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
