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

#include "Entity.h"
#include "PatchSequenceEnumerator.h"
#include "CuboidEntity.h"
#include "BarrelEntity.h"
#include "GlobeEntity.h"
#include "TeapotEntity.h"
#include "TriangleEntity.h"
#include "TriangleSetExt.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;
using namespace XML;

namespace {
	inline std::string vertexToString(const Vertex &v) {
		std::ostringstream oss;
		oss << v.x << "," << v.y << "," << v.z;
		return oss.str();
	}
    inline float colorDist(Triangle *ta, Triangle *tb) {
        Color ca = ta->radiosity;
        Color cb = tb->radiosity;

        float dr = ca.r - cb.r;
        float dg = ca.g - cb.g;
        float db = ca.b - cb.b;

        return dr*dr + dg*dg + db*db;
    }
}

Entity::Entity():
	spec_(1.0),	
	refl_(.0),	
	refr_(.0),	
	patchSet_(0)
{
}

// Virtual destructor
Entity::~Entity() {
	delete patchSet_;
}

Entity::Entity(const Entity &e) {
  operator=(e);
}

Entity& Entity::operator=(const Entity &e) {
	if( this != &e )
	{
		transformMatrix_ = e.transformMatrix_;
		triangleSet_ = e.triangleSet_;
		name_ = e.name_;

		patchSet_ = (e.patchSet_)?
			new TriangleSet(*e.patchSet_):
			0;
	}

	return *this;
}

/**
 * @return Entity*
 * @param  from
 * @author Jakub Filak
 * @date   2007-10-25
 */
Entity* Entity::create (XMLNode *from ) {
	
	XMLCSTR nodeName = from->getName();

#ifndef NDEBUG
	if( 0 == nodeName )
	{
		std::cerr << "WARNING: entity not created!" << std::endl;
	}
#endif
	
	if( !strcmp( nodeName, XMLNames::TAGS[ Cuboid ] ) )
	{
		return new CuboidEntity();
	}
	else if( !strcmp( nodeName,XMLNames::TAGS[ Barrel ]) )
	{
		return new BarrelEntity();
	}
	else if( !strcmp( nodeName,XMLNames::TAGS[ Globe ]) )
	{
		return new GlobeEntity();
	}
	else if( !strcmp( nodeName,XMLNames::TAGS[ Teapot ]) )
	{
		return new TeapotEntity();
	}
	else if( !strcmp( nodeName,XMLNames::TAGS[ TriangleSetNode ]) )
	{
		return new TriangleEntity();
	}
#ifndef NDEBUG
	else
	{
		std::cerr << "WARNING: unknow entity type : " << nodeName << std::endl;
	}
#endif

	return 0;
}

void Entity::setName( XMLNode *from, const char* implicit )
{
	XMLCSTR name = from->getAttribute( XMLNames::ATTRIBUTES[ Name ] );
	
	if( 0 != name )
		setName( std::string( name ));
	else
		setName( std::string( implicit ));
}

/**
 * @param matrix
 */
void Entity::setTransformMatrix (TransformMatrix *matrix) {
	transformMatrix_ = *matrix;
}


/**
 * @param  from
 */
void Entity::deserialize (XMLNode *from) {
	this->setColors(from,this->emission_,this->reflectivity_,this->radiosity_);
	XMLHelper::fromAttribute<double>( spec_,  1, from, XMLNames::ATTRIBUTES[ Spec ], false );   
	XMLHelper::fromAttribute<double>( refl_, .0, from, XMLNames::ATTRIBUTES[ Refl ], false );   
	XMLHelper::fromAttribute<double>( refr_, .0, from, XMLNames::ATTRIBUTES[ Refr ], false );   
	impl_deserialize( from );
}

void Entity::impl_deserialize( XMLNode* ) {
	assert( !"Not implemented deserialization method" );
}

/**
 * @param  size
 */
