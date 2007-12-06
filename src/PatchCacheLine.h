#ifndef PATCHCACHELINE_H
#define PATCHCACHELINE_H

/**
 * @file PatchCacheLine.h
 * @brief Class PatchCacheLine - low-level cache for radiosity renderer.
 * @author xdudka00
 * @date 2007-11-18
 */

#include "Triangle.h"

#include <vector>

class PatchRandomAccessEnumerator;

/**
 * The one and only communication way between RadiosityRenderer and FormFactorEngine classes.
 * @brief Low-level cache for radiosity renderer.
 */
class PatchCacheLine {
	public:
		/**
		 * @param patchEnumerator @b Shared instance of PatchRandomAccessEnumerator.
		 * @param ffTreshold Pair of patches with smaller form factor than formFactorTreshold will be ignored.
		 */
		PatchCacheLine(PatchRandomAccessEnumerator *patchEnumerator, float ffTreshold);
		
		/**
		 * @brief Add patch to cache-line (and save form factor of pair of patches).
		 * @param srcPatch Source patch from a pair of patches.
		 * @param formFactor Form factor of a pair of patches.
		 * @note There is no need to specify destination patch, because destination patch is cache-line specific.
		 */
		void addPatch(int srcPatch, float formFactor);
		
		/**
		 * This computation respectes cached form factor for each patch.
		 * @brief @return Return radiosity summarized from all patches saved in cache-line.
		 */
		Color totalRadiosity();
		
		/**
		 * @brief Return total count of patch saved in cache-line.
		 */
		size_t itemCount();
		
		/**
		 * @brief Return raw allocation size of one cache item.
		 */
		static size_t itemSize() {
			return sizeof(TCacheItem);
		}

	private:
		PatchRandomAccessEnumerator *patchEnumerator_;
		float ffTreshold_;
		typedef std::pair<Color *, float> TCacheItem;
		typedef std::vector<TCacheItem> TContainer;
		TContainer container_;
};

#endif // PATCHCACHELINE_H
