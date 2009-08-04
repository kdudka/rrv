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

#include <iostream>
#include <iomanip>
#include "VisualizeArguments.h"

#define PARAMETRS 3
#define KEYNUM 1
using namespace std;

// Constructors/Destructors
//

const char* VisualizeArguments::NAMES[PARAMETRS]  = { "filein",
						      "screenshot",
						      "fileout",};
							


const char* VisualizeArguments::descripts_[PARAMETRS]= { "Input file name, REQUIRED",
							 "Save screenshot",
							 "File to save screenshot", };

const bool  VisualizeArguments::reqs_[PARAMETRS]= { true,
						    false,
						    false,};

const char* VisualizeArguments::defaults_[PARAMETRS] = { "",
							 "0",
							 "screenshot.tga",};

const char* VisualizeArguments::keys_[KEYNUM] = { "M", };

const char* VisualizeArguments::kdesc_[KEYNUM] = { "model from xml, reflectivity is used in color property", };

VisualizeArguments::VisualizeArguments ( )
{
	initialize();
}

VisualizeArguments::~VisualizeArguments ( ) { }

/**
 * @return char*
 */
char* VisualizeArguments::getFileIn ( )
{
	return getCstringValue( string( NAMES[ 0 ] ) );
}

bool VisualizeArguments::getScreenshot( )
{
	bool result = true;
        char* val = getCstringValue( NAMES[ 1 ] );

        if( !strcmp( val, "1" ) || !strcmp( val, "on") || !strcmp( val, "true") )
        {
                result = true;
        }
        else if ( !strcmp( val, "0") || !strcmp( val, "off") || !strcmp( val, "false") )
        {
                result = false;
        }
        else
        {
                std::cerr << "Bad Screenshot value. Usable values is :  0 1 || on off || true false" << endl;
        }

        return result;
}

/**
 * @return char*
 */
char* VisualizeArguments::getScreenshotFile ( )
{
	return getCstringValue( string( NAMES[ 2 ] ) );
}


/**
 * Initialize members
 */
void VisualizeArguments::initialize ( )
{
	this->setDescription("Visualize computed or uncomputed scene.\nAuthors:\tKamil Dudka, Lukas Hefka, David Barina, Jakub Filak\nDate:\t\t2007-11-19");
	this->setVersion("unknow");

	for( int i = 0; i < PARAMETRS; i++ )
	{
		this->addArgument( NAMES[i], descripts_[i], reqs_[i], defaults_[i] );
	}
}

/**
 * Show help for controls
 */
void VisualizeArguments::showHelpAppendix()
{
	cout << "Controls: " << endl;

	for( int i = 0; i < KEYNUM; i++ )
	{
		cout << "  " << setw(10) << left << keys_[i] << kdesc_[i] << endl;
	}
}
