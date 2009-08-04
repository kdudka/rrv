/*
 * Copyright (C) 2007 Jakub Filak
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

#include "XMLNames.h"

const char* XML::XMLNames::TAGS[] = {
    "scene",
    "head",
    "body",
    "objectdef",
    "cuboid",
    "barrel",
    "globe",
    "teapot",
    "triangleset",
    "triangle",
    "vertex",
    "rotate",
    "scale",
    "shear",
    "translate",
    "object",
    "trianglenext" };

const char* XML::XMLNames::ATTRIBUTES[] = {
    "name",
    "emission",
    "reflectivity",
    "radiosity",
    "x",
    "y",
    "z",
    "dx",
    "dy",
    "dz",
    "angle",
    "file",
    "spec",
    "refr",
    "refl"};
