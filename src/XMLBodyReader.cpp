/*
 * @file XMLBodyReader.cpp
 * @brief XMLBodyReader implementation
 * @author Jakub Filak, filak.jakub@gmail.com
 * @date 2007-10-22
 */


#include "XMLBodyReader.h"
#include <string>
#include <iostream>
#include <cstring>

using namespace XML;

/*
 * TODO: Comments
 */
bool XMLBodyReader::changeMatrixByXMLNode( XMLNode* from, TransformMatrix &matrix )
{
	SpatialVector v;

	XMLHelper::fromAttribute<float>(v.dx, 0.0, from, XMLNames::ATTRIBUTES[ TransformationX ], false);
	XMLHelper::fromAttribute<float>(v.dy, 0.0, from, XMLNames::ATTRIBUTES[ TransformationY ], false);
	XMLHelper::fromAttribute<float>(v.dz, 0.0, from, XMLNames::ATTRIBUTES[ TransformationZ ], false);

	bool succes = true;

	string operation(from->getName());

	if( string( XMLNames::TAGS[ Rotate ] ) == operation )
	{
		float angle;
		XMLHelper::fromAttribute<float>(angle, 0.0, from, XMLNames::ATTRIBUTES[ RotateAngle ], false);
		matrix.rotate(angle, v);
	}
	else
	if( string( XMLNames::TAGS[ Scale ] ) == operation )
	{
		matrix.scale(v);
	}
	else
	if( string( XMLNames::TAGS[ Translate ] ) == operation )
	{
		matrix.translate(v);
	}
	else
	if( string( XMLNames::TAGS[ Shear ] ) == operation )
	{
		matrix.shear(v);
	}
	else
	{
		std::cerr << "Unrecognized transformation: " << from->getName() << std::endl;
		succes = false;
	}	

	return succes;
}

/**
 * TODO: Comments
 */
XMLNode* XMLBodyReader::next()
{		
	XMLNode* nextEntity = this->objectIterator_->next();
	
	while( 0 == nextEntity )
	{
		XMLNode* nextNode = XMLNodeNextNodeIterator::next();

		if( 0 == nextNode )
		{	// if base iterator finish then this iterator finish too
			// std::cerr << "XMLBodyReader::next() finished" << std::endl;
			return 0;
		}

		if ( !strcmp( XMLNames::TAGS[ Object ], nextNode->getName() ) )
		{	// if next read node has name object
			
			// try take attribute name	
			XMLCSTR name = nextNode->getAttribute( XMLNames::ATTRIBUTES[ Name ] );

			if ( 0 != name )
			{	// try take object definition node
				this->object_ = this->helper_->getObject( name );
	
				if( !this->object_.isEmpty() )
				{
					delete this->objectIterator_;
					this->objectIterator_ = new XMLNodeChildIterator( &(this->object_) );
					nextEntity = this->objectIterator_->next();
				}
				else
				{
					std::cerr << "WARNING: Object with name \"" << name << "\" wasn't declared" << std::endl;
				}
			}
			else
			{
				std::cerr << "WARNING: Object doesn't have attribute name. Entity cannot be created." << std::endl;
			}
		}
		else
		{	
			TransformMatrix tm = this->matrixes_.top();

			if ( XMLBodyReader::changeMatrixByXMLNode( nextNode, tm) )
			{	// if changing matrix succed then save matrix on stack
				// mustn't push matrix on top of stack when failure, 
				// because this matrix hasn't been removed from stack in future, 
				// observe isn't registered for bad node name
				this->matrixes_.push(tm);
			}
		}
	}

	return nextEntity;
}