void Entity::divide (float size ) {
 	TriangleSet *pset = new TriangleSet;
	
// Patched for old versions of MinGW
// 	class {
	class AnonStack {
		std::queue<Triangle> stack_;
		public:
			Triangle pop() {
				Triangle top = stack_.front();
				stack_.pop();
				return top;
			}
			void push(const Triangle &t) {
				stack_.push(t);
			}
			void push(TriangleSet *pTset) {
				TriangleSet &tset = *pTset;
				for (size_t i=0; i< tset.count(); i++)
					this->push(tset[i]);
			}
			bool empty() const {
				return stack_.empty();
			}
// Patched for old versions of MinGW
// 	} stack;
	}; AnonStack stack;
 	
	for (size_t i=0; i<triangleSet_.count(); i++)
		stack.push(triangleSet_[i]);
	
	while (!stack.empty()) {
		Triangle top = stack.pop();
		if (TriangleSet::size<1>(&top) <= size)
			pset->add(&top);
		else {
			TriangleSet *tset = TriangleSet::divide(&top);
			stack.push(tset);
			delete tset;
		}
	}
 	
 	delete patchSet_;
 	patchSet_ = pset;
}

void Entity::divide () {
    static const float MIN_PATCH_SIZE = 0.01; ///< TODO: read as parameter
    static const float MAX_PATCH_DIST = 0.01; ///< TODO: read as parameter

    TriangleSet &src = *patchSet_;

	// Build map Vertex -> list of Triangles
	typedef std::vector<Triangle *> TTrianglePtrList;
	typedef std::map<std::string, TTrianglePtrList> TVertexMap;
	TVertexMap vertexMap;
	for(size_t i=0; i<src.count(); i++) {
		Triangle &t = src[i];

		for(int j=0; j<3; j++) {
			TTrianglePtrList &cpList = vertexMap[vertexToString(t.vertex[j])];
			cpList.push_back(&t);
		}
	}

	TriangleSet *pset = new TriangleSet;

    // main loop
	for (size_t i=0; i<src.count(); i++) {
        Triangle &t = src[i];
        if (TriangleSet::size<1>(&t) < MIN_PATCH_SIZE) {
            // patch too small
            pset->add(&t);
            continue;
        }

        // compute color distance
        float maxDist = 0.0;
        for(int j=0; j<3; j++) {
			TTrianglePtrList &cpList = vertexMap[vertexToString(t.vertex[j])];
            for(size_t k=0; k<cpList.size(); k++) {
                float d = colorDist(&t, cpList[k]);
                if (maxDist < d)
                    maxDist = d;
            }
        }

        // divide if needed
        if (maxDist < MAX_PATCH_DIST)
            pset->add(&t);
        else
            pset->add(TriangleSet::divide(&t));
    }

 	delete patchSet_;
 	patchSet_ = pset;
}

/**
 * @return PatchSequenceEnumerator*
 */
PatchSequenceEnumerator* Entity::createPatchSequenceEnumerator ( ) {
	TriangleSet *tset = patchSet_;
	if (0== tset)
		tset = &triangleSet_;
	return new DirectPatchSequenceEnumerator(tset);
}


	class Converter {
		public:
		char* colorToStr( Color& c )
		{
			ostringstream oss;
			oss << c.r << "," << c.g << "," << c.b;
			return XMLWriter::copyString(oss.str().c_str());
		}

		template<typename V> char* valToS( V& f )
		{
			ostringstream oss;
			oss << f;
			return XMLWriter::copyString(oss.str().c_str());		
		}
	} converter;	


/**
 * @return XMLNode
 */
