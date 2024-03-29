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

#ifndef XMLWRITER_H
#define XMLWRITER_H

#include "xmlParser.h"

namespace XML
{
    class XMLWriter
    {
        public:
            XMLWriter();

            void addEntity( XMLNode entityNode );

            void writeToFile( const char* fileName );

            static XMLSTR copyString( XMLCSTR from );

        private:
            XMLCSTR objectName;

            XMLNode root_;
            XMLNode definition_;
            XMLNode objectdef_;
            XMLNode instantiate_;
            XMLNode object_;			
    };
}

#endif
