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

#include "ArgumentParser.h"
#include <exception>
#include <iomanip>

using namespace std;

// Constructors/Destructors
//

ArgumentParser::ArgumentParser ( )
{
    this->description_ = 0;
    this->version_ = 0;
    this->setDelimiter("--");
    this->helpC_="help";
    this->addArgument( this->helpC_, "Show this help", false, 0);
    this->versC_="version";
    this->addArgument( this->versC_, "Show program version", false, 0);
}

ArgumentParser::~ArgumentParser ( )
{
    this->clean();
}

/**
 * Set the value of delimiter_
 * Arguments delimiter.
 * @param new_var the new value of m_delimiter_
 */
void ArgumentParser::setDelimiter ( const char* new_var ) {
    this->delimiter_ = this->copyString( new_var );
}

/**
 * Get the value of delimiter_
 * Arguments delimiter.
 * @return the value of delimiter_
 */
char* ArgumentParser::getDelimiter ( ) {
    return this->delimiter_;
}

/**
 * Parse command line arguments
 * @return bool
 * @param  argc Argument count
 * @param  argv Array of arguments.
 */
bool ArgumentParser::parseArguments (int argc, char** argv )
{
    if( 1 >= argc )	showError( "Too few arguments" );	/// Show help and exit program

    unsigned int deliLen = strlen( this->delimiter_ );
    std::string strtmp;

    for ( int i = 1; i < argc ; i+=2)
    {
        if( strncmp( argv[i], this->delimiter_, deliLen ) || strlen( argv[i] ) < deliLen+1 )
        {
            const char* parts[] = {"Bad argument format ", argv[i]};
            showError( StringContainer( parts, parts+2 ) );
        }
        else if ( !strcmp( argv[i]+deliLen, this->helpC_ ) )
        {
            showHelp();
        }
        else if ( !strcmp( argv[i]+deliLen, this->versC_ ) )
        {
            showVersion(true);
        }
        else if ( i >= argc-1 )
        {	
            const char* parts[] = {"Missing value for argument ", argv[i]};
            showError( StringContainer( parts, parts+2 )  );
        }
        else if ( !strncmp( argv[i+1], this->delimiter_, 2 ) )
        {	// argument name must contain delimiter and next argument mustnot contain delimiter
            const char* parts[] = {"Bad argument value format ", argv[i]};
            showError( StringContainer( parts, parts+2 )  );
        }

        strtmp = std::string( argv[i]+strlen(this->delimiter_) );

        if ( this->arguments_.find( strtmp ) == this->arguments_.end() )
        {
            const char* parts[] = { "Unknow attribute ",  argv[i]};
            showError( StringContainer( parts, parts+2) );
        }

        if ( 0 != this->arguments_[ strtmp ] )
        {
            free(this->arguments_[ strtmp ]);
            this->arguments_[ strtmp ]=0;
        }

        this->arguments_[ strtmp ] = this->copyString( argv[ i+1 ] );
        this->requireds_[ strtmp ] = false;
    }

    for ( map<string,bool>::iterator iter = this->requireds_.begin(); iter != this->requireds_.end(); iter++ )
    {
        if( iter->second )
        {
            const char* parts[] = { "Missing required arguments", iter->first.c_str() };
            showError( StringContainer( parts, parts+2 ) );
        }
    }

    return true;
}


/**
 * Return char*
 * @return char*
 * @param  name Argument name
 */
char* ArgumentParser::getCstringValue ( string name )
{
    return this->arguments_[ name ];
}


/**
 * Set program description
 * @param  description Program description
 */
void ArgumentParser::setDescription (const char* description )
{
    if( 0 != this->description_ )
    {
        free(this->description_);
        this->description_ = 0;
    }

    this->description_ = this->copyString( description );
}


/**
 * Show help
 * @param  custMessage Custom message to show on the top of help
 */
void ArgumentParser::showHelp ( bool quit, const char* custMessage )
{
    if ( 0 != custMessage ) cout << custMessage << endl << endl;

    if ( 0 != this->description_ ) cout << this->description_ << endl;

    cout << "Version:\t";
    showVersion();

    cout << "Parameters: " << endl;

    for ( map<std::string,char*>::iterator iter = descriptions_.begin(); iter != descriptions_.end(); iter++ )
    {
        cout << setw(4) << this->delimiter_ << setw(12) << left << iter->first << iter->second << right << endl;
    }

    showHelpAppendix();

    if ( quit )
    {
        this->clean();
        exit(1);
    }
}

/**
 * Show error
 */
void ArgumentParser::showError( const char* message )
{
    this->showError( StringContainer( 1, message ) );
}

/**
 * Show error message and quit
 */
void ArgumentParser::showError( StringContainer parts )
{
    for( StringContainer::iterator piter = parts.begin(); piter != parts.end(); piter++ )
    {
        std::cerr << *piter;
    }

    std::cerr << std::endl << "For more help use argument " << this->delimiter_ << this->helpC_ << std::endl;

    this->clean();
    exit( 1 );
}

/**
 * Set program version
 */
void ArgumentParser::setVersion( const char* version )
{
    if( 0 != this->version_ )
    {
        free(this->version_);
        this->version_ = 0;
    }

    this->version_ = this->copyString( version );
}

/**
 * Show program version
 */
void ArgumentParser::showVersion( bool quit )
{
    if( 0 != this->version_ )
    {
        std::cout << this->version_ << std::endl;
    }
    else
    {
        std::cerr << "Error: version string isn't set" << std::endl;
    }

    if( quit )
    {
        this->clean();
        exit(1);
    }
}

/**
 * Add argument
 * @param  name Argument name
 * @param  description Argument description
 * @param  required True if argument is required
 * @param  def Default value like cstring ( char* )
 */
void ArgumentParser::addArgument (const char* name, const char* description, bool required, const char* def )
{
    string nameStr = string( name );

    this->arguments_[ nameStr ] = this->copyString( def );
    this->descriptions_[ nameStr ] = this->copyString( description );

    if( required )
    {
        this->requireds_[ nameStr ] = true;
    }
}


/**
 * Returns copy of cstring.
 * @return char*
 * @param  original Cstring to copy.
 */
char* ArgumentParser::copyString (const char* original )
{	
    char* copy = 0;

    if ( 0 != original )
    {
        if ( 0 != (copy = (char*)malloc( sizeof(char) * (strlen(original) + 1) )))
        {
            strcpy(copy, original);
        }
    }

    return copy;
}

/**
 * Free map
 * @param  map Map for free.
 */
void ArgumentParser::freeMap ( map<std::string, char*> *map )
{
    for ( std::map<std::string, char*>::iterator iter = (*map).begin(); iter != (*map).end(); iter++ )
    {
        free( iter->second );
        iter->second = 0;
    }
}

/**
 * Free malloced memory.
 */
void ArgumentParser::clean ( )
{
    free(this->description_);
    this->description_ = 0;
    free(this->version_);
    this->version_ = 0;
    free(this->delimiter_);
    this->delimiter_ = 0;

    this->freeMap( &(this->arguments_) );
    this->freeMap( &(this->descriptions_) );
}
