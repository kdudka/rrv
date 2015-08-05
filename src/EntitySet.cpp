/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>, Jakub Filak
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

#include "EntitySet.h"
#include "PatchSequenceEnumerator.h"
#include "PatchRandomAccessEnumerator.h"
#include "TriangleSetExt.h"

/**
 * @param  entity
 */
void EntitySet::add (Entity *entity) {
    container_.push_back(*entity);
}

/**
 * @param  size
 */
void EntitySet::divide (float size ) {
    TContainer::iterator i;
    for(i= container_.begin(); i!= container_.end(); i++) {
        Entity &e = *i;
        e.divide(size);
    }
}

/**
 * @return PatchSequenceEnumerator*
 */
PatchSequenceEnumerator* EntitySet::createPatchSequenceEnumerator ( ) {
    IndirectPatchSequenceEnumerator *pe= new IndirectPatchSequenceEnumerator;

    TContainer::iterator i;
    for(i= container_.begin(); i!=container_.end(); i++) {
        Entity &e = *i;
        pe->add(e.createPatchSequenceEnumerator());
    }

    return pe;
}

size_t EntitySet::count()
{
    return container_.size();
}

Entity& EntitySet::operator[]( unsigned index )
{
    return container_[index];
}

TriangleSetExt* EntitySet::computeVertexColors() {
    TriangleSetExt *tset = new TriangleSetExt;

    TContainer::iterator i;
    for(i= container_.begin(); i!=container_.end(); i++) {
        Entity &e = *i;
        tset->add(e.computeVertexColors());
    }

    return tset;
}

