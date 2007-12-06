#include "ConsoleProgressIndicator.h"
#include "RadiosityRenderer.h"

#include <iostream>
#include <iomanip>

ConsoleProgressIndicator::ConsoleProgressIndicator(RadiosityRenderer *renderer):
		renderer_(renderer),
		counter_(0)
{
	renderer->attach(this, 100);
}

ConsoleProgressIndicator::~ConsoleProgressIndicator() {
	renderer_->detach(this);
}

void ConsoleProgressIndicator::updateStep() {
	counter_ = 0;
	std::cout
			<< ">>> Step "
			<< std::setw(3) << std::setfill(' ') << renderer_->currentStep()+1
			<< " of " << renderer_->stepCount()
			<< " "
			<< std::flush;
	
	if (1== renderer_->currentStep()) {
		renderer_->detach(this);
		renderer_->attach(this, 10);
		updatePerStepProgress();
	}
	
	// FIXME: This should not be here
 	if (!renderer_->currentStep())
 		updatePerStepProgress();
}

void ConsoleProgressIndicator::updatePerStepProgress() {
	if (0==counter_%10 && !renderer_->currentStep()) {
		if (counter_) {
			std::cout
					<< " cache raw size: "
					<< std::setw(7) << std::setfill(' ')
					<< std::setprecision(2) << std::fixed
					<< static_cast<float>(renderer_->cacheRawSize())/(1024*1024)
					<< " MB";
		}
		std::cout
				<< std::endl
				<< "    "
				<< std::setw(2) << std::setfill(' ') << counter_
				<< "% ";
	}
	
	if (100> counter_)
		std::cout << "." << std::flush;
	else
		std::cout << std::endl;
	
	if (renderer_->currentStep() && counter_ >=10)
		std::cout << std::endl;
	
	counter_++;
}
