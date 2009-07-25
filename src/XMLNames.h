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

#ifndef XMLNAMES_H
#define XMLNAMES_H
	
namespace XML
{
	typedef enum XMLTagName
	{
		Root=0,
		Definition,
		Instantiate,
		ObjectDefinition,
		Cuboid,
		Barrel,
		Globe,
		Teapot,
		TriangleSetNode,
		TriangleNode,
		VertexNode,
		Rotate,
		Scale,
		Shear,
		Translate,
		Object,
		Trianglenext,
		angle
       	} XMLTN;
	
	typedef enum XMLTagAttributeName
	{
		Name=0,
		Emission,
		Reflectivity,
		Radiosity,
		VertexX,
		VertexY,
		VertexZ,
		TransformationX,
		TransformationY,
		TransformationZ,
		RotateAngle,
		IncludeFile,
		Spec,
		Refr,
		Refl
	} XMLTAN;
	
	struct XMLNames
	{
		public:
			static const char* TAGS[];

			static const char* ATTRIBUTES[];
	};
}	
#endif
