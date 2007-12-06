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
