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
