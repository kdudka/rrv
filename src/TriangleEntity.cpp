#include "TriangleEntity.h"
#include <iostream>
#include <sstream>
#include <cstring>
//  
// Methods
//  



// Other methods
//  

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
Triangle TriangleEntity::triangleFromXMLNode( XMLNode* from, Color& defEm, Color& defRefl, Color& defRad )
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
		catch( XMLException )	
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
		tmpTr.emission = defEm;
	}
	
#ifndef NDEBUG
	if (  !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Reflectivity], tmpTr.reflectivity, 1 ))
#else
	if (  !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Reflectivity], tmpTr.reflectivity ))
#endif
	{
		tmpTr.reflectivity = defRefl;
	}
	
#ifndef NDEBUG
	if ( !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Radiosity], tmpTr.radiosity, 1 ))
#else
	if ( !Entity::colorFromXMLNode( from, XMLNames::ATTRIBUTES[Radiosity], tmpTr.radiosity ))
#endif
	{
		tmpTr.radiosity = defRad;
	}
	
        return tmpTr;
}


/**
 * @param  from
 * @brief  Create all triangles from XLNode
 */
void TriangleEntity::deserialize (XMLNode *from ) {
	/* Set etntity name if presented */
	setName( from, "TriangleEntity" );
	
	Entity::setColors( from, emission_, reflectivity_, radiosity_);
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
				t = TriangleEntity::triangleFromXMLNode( triangleNode, emission_, reflectivity_, radiosity_ );
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
		catch(XMLException e)
		{
			std::ostringstream oss;
			oss << "Error: in triangleset " << "named " << getName() << " " << " in triangle " << tNum << " ";
			throw XMLException( oss.str() + e.what() );
		}

		tNum++;
		addTriangle( &t );
	}
}
