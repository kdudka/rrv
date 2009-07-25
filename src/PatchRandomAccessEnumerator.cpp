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

#include "PatchRandomAccessEnumerator.h"
#include "PatchSequenceEnumerator.h"
#include "Entity.h"
#include "EntitySet.h"

PatchRandomAccessEnumerator* PatchRandomAccessEnumerator::create(Entity *fromEntity) {
  return createGeneric(fromEntity);
}

PatchRandomAccessEnumerator* PatchRandomAccessEnumerator::create(EntitySet *fromEntitySet) {
  return createGeneric(fromEntitySet);
}

PatchRandomAccessEnumerator::PatchRandomAccessEnumerator (
		PatchSequenceEnumerator *pe)
{
	Triangle *t;
	for(int i=0; 0!= (t=pe->nextPatch()); i++)
		container_[i] = t;
}

/**
 * @return int
 */
int PatchRandomAccessEnumerator::count ( ) {
	return container_.size();
}


/**
 * @param index
 */
Triangle& PatchRandomAccessEnumerator::operator[] (unsigned index ) {
	Triangle *t = container_[index];
	return *t;
}
