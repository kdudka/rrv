#ifndef CONSOLEPROGRESSINDICATOR_H
#define CONSOLEPROGRESSINDICATOR_H

/**
 * @file ConsoleProgressIndicator.h
 * @brief Class ConsoleProgressIndicator - Console progress indicator for RadiosityRenderer.
 * @author xdudka00
 * @date 2007-11-18
 */


#include "ProgressObserverMultiStep.h"

class RadiosityRenderer;

/**
 * @brief Console progress indicator for RadiosityRenderer.
 */
class ConsoleProgressIndicator: public ProgressObserverMultiStep
{
	public:
		ConsoleProgressIndicator(RadiosityRenderer *);
		~ConsoleProgressIndicator();
		
		virtual void updateStep();
		virtual void updatePerStepProgress();
		
	private:
		RadiosityRenderer *renderer_;
		int counter_;
};

#endif // CONSOLEPROGRESSINDICATOR_H
