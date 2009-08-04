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
