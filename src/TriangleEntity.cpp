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

#include "TriangleEntity.h"
#include <iostream>
#include <sstream>
#include <cstring>

using namespace XML;

/*
 * @param  XMLnode*
 * @return Vertex
 * @brief  Convert XMLNode to Vertex. If any coord attribute isn't presented then 0 is used.
 */
Vertex TriangleEntity::vertexFromXMLNode( XMLNode* from )
{
    if( from->isEmpty() )
    {
        throw XMLException("vertex isn't presented");
    }

    Vertex v;

    XMLHelper::fromAttribute<float>(v.x, 0.0, from, XMLNames::ATTRIBUTES[VertexX], true);
    XMLHelper::fromAttribute<float>(v.y, 0.0, from, XMLNames::ATTRIBUTES[VertexY], true);
    XMLHelper::fromAttribute<float>(v.z, 0.0, from, XMLNames::ATTRIBUTES[VertexZ], true);

    return v;
}

/*
 * @param  XMLNode*
 * @param  Color&
 * @param  Color&
 * @param  Color$
 * @return Triangle
 * @brief  Convert XMLNode to Triangle. If triangle doesn't have declared any color explicitly then default from params is used.
 */
Triangle TriangleEntity::triangleFromXMLNode( XMLNode* from )
{
    Triangle tmpTr;
    XMLNode tmpVertex;

    /* At first try read 3 vertexes. All vertexes must by presneted */
    for( int i = 0; i < 3; i++)
    {	
        tmpVertex = from->getChildNode(i);

        try
        {
            tmpTr.vertex[i] = TriangleEntity::vertexFromXMLNode( &tmpVertex );
        }
        catch(const XMLException &)
        {	// if not then throw exception with message
            std::ostringstream oss;
            oss << "vertex " << i+1 << " isn't presented ";
            throw XMLException( oss.str() );
        }
    }

    /* Trying read colors. Each color is defined or default is used */
#ifndef NDEBUG
    if (  !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Emission], tmpTr.emission, 1 ))
#else
        if (  !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Emission], tmpTr.emission ))
#endif
        {	
            tmpTr.emission = this->emission_;
        }

#ifndef NDEBUG
    if (  !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Reflectivity], tmpTr.reflectivity, 1 ))
#else
        if (  !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Reflectivity], tmpTr.reflectivity ))
#endif
        {
            tmpTr.reflectivity = this->reflectivity_;
        }

#ifndef NDEBUG
    if ( !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Radiosity], tmpTr.radiosity, 1 ))
#else
        if ( !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Radiosity], tmpTr.radiosity ))
#endif
        {
            tmpTr.radiosity = this->radiosity_;
        }

    XMLHelper::fromAttribute<double>( tmpTr.spec, this->spec_, from, XMLNames::ATTRIBUTES[Spec], false );
    XMLHelper::fromAttribute<double>( tmpTr.refl, this->refl_, from, XMLNames::ATTRIBUTES[Refr], false );
    XMLHelper::fromAttribute<double>( tmpTr.refr, this->refr_, from, XMLNames::ATTRIBUTES[Refl], false );


    return tmpTr;
}


/**
 * @param  from
 * @brief  Create all triangles from XLNode
 */
void TriangleEntity::impl_deserialize (XMLNode *from ) {
    /* Set etntity name if presented */
    setName( from, "TriangleEntity" );

    XMLNodeChildIterator triangles(from); 		// create iterator used to read nodes in triangleset

    XMLNode* triangleNode = 0;				
    Triangle t;
    int tNum = 1;
    bool tnSwap = false;

    /* try convert all child nodes to triangles */
    while( 0 != ( triangleNode = triangles.next() ) )
    {
        try
        {
            XMLCSTR tagName = triangleNode->getName();

            if( !strcmp( tagName, XMLNames::TAGS[ TriangleNode ] ))
            {	// Child is triangle
                t = TriangleEntity::triangleFromXMLNode( triangleNode );
                tnSwap = false;
            }
            else if( !strcmp( tagName, XMLNames::TAGS[ Trianglenext ] ))
            {	// Childe is trianglenext
                if( tNum < 2 )
                {	// trianglenext musn't be first child because using vertexes from last triangle
                    std::cerr << "WARNING: node \"trianglenext\" can't be first child node in triangleset named " << getName() << std::endl;
                }
                else
                {
                    XMLNode tempNode = triangleNode->getChildNode(0);
                    Vertex lVertex = vertexFromXMLNode( &tempNode );	// read new vertex

                    if ( ( tnSwap = !tnSwap ) )
                    {
                        t.vertex[0] = t.vertex[1];			// Used last string like OpenGL 	
                        t.vertex[1] = lVertex;				// GL_TRIANGLES_QUADS
                    }
                    else
                    {
                        t.vertex[0] = t.vertex[2];			// Used last string like OpenGL 	
                        t.vertex[2] = lVertex;				// GL_TRIANGLES_QUADS
                    }
                }
            }
            else
            {
                std::cerr << "WARNING: not recognized node name \"" << tagName << "\" in triangleset named " << getName() << std::endl;
            }
        }
        catch(const XMLException &e)
        {
            std::ostringstream oss;
            oss << "Error: in triangleset " << "named " << getName() << " " << " in triangle " << tNum << " ";
            throw XMLException( oss.str() + e.what() );
        }

        tNum++;
        addTriangle( &t );
    }
}
