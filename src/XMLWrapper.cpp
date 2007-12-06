#include "XMLWrapper.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

/**
 * @param  node
 * @param  required
 * @author Jakub Filak
 * @date   2007-10-25
 */

using namespace std;
using namespace XML;

/**
 * @brief  Constructor initialize members
 */
XMLWrapper::XMLWrapper()
{
	this->xmlHelper_ = 0;
	this->xmlBodyReader_ = 0;
}

/**
 * @param  fileName
 * @brief  Load and parse file
 */
void XMLWrapper::loadFile( std::string fileName ) 
{
	this->xmlHelper_ = new XMLHelper(); 
	this->xmlHelper_->loadFile( fileName );
	this->xmlBodyReader_ = new XMLBodyReader( this->xmlHelper_, this->xmlHelper_->getBody() );
}

/**
 * @return XMLNode*
 * @brief  Return next entity node
 */
XMLNode* XMLWrapper::nextEntityNode() 
{ 
	if ( 0 == this->xmlBodyReader_ )
	{
		throw XMLException( "ERROR: Probalby file not loaded, can't return next entity" );
	}

	return this->xmlBodyReader_->next();
}

/**
 * @return TransformMatrix*
 * @brief  Return actual transformation matrix
 */
TransformMatrix* XMLWrapper::getTransformMatrix()
{
	if ( 0 == this->xmlBodyReader_ )
	{
		throw XMLException( "ERROR: Probalby file not loaded, can't return next entity" );
	}

	return this->xmlBodyReader_->getMatrix();
}

/**
 * @brief  Destructor clean space
 */
XMLWrapper::~XMLWrapper()
{
	if ( 0 != this->xmlHelper_ )
	{
		delete this->xmlHelper_;
	}

	if ( 0 != this->xmlBodyReader_ )
	{
		delete this->xmlBodyReader_;
	}
}
