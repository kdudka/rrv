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

#include "Scene.h"
#include "RadiosityRenderer.h"
#include "ConsoleProgressIndicator.h"
#include "ComputeArguments.h"

#ifndef NDEBUG
#	include <iostream>
#endif

#include <iomanip>

class ContinousSaver: public ProgressObserverMultiStep {
		Scene *scene_;
		RadiosityRenderer *renderer_;
		std::string nameBase_;
		int perStepFreq_;
		int perStepCounter_;
// 		int stepSkip_;
	public:
		ContinousSaver(Scene *scene, RadiosityRenderer *renderer, std::string nameBase, int perStepFreq):
			scene_(scene),
			renderer_(renderer),
			nameBase_(nameBase),
			perStepFreq_(perStepFreq<<1),
			perStepCounter_(0)/*,
			stepSkip_(1)*/
		{
			renderer->attach(this, perStepFreq);
		}
		virtual void updateStep() {
			perStepCounter_ = 0;
			perStepFreq_ >>= 1;
			if (perStepFreq_ < 1)
				perStepFreq_ = 1;
			renderer_->detach(this);
			renderer_->attach(this, perStepFreq_);
		}
		virtual void updatePerStepProgress() {
// 			if (1==perStepFreq_) {
// 				if (0==renderer_->currentStep()%stepSkip_)
// 					stepSkip_ <<= 1;
// 				else if (renderer_->currentStep()+1!=renderer_->stepCount())
// 					return;
// 			}
			std::ostringstream nameStream;
			nameStream
					<< nameBase_
					<< "-step" << std::setw(3) << std::setfill('0') << renderer_->currentStep()
					<< "-snapshot" << std::setw(3) << std::setfill('0') << perStepCounter_
					<< ".xml";
			scene_->save(nameStream.str());
			perStepCounter_ ++;
		}
};

int main(int argc, char *argv[]) {
	ComputeArguments *args = new ComputeArguments();
	args->parseArguments( argc, argv );

	/* if (1>=argc)
		return 1; */
	
	std::string fileName(args->getFileIn());
	
	// Load scene
	Scene scene;

	try
	{
		scene.load(args->getFileIn());
	}
	catch( XML::XMLException &e )
	{
		std::cerr << e.what() << std::endl;
		exit( 0 );
	}
	
	// Acceleration
	scene.applyEmission();
	
	// Patch division
	//scene.divide(DIVIDE_TO);
	scene.divide(args->getDivide());
	
	// Compute radiosity
	RadiosityRenderer *renderer =
			scene.createRadiosityRenderer(args->getSteps(), args->getTreshold(), args->getCache()*1048576);
			//scene.createRadiosityRenderer(STEP_COUNT, FORM_FACTOR_TRESHOLD, MAX_CACHE_SIZE);
	
	ConsoleProgressIndicator *progressIndicator=
			new ConsoleProgressIndicator(renderer);
	
	ContinousSaver *saver=
		 	new ContinousSaver(&scene, renderer, fileName, args->getSaves());
			//new ContinousSaver(&scene, renderer, fileName, PER_STEP_SAVE_INITIAL);
	
	renderer->compute();
		
	delete saver;
	delete progressIndicator;
	delete renderer;

	//scene.save(fileName+"-final.xml");
	scene.save(args->getFileOut());
	
	delete args;

	return 0;
}
