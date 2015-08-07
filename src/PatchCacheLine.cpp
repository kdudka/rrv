/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>
 * Copyright (C) 2015 Claude Heiland-Allen <claude@mathr.co.uk>
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

PatchCacheLine::PatchCacheLine(PatchRandomAccessEnumerator *patchEnumerator, float ffTreshold):
    patchEnumerator_(patchEnumerator),
    ffTreshold_(ffTreshold),
    formFactors_(0)
{
}

PatchCacheLine::~PatchCacheLine()
{
    if (formFactors_)
        delete formFactors_;
}

void PatchCacheLine::addPatches(const DenseVector<float> &formFactors) {
    if (formFactors_)
        delete formFactors_;
    formFactors_ = new CompactVector<float>(formFactors, ffTreshold_);
}

Color PatchCacheLine::totalRadiosity(const DenseVector<Color> &sceneRadiosity) const {
    return formFactors_->dot(sceneRadiosity);
}

size_t PatchCacheLine::size() const{
    return formFactors_->size();
}
