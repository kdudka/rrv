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
