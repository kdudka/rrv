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

#include "XMLWriter.h"
#include "XMLNames.h"
#include <iostream>
#include <cstring>

using namespace XML;

/**
 * @param  const char*
 * @return char*
 * @brief  Copy string. Using malloc.
 */ 
XMLSTR XMLWriter::copyString( XMLCSTR from )
{
	XMLSTR copy = 0;

	if ( 0 != from )
	{
		if ( 0 != (copy = (XMLSTR)malloc( sizeof(XMLCHAR) * (strlen(from) + 1) )))
		{
			strcpy(copy, from);
		}
	}

	return copy;
}

/**
 * @brief  Construct creates XML nodes in our DTD.
 */
XMLWriter::XMLWriter()
{
	this->objectName = "scene";
	this->root_ = XMLNode::createXMLTopNode_WOSD( XMLWriter::copyString( XMLNames::TAGS[Root] ) );
	this->definition_ = this->root_.addChild_WOSD( XMLWriter::copyString( XMLNames::TAGS[Definition] ) );
	this->instantiate_ = this->root_.addChild_WOSD(XMLWriter::copyString( XMLNames::TAGS[Instantiate] ) );

	this->objectdef_ = this->definition_.addChild_WOSD( XMLWriter::copyString( XMLNames::TAGS[ObjectDefinition] ) );
	this->objectdef_.addAttribute_WOSD( XMLWriter::copyString( XMLNames::ATTRIBUTES[Name] ) , XMLWriter::copyString( objectName ) );

	this->object_ = this->instantiate_.addChild_WOSD(XMLWriter::copyString( XMLNames::TAGS[Object] ) );
	this->object_.addAttribute_WOSD( XMLWriter::copyString( XMLNames::ATTRIBUTES[Name] ) , XMLWriter::copyString( objectName ) );
}

/**
 * @param  XMLNode
 * @brief  Add entity to objectdef
 */
void XMLWriter::addEntity( XMLNode addedNode )
{
	this->objectdef_.addChild( addedNode );
}

/**
 * @param  const char*
 * @brief  Write data to file
 */
void XMLWriter::writeToFile( const char* fileName )
{
	this->root_.writeToFile( fileName );
}
