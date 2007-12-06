#ifndef COMPUTEARGUMENTS_H
#define COMPUTEARGUMENTS_H

/**
 * @file ComputeArguments.h
 * @brief Class ComputeArguments - Command-line arguments parser for compute binary.
 * @author xfilak01
 * @date 2007-11-28
 */

#include "ArgumentParser.h"

enum ComputeArgumentsNames 
{
	ArgFileIn,
	ArgFileOut,
	ArgTreshold,
	ArgCache,
	ArgDivide,
	ArgSteps,
	ArgSaves,
	ArgNormalize
};

/**
 * @brief Command-line arguments parser for compute binary.
 */
class ComputeArguments : public ArgumentParser
{
public:

	// Constructors/Destructors
	//  


	/**
	 * Empty Constructor
	 */
	ComputeArguments ( );

	/**
	 * Empty Destructor
	 */
	virtual ~ComputeArguments ( );

	// Static public attributes
	//  

	static const char* NAMES[];

	// public attribute accessor methods
	//  

	/**
	 * @return float
	 */
	float getTreshold ( );


	/**
	 * @return float
	 */
	float getDivide ( );


	/**
	 * @return int
	 */
	int getSteps ( );


	/**
	 * @return int
	 */
	int getSaves ( );


	/**
	 * @return long
	 */
	long getCache ( );


	/**
	 * @return char*
	 */
	char* getFileIn ( );


	/**
	 * @return char*
	 */
	char* getFileOut ( );


	/**
	 * @return bool
	 */
	bool getNormalize ( );

private:
	/**
	 * Show help for controls
	 */
	virtual void showHelpAppendix() {return;}
	// private attributes
	//  

	static const char* descripts_[];
	static const bool reqs_[];
	static const char* defaults_[];

	/**
	 * Initialize object.
	 */
	void initialize ( );
};

#endif // COMPUTEARGUMENTS_H
