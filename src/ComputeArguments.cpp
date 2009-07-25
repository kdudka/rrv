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

#include "ComputeArguments.h"
#define PARAMETRS 8
using namespace std;

// Constructors/Destructors
//  

const char* ComputeArguments::NAMES[PARAMETRS] 	  = {	"filein",
						"fileout",
						"treshold",
						"cache",
						"divide",
						"steps",
						"saves",
						"normalize",
						}; 

const char* ComputeArguments::descripts_[PARAMETRS]= {	"Input file name, REQUIRED",
						"Output file name, default is output.xml",
						"Form factor treshold value, default is 0.0",
						"Form factor cache size in MiB, default is 1024",
						"Size of largest acceptable patch after patch division, default is 1.0",
						"Number of steps, default is 32",
						"Number of continous saves in 1st step, default is 8",
						"'on' lightness is normalized, 'off' ligthness is truncated, default is 'on'",
						 };

const bool  ComputeArguments::reqs_[PARAMETRS]= { 	true,
						false,
						false,
						false,
						false,
						false,
						false,
						false,
						};

const char* ComputeArguments::defaults_[PARAMETRS] = { 	"",
						"output.xml",
						"0.0",
						"1024",
						"1",
						"32",
						"8",
						"1",
						};

ComputeArguments::ComputeArguments ( ) 
{
	this->initialize();
}

ComputeArguments::~ComputeArguments ( ) { }

// Other methods
//  


/**
 * @return float
 */
float ComputeArguments::getTreshold ( ) 
{
	float res = getValue<float>( string( NAMES[ ArgTreshold ] ) );

	if( 0 > res )
	{
		std::cerr << "Treshold cant be less than 0" << endl;
		return 0.0;
	}

	return res;
}


/**
 * @return float
 */
float ComputeArguments::getDivide ( ) 
{
	float res = getValue<float>( string( NAMES[ ArgDivide ] ) );
	
	if( 0 >= res )
	{
		std::cerr << "Divide must be greater than 0" << endl;
		return 1.0;
	}

	return res;
}


/**
 * @return int
 */
int ComputeArguments::getSteps ( ) 
{
	int res = getValue<int>( string( NAMES[ ArgSteps ] ) );
	
	if( 0 >= res )
	{
		std::cerr << "Steps must be greater than 0" << endl;
		return 32;
	}

	return res;
}


/**
 * @return int
 */
int ComputeArguments::getSaves ( ) 
{
	int res = getValue<int>( string( NAMES[ ArgSaves ] ) ); 

	if( 0 > res )
	{
		std::cerr << "Saves cant be less than 0" << endl;
		return 8;
	}

	return res;
}


/**
 * @return long
 */
long ComputeArguments::getCache ( ) 
{
	long res = getValue<long>( string( NAMES[ ArgCache ] ) );

 	if( 0 > res ) 
	{
		std::cerr << "Cache cant be less than 0" << endl; 
		return 1024l;
	}
	
	return res;
}


/**
 * @return char*
 */
char* ComputeArguments::getFileIn ( ) 
{
	return getCstringValue( string( NAMES[ ArgFileIn ] ) );
}


/**
 * @return char*
 */
char* ComputeArguments::getFileOut ( ) 
{
	return getCstringValue( string( NAMES[ ArgFileOut ] ) );
}


/**
 * @return bool
 */
bool ComputeArguments::getNormalize ( ) 
{
	bool result = true;
	char* val = getCstringValue( NAMES[ ArgNormalize ] );

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
		std::cerr << "Bad Noramlize value. Usable values is :  0 1 || on off || true false" << endl;
	}

	return result;
}


/**
 * Initialize object.
 */
void ComputeArguments::initialize ( ) 
{
	this->setDescription("Compute global ilimuniation of scene.\nAuthors:\tKamil Dudka, Lukas Hefka, David Barina, Jakub Filak\nDate:\t\t2007-11-19");
	this->setVersion("unknow");

	for( int i = 0; i < PARAMETRS; i++ )
	{
		this->addArgument( NAMES[i], descripts_[i], reqs_[i], defaults_[i] );
	}
}