XMLNode Entity::serialize ( ) 
{
	XMLNode n = XMLNode::createXMLTopNode((XMLNames::TAGS[TriangleSetNode] ));
	n.addAttribute_WOSD( XMLWriter::copyString( XMLNames::ATTRIBUTES[Name]), XMLWriter::copyString( getName().c_str() ) );
	PatchSequenceEnumerator *e = createPatchSequenceEnumerator();

	Triangle *t = 0;
	XMLNode tnode;
	while( 0 != ( t = e->nextPatch() ) )
	{
		tnode = n.addChild_WOSD( XMLWriter::copyString( XMLNames::TAGS[TriangleNode] ) );

		tnode.addAttribute_WOSD( 
				XMLWriter::copyString( XMLNames::ATTRIBUTES[Emission] ), 
				converter.colorToStr(t->emission) );

		tnode.addAttribute_WOSD( 
				XMLWriter::copyString( XMLNames::ATTRIBUTES[Reflectivity] ),
				converter.colorToStr(t->reflectivity) );

		tnode.addAttribute_WOSD(
				XMLWriter::copyString( XMLNames::ATTRIBUTES[Radiosity] ),
				converter.colorToStr(t->radiosity) );

		tnode.addAttribute_WOSD(
				XMLWriter::copyString( XMLNames::ATTRIBUTES[Spec] ),
				converter.valToS<double>(t->spec) );

		tnode.addAttribute_WOSD(
				XMLWriter::copyString( XMLNames::ATTRIBUTES[Refl] ),
				converter.valToS<double>(t->refl) );

		tnode.addAttribute_WOSD(
				XMLWriter::copyString( XMLNames::ATTRIBUTES[Refr] ),
				converter.valToS<double>(t->refr) );

		for( int i = 0; i < 3; i++ )
		{
			XMLNode vnode = tnode.addChild_WOSD( XMLWriter::copyString( XMLNames::TAGS[VertexNode] ) );
			
			vnode.addAttribute_WOSD(
					XMLWriter::copyString( XMLNames::ATTRIBUTES[VertexX] ), 
					converter.valToS<float>( t->vertex[i].x ) );

			vnode.addAttribute_WOSD( 
					XMLWriter::copyString( XMLNames::ATTRIBUTES[VertexY] ), 
					converter.valToS<float>( t->vertex[i].y ) );
			
			vnode.addAttribute_WOSD( 
					XMLWriter::copyString( XMLNames::ATTRIBUTES[VertexZ] ), 
					converter.valToS<float>( t->vertex[i].z ) );
		}
	}

	delete e;
	return n;
}

void Entity::setTriangleProperties( Triangle& triangle )
{
	triangle.emission = this->emission_;
	triangle.reflectivity = this->reflectivity_;
	triangle.radiosity = this->radiosity_;
	
	triangle.spec = this->spec_;
	triangle.refl = this->refl_;
	triangle.refr = this->refr_;
}

/**
 * @param  triangle
 */
void Entity::addTriangle ( Triangle *triangle ) {
	// Transform all vertex using transformMatrix_ matrix
	Triangle t = *triangle;

	for (int i=0; i<3; i++) {
		Vertex &v = t.vertex[i];
		v = transformMatrix_.transform(v);
	}
	
	// Add transformed triangle to container
	triangleSet_.add(&t);
}

#ifndef  NDEBUG
bool Entity::colorFromXMLNode( XMLNode *node, XMLCSTR attName, Color& to, int wlevel) 
#else
bool Entity::colorFromXMLNode( XMLNode *node, XMLCSTR attName, Color& to) 
#endif
{
	float rgb[] = {0.0,0.0,0.0}; 			// array for colors

	XMLCSTR fRGBValues = node->getAttribute( attName );	// try take attribute color

	if ( 0 == fRGBValues || strlen(fRGBValues) < 5 ) // "1,1,1"
	{	
#ifndef NDEBUG
		if ( wlevel > 1 )
		{
			cerr << "WARNING: node: " << 
				node->getName() << " - " << attName 
				<< " , color unrecognized, using default BLACK" << endl;	
		}
#endif

		to = Color( rgb );
		return false; 
	}
	
	XMLCSTR mPtr = fRGBValues;
	int i = 0;

	while( 0 != *mPtr )
	{
		ostringstream oss;

		while( 0 != *mPtr && ',' != *mPtr )
		{
			oss << *mPtr;
			mPtr++;
		}

		string fln = oss.str();
		istringstream iss( fln );

		float tmp;

		if ( ( iss >> tmp ).fail() )
		{	
#ifndef NDEBUG
			if ( wlevel > 0 )
			{
				cerr << "WARNING: node: " << node->getName() << " , bad color format : " << fRGBValues << endl;
			}
#endif

			to = Color( rgb );
			return false;
		}
		else
		{
			if ( i < 3 )
			{
				rgb[i] = tmp;
				i++;
			}
		}

		if( 0 != *mPtr )
			mPtr++;
	}	

	to = Color( rgb );
	return true;
}

