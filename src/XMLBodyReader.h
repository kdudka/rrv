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

#ifndef XMLBODYREADER_H
#define XMLBODYREADER_H

/**
 * @file XMLBodyReader.h
 * @brief XMLBodyReader interface
 * @author Jakub Filak, filak.jakub@gmail.com
 * @date 2007-10-23
 */
#include "XMLHelper.h"
#include "XMLNames.h"
#include "TransformMatrix.h"
#include <stack>

using std::stack;
using std::string;

namespace XML
{
	/*
	 * Read body from xml file.
	 * On next returs only entity nodes.
	 */
	class XMLBodyReader: public XMLNodeNextNodeIterator, AbstractObserver
	{
		public:
			XMLBodyReader( XMLHelper* helper, XMLNode* node ) : XMLNodeNextNodeIterator( node )
			{	
				this->addObservs( string( XMLNames::TAGS[ Rotate    ] ), this );		// register callbac function for matrix end
				this->addObservs( string( XMLNames::TAGS[ Scale     ] ), this );		// register callbac function for matrix end
				this->addObservs( string( XMLNames::TAGS[ Translate ] ), this );		// register callbac function for matrix end
				this->addObservs( string( XMLNames::TAGS[ Shear     ] ), this );		// register callbac function for matrix end
				
				this->object_ = XMLNode::emptyXMLNode;
				this->objectIterator_ = new XMLNodeChildIterator( &(this->object_) );		// initialize to empty node
				this->matrixes_.push( TransformMatrix() );		// intialize with standart matrix
				this->helper_ = helper;
			}	

			/*
			 * Return top of transform matrixies stack.
			 * It is an actual matrix which is used to transform objects.
			 */
			TransformMatrix* getMatrix() { return &(this->matrixes_.top()); }

			/*
			 * Method reads node body and return entity from objects whose are in body.
			 * Doesn't return transform matrix.
			 */
			virtual XMLNode* next();

			virtual ~XMLBodyReader() { delete this->objectIterator_; }

		private:
			/***************/
			/*** Methods ***/

			virtual void respond() { this->matrixEnd(); }

			/*
			 * Removes matrix from top of matrixies stack when became inactive.
			 */
			void matrixEnd(void) { this->matrixes_.pop(); }
				
			/*
			 * Create TransformMatrix from XML node.
			 */
			static bool changeMatrixByXMLNode( XMLNode* from, TransformMatrix &matrix );

			/*****************/
			/*** Variables ***/

			XMLHelper *helper_;
			stack<TransformMatrix> matrixes_;
			XMLNode object_;
			XMLNodeChildIterator* objectIterator_;
	};
}
#endif
