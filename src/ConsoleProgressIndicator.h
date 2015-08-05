/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>
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
