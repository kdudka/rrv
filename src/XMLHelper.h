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

#ifndef XMLHELPER_H
#define XMLHELPER_H

/*
 * @file XMLHelper.h
 * @brief XMLHelper interface
 * @author Jakub Filak, filak.jakub@gmail.com
 * @date 2007-10-20
 */

#include <map>		// Import std::map
#include <string>	// Import std::string
#include <stack>	// Import std::stack
#include <exception>	// Include excpetions
#include <sstream>
#include <iostream>
#include "xmlParser.h" 	// Include xmlParser library
#include "XMLNames.h"

/*
 * @namespace XML
 * @brief XMLHelper namespace
 */
namespace XML {

	/*
	 * @brief Exception
	 */
	class XMLException: public std::exception 
	{
		public:
			/*
			* @brief Exception constructors
			 */
			XMLException() : desc_("General XMLException") {};

			XMLException( std::string description ) : desc_( description ) {};
			
			virtual const char* what() const throw() { return this->desc_.c_str(); }

			virtual ~XMLException () throw()  {};

		private: 
			std::string desc_;

	};

	/*
	 * @brief Base class for iteration XMLNodes
	 */
	class AbstractXMLNodeIterator
	{
		public:
			/*
			* @brief Return pointer to node form which iteration start.
			 */
			XMLNode* getRootNode() { return this->rootNode_; }

			/*
			* @brief Return pointer to current node.
			 */
			virtual XMLNode* getCurrentNode();

			/*
			* @brief Virtual method for move next
			 */
			virtual XMLNode* next() = 0;

			/*
			* @brief Virtual method for reset state of iterator
			 */
			virtual void reset();

			virtual ~AbstractXMLNodeIterator() {};

		protected:	
			
			/*
			* @brief Initialization method
			 */		
			void initialize( XMLNode* root=0 );
		
		        XMLNode currentNode_;
			XMLNode* rootNode_;
	};
	

	/*
	 * @brief Iterator class for iteration on child nodes.
	 */
	class XMLNodeChildIterator: public AbstractXMLNodeIterator
	{
		public:
			/**
			 * @param  XMLNode*
			 * @brief  Initialize class members
			 */
			XMLNodeChildIterator( XMLNode* node ) : nextChildNumber_(0)
			{
				initialize( node );
			}

			/*
			 * @brief  Change current node to next child of root node
			 */
			virtual XMLNode* next();

			/*
			 * @brief  Reset to start state
			 */
			virtual void reset() { AbstractXMLNodeIterator::reset(); this->nextChildNumber_ = 0; }

			/*
			 * @brief  Virtual destrucor
			 */
			virtual ~XMLNodeChildIterator() {};
			
		private:
			int nextChildNumber_;
	};	

	/*
	 * @brief Observer interface
	 */
	class AbstractObserver
	{
		public:
			/*
			 * @brief Method called to observe
			 */
			virtual void respond() { return; }
			
			/*
			 * @brief Virtual destructor
			 */
			virtual ~AbstractObserver() {};
	};

	/*
	 * @brief Iterator class used for iteration to next nodes
	 */
	class XMLNodeNextNodeIterator: public AbstractXMLNodeIterator
	{
		public:
			XMLNodeNextNodeIterator( XMLNode* node );

			/*
			 * @return XMLNode*
			 * @brief  Return nex node
			 */
			virtual XMLNode* next();

			/*
			 * @brief  Reset state to start state
			 */
			virtual void reset();

			/*
			 * @param observer
			 * @param node name
			 * @brief Add observ to finish tag with specified name
			 */
			void addObservs(std::string nodeName, AbstractObserver* obs  ) { this->observs_[nodeName] = obs; } // NEVOLAT DVAKRAT !!!

			/*
			 * @brief  Virtual destructor
			 */
			virtual	~XMLNodeNextNodeIterator();

		private:
			/*
			 * @brief  Clean work space	
			 */
			void clean();

			std::map<std::string, AbstractObserver*> observs_;
			std::stack<XMLNode> parents_;
			std::stack<XMLNodeChildIterator*> parentsIterators_;
			XMLNodeChildIterator* currentChildIterator_;
	};

	/*
	 * @brief Class xmlHelper
	 */
	class XMLHelper
	{
		public:
			/**
			 * @brief  Constructor, initialize members.
			 */
			XMLHelper() : loaded_(false) 
			{}

			/**
			 * @param  string
			 * @brief  Read and parse file.
			 */
			void loadFile( std::string filePath );

			/**
			 * @return XMLNode*
			 * @biref  Return node with name body. This node contains transformations and obejcts.
			 */
			XMLNode* getBody();

			/*
			 * @param  const char*
			 * @brief  Returs node of object defined in head of file.
			 */
			XMLNode getObject( XMLCSTR name );
			
			/*
			 * @param  T
			 * @param  T
			 * @param  XMLNode*
			 * @param  const char*
			 * @param  bool
			 * @return bool
			 * @brief  Template function for reading attributes values from xml node. If value can;t be read from node then default is used.
			 */
			template <class T> static bool fromAttribute(T& t, T defaultValue, XMLNode* node, XMLCSTR attName, bool required )
			{	
				bool crash = true;

				XMLCSTR val = node->getAttribute(attName);

				if ( !( crash = !(0 != val) ) )
				{	// if value is 0 then value not read
					try 
					{
						std::string strVal = val;
						std::istringstream iss(strVal);
						crash = ( iss >> t ).fail();
					}
					catch(...)
					{	// something bad happend
						if ( required ) 
							throw XMLException(
								"Fail while converting XML element attribute value!");
					}

					if( crash && required )
					{	// can't convert value to type
						throw XMLException("Fail converting attribute! Bad format!");
					}
				} 
				else
				{	
					if ( required ) 
					{	// attribute doesn't exist
						throw XMLException(
						"Fail before converting attribute! Attribute NOT SETTED !");
					}
				}

				if ( crash )
				{	// print warning message and use default value
#ifndef NDEBUG
					std::cout << "WARNING: \""
						<< node->getName() << "\" doesn't have attribute \"" << attName
						<<  "\", using default value = " 
						<< defaultValue << std::endl;
#endif
					t = defaultValue;
				}

				return crash;
			}

			private:
				/**
				 * @param  fileName
				 * @param  rootNodeName
				 * @return XMLNode
				 * @brief  Open and parse file. After succesfull parsing returns file root
				 */
				XMLNode parseFile( XMLCSTR filePath, XMLCSTR rootNodeName );

				/**
				 * @param  fileName
				 * @param  rootName
				 * @return XMLNode
				 * @brief  Return the root node in file
				 */
				XMLNode getFileRoot( XMLCSTR fileName, XMLCSTR rootName );

				/**
				 * @param  name
				 * @return XMLNode
				 * @brief  Return objectdef with specified name
				 */
				XMLNode getObjectdef( XMLCSTR name );
				
				/**
				 * @param  fileName
				 * @param  objName
				 * @return XMLNode
				 * @brief  Returns objecdef with specified name from file
				 */
				XMLNode getObjectdefFromFile( XMLCSTR fileName, XMLCSTR objName );

				std::map< std::string, XMLNode> files_;
				
				std::string thisFile_;
				XMLNode documentRoot_;
				XMLNode documentHead_;
				XMLNode documentBody_;
				
				bool loaded_;
	};
}
#endif
