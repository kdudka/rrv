/*
 * @file XMLHelper.cpp
 * @brief XMLHelper implementation
 * @author Jakub Filak, filak.jakub@gmail.com
 * @date 2007-10-20
 */
#include "XMLHelper.h"
#include <iostream>
#include <cstring>
#include <sstream>

using namespace XML;
using namespace std;

/*
 * Intialize class members.
 */
void AbstractXMLNodeIterator::initialize( XMLNode* root )
{
	this->rootNode_ = root;
	this->currentNode_ = 0 == this->rootNode_ ? XMLNode::emptyXMLNode : *(this->rootNode_);
}

/*
 * Return pointer to current.
 */
XMLNode* AbstractXMLNodeIterator::getCurrentNode()
{
	return &(this->currentNode_);
}

/*
 * Set current to root node
 */
void AbstractXMLNodeIterator::reset()
{
	this->initialize( this->rootNode_ );
}

/*
 * At first check if rootNode isn't NULL.
 * Then take rootNode child and if child isn't empty then set current to child and return pointer to current.
 */
XMLNode* XMLNodeChildIterator::next()
{
	if ( 0 == this->rootNode_ ) throw new XMLException();				// not initialized

	if ( this->rootNode_->nChildNode() < this->nextChildNumber_ ) {
		return 0;		// all childrens was returned
	}

	this->currentNode_ = this->rootNode_->getChildNode( this->nextChildNumber_++ );	// take child and increment child counter

	return this->currentNode_.isEmpty() ? 0 : &(this->currentNode_);			// if isn't empty return pointer
}

/*
 * Initialize class members
 */
XMLNodeNextNodeIterator::XMLNodeNextNodeIterator( XMLNode* node )
{
	AbstractXMLNodeIterator::initialize( node );					// base initialize
	this->currentChildIterator_ = new XMLNodeChildIterator(this->rootNode_);
}

/*
 * Recursive calling this method while next node is taken or all nodes was read.
 */
XMLNode* XMLNodeNextNodeIterator::next()
{
	while( 1 )
	{
 		XMLNode* tmpNode = this->currentChildIterator_->next();				// try take next child
 	
 		if ( 0 != tmpNode )
 		{	// child go to be current
 			this->parentsIterators_.push( this->currentChildIterator_ );		// push current iterator
 			this->currentNode_ = *tmpNode;						// change current to new child
 			this->parents_.push( this->currentNode_ );				// push current to parents stack
 			this->currentChildIterator_ = new XMLNodeChildIterator( &(this->parents_.top()) );	// create new iterator
 			return tmpNode;
 		}
 		else
 		{	// current doesn't have any more childrens
 			if ( this->currentChildIterator_->getRootNode() != this->rootNode_ )	
 			{ 	
 				AbstractObserver* o = this->observs_[string(this->currentChildIterator_->getRootNode()->getName())];
 				if ( 0 != o ) o->respond();
 	
 				delete this->currentChildIterator_;					// delete his iterator
 	
 				this->currentChildIterator_ = this->parentsIterators_.top();		// take parent iterator
 				this->parentsIterators_.pop();						// pop
 	
 				this->parents_.pop();	// TODO Co se stane kdyz je prazdny.
 				//tmpNode = XMLNodeNextNodeIterator::next();
 			}
 			else 
			{	// every node in root was read
 				return 0;
			}
 		}
	}
}

/*
 * Reset state
 */
void XMLNodeNextNodeIterator::reset()
{
	this->clean();
	this->currentChildIterator_ = new XMLNodeChildIterator(this->rootNode_);
}

/*
 * Clean
 */
void XMLNodeNextNodeIterator::clean()
{
	delete this->currentChildIterator_;

	while( !this->parentsIterators_.empty() )
	{	// delete all iterators from stack
		delete this->parentsIterators_.top();
		this->parentsIterators_.pop();
	}
}

/*
 * Destructor
 */
XMLNodeNextNodeIterator::~XMLNodeNextNodeIterator()
{
	this->clean();
}

/*
 * At first function open file and parse it.
 */
