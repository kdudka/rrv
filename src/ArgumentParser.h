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

#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

/**
 * @file ArgumentParser.h
 * @brief Class ArgumentParser - generic command-line arguments parser.
 * @author xfilak01
 * @date 2007-11-28
 */

#include <iostream>
#include <string>
#include <cstdlib> 
#include <sstream>
#include <map>
#include <vector>

/**
 * @brief Generic command-line arguments parser.
 */
class ArgumentParser
{
public:

	typedef std::vector<std::string> StringContainer;

	/**
	 * Empty Constructor
	 */
	ArgumentParser ( );

	/**
	 * Empty Destructor
	 */
	virtual ~ArgumentParser ( );

	/**
	 * Parse command line arguments
	 * @return bool
	 * @param  argc Argument count
	 * @param  argv Array of arguments.
	 */
	bool parseArguments (int argc, char** argv );

	/**
	 * Return char*
	 * @return char*
	 * @param  name Argument name
	 */
	char* getCstringValue (std::string name );

	/**
	 * Set program description
	 * @param  description Program description
	 */
	void setDescription (const char* description );

	/**
	 * Set program version
	 * @param version String describing program version
	 */
	void setVersion( const char* version );

	/**
	 * Template for prasing cstrings.
	 * @return T
	 * @param  name Argument name.
	 */
	template <class T> T getValue( std::string name )
	{
	        T tmp;
		std::istringstream iss( this->getCstringValue( name ) );

	        if ( ( iss >> tmp ).fail() )
	        {       // can't convert value to type
			showHelp( true, "Bad argument value type" );
	        }

	        return tmp;
	}

/**
	 * Free malloced memory.
	 */
	void clean ( );

protected:
	/**
	 * Show help
	 * @param  quit if is true then function call exit(0) on end
	 * @param  custMessage Custom message to show on the top of help
	 */
	virtual void showHelp ( bool quit=true, const char* custMessage=0 );

	/**
	 * This method is called after help is showed.
	 */
	virtual void showHelpAppendix ()=0;
	
	/**
	 * Show help message and quit
	 * @param  parts vector containing parts of error message
	 */
	void showError( StringContainer	parts );

	/**
	 * Show help message and quit
	 * @param  message error message
	 */
	void showError( const char* message );

	/**
	 * Show version
	 * @param  quit if is true then function call exit(1) on end
	 */
	void showVersion( bool quit=false );

	/**
	 * Set the value of delimiter_
	 * Arguments delimiter.
	 * @param new_var the new value of m_delimiter_
	 */
	void setDelimiter( const char* new_var );

	/**
	 * Get the value of delimiter_
	 * Arguments delimiter.
	 * @return the value of m_delimiter_
	 */
	char* getDelimiter( );

	/**
	 * Add argument
	 * @param  name Argument name
	 * @param  description Argument description
	 * @param  required True if argument is required
	 * @param  def Default value like cstring ( char* )
	 */
	void addArgument (const char* name, const char* description, bool required, const char* def);

private:

	const char* helpC_;
	const char* versC_;
	// Arguments delimiter.
	char* delimiter_;
	// Program description, used for help
	char* description_;
	// Program version
	char* version_;
	// Arguments from command line. Values mapped to names.	
	std::map<std::string,char*> arguments_;
	// Argument descriptions mapped to names.
	std::map<std::string,char*> descriptions_;
	// If argument is required there is true mapped to his name.
	std::map<std::string,bool> requireds_;
	std::map<unsigned long,bool> malloced_;

	/**
	 * Returns copy of cstring.
	 * @return char*
	 * @param  original Cstring to copy.
	 */
	char* copyString (const char* original );

	/**
	 * Returns copy of cstring.
	 * @return char*
	 * @param  original Cstring to copy.
	 */
	char* copyStringDesc (const char* original );

	/**
	 * Free map
	 * @param  map Map for free.
	 */
	void freeMap (std::map<std::string,char*>* map );

	void myFree( void** pointer );

	/**
	 * Free malloced memory.
	 */
	//void clean ( );
};

#endif // ARGUMENTPARSER_H
