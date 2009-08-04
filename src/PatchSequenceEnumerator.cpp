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

#include "PatchSequenceEnumerator.h"
#include "TriangleSet.h"

// virtual destructor
PatchSequenceEnumerator::~PatchSequenceEnumerator() {
}

DirectPatchSequenceEnumerator::DirectPatchSequenceEnumerator(TriangleSet *tset):
    tset_(*tset),
    current_(0),
    size_(tset->count())
{
}

    Triangle* DirectPatchSequenceEnumerator::nextPatch() {
        if (current_>=size_)
            // No next patch available
            return 0;

        // Human readable post-increment
        Triangle &t= tset_[current_];
        current_++;
        return &t;
    }

void DirectPatchSequenceEnumerator::reset() {
    current_ = 0;
}

IndirectPatchSequenceEnumerator::IndirectPatchSequenceEnumerator():
    current_(0)
{
}

IndirectPatchSequenceEnumerator::~IndirectPatchSequenceEnumerator() {
    TContainer::iterator i;
    for(i= container_.begin(); i!= container_.end(); i++)
        delete *i;
}

void IndirectPatchSequenceEnumerator::add(PatchSequenceEnumerator *pe) {
    container_.push_back(pe);
}

Triangle* IndirectPatchSequenceEnumerator::nextPatch() {
    Triangle *t;

    do {
        if (current_ >= static_cast<int>(container_.size()))
            // No next patch available
            return 0;

        t = container_[current_]->nextPatch();
        if (0== t)
            current_++;
    }
    while (0== t);

    return t;
}

void IndirectPatchSequenceEnumerator::reset() {
    TContainer::iterator i;
    for(i= container_.begin(); i!= container_.end(); i++)
        (*i)->reset();
    current_ = 0;
}