void XMLHelper::loadFile( std::string filePath )
{
	this->thisFile_ = filePath;
	this->documentRoot_ = this->parseFile( filePath.c_str(), XMLNames::TAGS[ Root ] );
	this->documentHead_ = documentRoot_.getChildNode( XMLNames::TAGS[ Definition  ] );	// Take head, declarations
	this->documentBody_ = documentRoot_.getChildNode( XMLNames::TAGS[ Instantiate ]);	// Take body, transformations and objects

	if ( documentHead_.isEmpty()  || documentHead_.nChildNode() < 1 || this->documentBody_.isEmpty() || this->documentBody_.nChildNode() < 1 )
	{	// If file doesnt contain node head.
		throw XMLException( "Input file must contains node \"head\" and \"body\" with defined objects!");
	}

	this->loaded_ = true;
}

XMLNode* XMLHelper::getBody()
{
	if ( !this->loaded_ )
	{
		throw XMLException( "ERROR: can't return document body, file not loaded!" );
	}

	return &(this->documentBody_);
}

/**
 * @brief  Open file and parse it. If succed then return root node.
 */
XMLNode XMLHelper::parseFile( XMLCSTR filePath, XMLCSTR rootNodeName )
{
	XMLResults *parseRes = new XMLResults();						// Instance for parsing result
        XMLNode tmpRoot = XMLNode::parseFile( filePath, rootNodeName, parseRes ); 	// Calling library function, 
										// parseFile dont print any message, openFileHelper print many messages
	if ( 0 != parseRes->error )
	{	// If file contains erros.
		ostringstream oss;
		oss << XMLNode::getError( parseRes->error) << " on line: " << parseRes->nLine << string( oss.str() );
	
		delete parseRes;

		throw XMLException( oss.str() );
	}

	delete parseRes;

	return tmpRoot;
}

/**
 * @brief  Returns file root node. If file isn't open then parse try parse file and store his root in map.
 */
XMLNode XMLHelper::getFileRoot( XMLCSTR fileName, XMLCSTR rootName)
{	//TODO : add some inteligence like map
	std::string fNameStr(fileName);

	XMLNode frn;

	if( this->files_.find(fNameStr) == this->files_.end() )
	{
		frn = this->parseFile( fileName, rootName);
		this->files_[fNameStr] = frn;
	}
	else
	{
		frn = this->files_[fNameStr];
	}

	return frn;
}

/**
 * @param  fileName
 * @param  objName
 * @return XMLNode
 * @brief  Return objectdef from file. 
 */
XMLNode XMLHelper::getObjectdefFromFile( XMLCSTR fileName, XMLCSTR objName )
{
	if( strcmp( fileName, this->thisFile_.c_str() ) )
	{	// include object only if iobject isn't from same file
		/* Parse file and take head node and from head take definition of object */
		XMLNode frn = this->getFileRoot( fileName, XMLNames::TAGS[ Root ] );	
		return frn.getChildNode( XMLNames::TAGS[ Definition  ] ).getChildNodeWithAttribute( XMLNames::TAGS[ ObjectDefinition ], XMLNames::ATTRIBUTES[ Name ], objName );
	}

#ifndef NDEBUG
	std::cerr << "WARNING: can't include object from same file where including and object is declared" << std::endl;
#endif

	return XMLNode::emptyXMLNode;
}

/**
 * @param  Name
 * @return XMLNode
 * @brief  Return objectdef from actual file or return objecdef from included file
 */
XMLNode XMLHelper::getObjectdef( XMLCSTR name )
{
	XMLNode obj = this->documentHead_.getChildNodeWithAttribute( XMLNames::TAGS[ ObjectDefinition ], XMLNames::ATTRIBUTES[ Name ], name );

	if( !obj.isEmpty() && obj.isAttributeSet( XMLNames::ATTRIBUTES[ IncludeFile ]) )
	{	// if object has attribute "file" then is included from other file
		obj = this->getObjectdefFromFile( obj.getAttribute(XMLNames::ATTRIBUTES[ IncludeFile ]), name);
	}
	
	return obj;
}

/**
 * @param  Object name
 * @return XMLNode
 * @brief  Return object node with name. If object doesn't exists return emtpy node.
 */
XMLNode XMLHelper::getObject( XMLCSTR name )
{
	if ( !this->loaded_ )
	{
		throw XMLException( "ERROR: can't return document body, file not loaded!" );
	}

	return this->getObjectdef( name );
}