void Entity::setColors( XMLNode* from, Color& em, Color& refl, Color& rad )
{
	Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[ Emission     ] , em );
	Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[ Reflectivity ] , refl );
	Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[ Radiosity    ] , rad );
}

TriangleSetExt* Entity::computeVertexColors() {
	// Create and fill TriangleSetExt
	TriangleSetExt *tset = new TriangleSetExt;
	PatchSequenceEnumerator *patchEnumerator = createPatchSequenceEnumerator();
	Triangle *t;
	while (0!= (t= patchEnumerator->nextPatch())) {
		TriangleExt te(*t);
		tset->add(&te);
	}
	delete patchEnumerator;
	
	// Build map Vertex -> list of colors
	typedef std::vector<Color *> TColorPtrList;
	typedef std::map<std::string, TColorPtrList> TVertexMap;
	TVertexMap vertexMap;
	for(size_t i=0; i<tset->count(); i++) {
		TriangleExt &te = tset->operator[](i);
		
		for(int j=0; j<3; j++) {
			TColorPtrList &cpList = vertexMap[vertexToString(te.vertex[j])];
			cpList.push_back(te.vertexColor+j);
		}
	}
	
	// Interpolate vertex colors from triangle colors
	TVertexMap::iterator i;
	for(i= vertexMap.begin(); i!= vertexMap.end(); i++) {
		TColorPtrList &cpList= i->second;
		TColorPtrList::iterator j;
		
		Color sum(0.0, 0.0, 0.0);
		int n=0;
		for(j= cpList.begin(); j!= cpList.end(); j++) {
			Color &color= **j;
			if (Color(0.0,0.0,0.0)==color)
				// Ignore black triangles
				continue;
			sum += color;
			n++;
		}
		if (n>1)
			sum *= 1.0f/n;
		
		for(j= cpList.begin(); j!= cpList.end(); j++) {
			Color &color= **j;
			color = sum;
		}
	}
	
	// Save 1st level interpolation
	for(size_t i=0; i<tset->count(); i++) {
		TriangleExt &te = tset->operator[](i);
		
		for(int j=0; j<3; j++)
			te.vertexColorRaw[j] = te.vertexColor[j];
	}
	
	// Interpolate triangle colors from vertex colors
	for(size_t i=0; i<tset->count(); i++) {
		TriangleExt &te = tset->operator[](i);
		
		Color sum(0.0, 0.0, 0.0);
		for(int j=0; j<3; j++) {
			sum += te.vertexColor[j];
		}
		sum *= 1.0f/3;
		te.radiosityLast = sum;
		for(int n=0; n<3; n++)
			te.vertexColor[n] = sum;
	}
	
	// Interpolate vertex colors from trinagle colors again
	for(i= vertexMap.begin(); i!= vertexMap.end(); i++) {
		TColorPtrList &cpList= i->second;
		TColorPtrList::iterator j;
		Color sum(0.0, 0.0, 0.0);
		for(j= cpList.begin(); j!= cpList.end(); j++) {
			Color &color= **j;
			sum += color;
		}
		sum *= 1.0f/cpList.size();
		for(j= cpList.begin(); j!= cpList.end(); j++) {
			Color &color= **j;
			color = sum;
		}
	}
	
	return tset;
}
