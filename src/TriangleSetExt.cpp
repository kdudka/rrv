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

#include "TriangleSetExt.h"

/**
 * @param  triangle
 */
void TriangleSetExt::add (TriangleExt* triangle) {
	container_.push_back(*triangle);
}

void TriangleSetExt::add (TriangleSetExt *pTset) {
	TriangleSetExt &tset = *pTset;
	for(size_t i=0; i< tset.count(); i++) {
		TriangleExt &t = tset[i];
		this->add(&t);
	}
}

/**
 */
size_t TriangleSetExt::count ( ) {
	return container_.size();
}


/**
 * @param  index
 */
TriangleExt& TriangleSetExt::operator[] (unsigned index ) {
	return container_[index];
}
