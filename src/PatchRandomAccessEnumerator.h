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

#ifndef PATCHRANDOMACCESSENUMERATOR_H
#define PATCHRANDOMACCESSENUMERATOR_H

/**
 * @file PatchRandomAccessEnumerator.h
 * @brief Class PatchRandomAccessEnumerator - indexed access to patches.
 * @author xdudka00
 * @date 2007-11-17
 */

#include <map>

struct Triangle;
class Entity;
class EntitySet;
class PatchSequenceEnumerator;

/**
 * Adapter for PatchSequenceEnumerator, which does not support indexed (random) access.
 * @brief Indexed access to patches stored in generic hierarchical structure.
 * @note This enumerator hold large ammount of data.
 * It should be shared between objects as possible.
 */
class PatchRandomAccessEnumerator {
  public:
		/**
		 * @brief Create enumerator for all patches from desired Entity.
		 * @param forEntity Entity to create enumerator for.
		 * @return Return enumerator object allocated on the heap.
		 */
    static PatchRandomAccessEnumerator* create(Entity *forEntity);
		
		/**
		 * @brief Create enumerator for all patches from desired EntitySet.
		 * @param forEntitySet EntitySet to create enumerator for.
		 * @return Return enumerator object allocated on the heap.
		 */
    static PatchRandomAccessEnumerator* create(EntitySet *forEntitySet);

  private:
    template <class T>
    static PatchRandomAccessEnumerator* createGeneric(T *from) {
      PatchSequenceEnumerator *pe = from->createPatchSequenceEnumerator();
      PatchRandomAccessEnumerator *iter =
          new PatchRandomAccessEnumerator(pe);
      delete pe;
      return iter;
    }

  public:
		/**
		 * @brief Create indexed enumerator from non-indexed enumerator.
		 */
		PatchRandomAccessEnumerator(PatchSequenceEnumerator *);

		/**
		 * @brief Return total count of patches accessible trough enumerator.
		 * @return Return total count of patches accessible trough enumerator.
		 */
		int count();

		/**
		 * @brief Access patch using its index.
		 * @param index Index of desired patch. This index has to be in range <0, count()-1>.
		 */
		Triangle& operator[] (unsigned index);
		
	private:
		typedef std::map<unsigned, Triangle*> TContainer;
		TContainer container_;
};

#endif // PATCHRANDOMACCESSENUMERATOR_H
