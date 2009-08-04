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

#ifndef XMLWRAPPER_H
#define XMLWRAPPER_H

/*
 * @brief  XMLWrapper interface
 * @author Jakub Filak, filak.jakub@gmail.com
 * @date   2007-10-25
 */

#include "XMLHelper.h"
#include "XMLWriter.h"
#include "XMLNames.h"
#include "XMLBodyReader.h"

namespace XML
{
    class XMLWrapper
    {
        public:
            XMLWrapper();

            void loadFile( std::string fileName );

            XMLNode* nextEntityNode();

            TransformMatrix* getTransformMatrix();

            virtual ~XMLWrapper();

        private:
            XMLHelper *xmlHelper_;
            XMLBodyReader *xmlBodyReader_;
    };
}

#endif
