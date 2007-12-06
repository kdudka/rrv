#ifndef VISUALIZEARGUMENTS_H
#define VISUALIZEARGUMENTS_H

/**
 * @file VisualizeArguments.h
 * @brief Class VisualizeArguments - Command-line arguments parser for visualize binary.
 * @author xfilak01
 * @date 2007-11-28
 */

#include "ArgumentParser.h"
#include <string>

/**
 * @brief Command-line arguments parser for visualize binary.
 */
class VisualizeArguments : virtual public ArgumentParser
{
public:
	/**
	 * Empty Constructor
	 */
	VisualizeArguments ( );

	/**
	 * Empty Destructor
	 */
	virtual ~VisualizeArguments ( );

	// Names of usable arguments
	static const char* NAMES[];
	
	/**
	 * @return char*
	 */
	char* getFileIn ( );
	
	/**
	 * @return char*
	 */
	bool getScreenshot ( );

	/**
	 * @return char*
	 */
	char* getScreenshotFile ( );

private:
	// Descriptions of arguments
	static const char* descripts_[];
	// Arguments requiration
	static const bool reqs_[];
	// Default values of arguments
	static const char* defaults_[];
	// Keys value
	static const char* keys_[];
	// Keys description
	static const char* kdesc_[];

	/**
	 * Initialize members
	 */
	void initialize ( );

	/**
	 * Show help for controls
	 */
	virtual void showHelpAppendix();
};

#endif // VISUALIZEARGUMENTS_H
