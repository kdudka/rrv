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

#ifndef PATCHSEQUENCEENUMERATOR_H
#define PATCHSEQUENCEENUMERATOR_H

/**
 * @file PatchSequenceEnumerator.h
 * @brief Class PatchSequenceEnumerator - tree-topology patch enumerator.
 * @author xdudka00
 * @date 2007-11-17
 */

#include <vector>

struct Triangle;
class TriangleSet;

/**
 * Leaf nodes are instances of DirectPatchSequenceEnumerator and
 * other nodes are instances of IndirectPatchSequenceEnumerator.
 * @brief Tree-topology patch enumerator.
 */
class PatchSequenceEnumerator {
    public:
        virtual ~PatchSequenceEnumerator();

        /**
         * @brief Return current patch and move to the next one.
         * @return Return current patch (before move).
         * Return 0 if there is no current patch (behind range).
         */
        virtual Triangle* nextPatch() =0;

        /**
         * @brief Move enumerator to first patch.
         */
        virtual void reset() =0;
};

/**
 * See PatchSequenceEnumerator documentation.
 * @brief Class for leaf nodes of PatchSequenceEnumerator object tree.
 */
class DirectPatchSequenceEnumerator: public PatchSequenceEnumerator {
    public:
        /**
         * @brief Create direct patch enumerator.
         * @param tset TriangleSet to create enumerator for.
         */
        DirectPatchSequenceEnumerator(TriangleSet *tset);
        virtual Triangle* nextPatch();
        virtual void reset();

    private:
        TriangleSet &tset_;
        int current_;
        int size_;
};

/**
 * See PatchSequenceEnumerator documentation.
 * @brief Class for non-leaf nodes of PatchSequenceEnumerator object tree.
 */
class IndirectPatchSequenceEnumerator: public PatchSequenceEnumerator {
    public:
        IndirectPatchSequenceEnumerator();
        virtual ~IndirectPatchSequenceEnumerator();
        virtual Triangle* nextPatch();
        virtual void reset();

        /**
         * @brief Add child node of PatchSequenceEnumerator object tree.
         * @param patchEnumerator Child node to add.
         */
        void add(PatchSequenceEnumerator *patchEnumerator);

    private:
        typedef std::vector<PatchSequenceEnumerator *> TContainer;
        TContainer container_;
        int current_;
};

#endif // PATCHSEQUENCEENUMERATOR_H
