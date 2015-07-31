/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>, Jakub Filak
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
#include "XMLBodyReader.h"
#include "PatchSequenceEnumerator.h"
#include "RadiosityRenderer.h"
#include "TriangleSetExt.h"
#include "PatchRandomAccessEnumerator.h"

#include <cstdio>

using namespace XML;


/**
 * @param  fileName
 * @author Jakub Filak
 * @date   2007-10-25
 */
void Scene::load (std::string fileName )
{
    XMLWrapper xmlW;

    try
    {
#ifndef NDEBUG
        fprintf( stderr, "%s", "Start loading file ... ");
#endif
        xmlW.loadFile( fileName );						// Load file
    }
    catch(XMLException xe)
    {
        fprintf( stderr, "ERROR Scene::load(): %s\n", xe.what() );
        exit(0); // TODO: ???
    }

#ifndef NDEBUG
    fprintf( stderr, "%s", "File loaded\n");
    fprintf( stderr, "%s", "Reading objects and transformations ...\n");	
#endif
    XMLNode* entity = 0;
    while( 0 != ( entity = xmlW.nextEntityNode() ) )
    {	// read nodes from body and aply transformations and create entities
        Entity* ne = Entity::create( entity );				// try create entity

        if( 0 != ne )
        {	// if entity exists then
#ifndef NDEBUG
            printf( "\tVytvorena entita: %s\n", entity->getName() );	
#endif
            ne->setTransformMatrix( xmlW.getTransformMatrix() );		// take actual transform matrix and set this to entity
            ne->deserialize( entity );				// initialize entity from XMLNode
            entitySet_.add( ne );					// insert entity to scene entity set

            delete ne;
        }
    }
#ifndef NDEBUG
    fprintf( stderr, "%s", "*** Scene is loaded ***\n");
#endif
}

void Scene::applyEmission() {
#ifndef NDEBUG
    std::cout << ">>> Applying emission ... " << std::flush;
#endif
    PatchSequenceEnumerator *enumerator=
        entitySet_.createPatchSequenceEnumerator();

    Triangle *t;
    while (0!= (t= enumerator->nextPatch())) {
        Color &radiosity= t->radiosity;
        Color &emission= t->emission;
        if (radiosity.r < emission.r)
            radiosity.r = emission.r;

        if (radiosity.g < emission.g)
            radiosity.g = emission.g;

        if (radiosity.b < emission.b)
            radiosity.b = emission.b;
    }

    delete enumerator;
#ifndef NDEBUG
    std::cout << "ok" << std::endl;
#endif
}

/**
*/
void Scene::divide (float size) {
#if 1//ndef NDEBUG
    PatchRandomAccessEnumerator *iter=
        PatchRandomAccessEnumerator::create(&entitySet_);
    std::cout << "--- Count of patch: "	<< iter->count() << std::endl;
    //std::cout << "--- Max. patch size: " << size << std::endl;
    delete iter;

    std::cout << ">>> Starting patch division ... " << std::flush;
#endif
    entitySet_.divide(size);
#if 1//ndef NDEBUG
    std::cout << "ok" << std::endl;
    iter= PatchRandomAccessEnumerator::create(&entitySet_);
    std::cout << "--- Count of patch: "	<< iter->count() << std::endl;
    std::cout << std::endl;
    delete iter;
#endif
}

RadiosityRenderer* Scene::createRadiosityRenderer(int stepCount, float formFactorTreshold, long maxCacheSize) {
    PatchSequenceEnumerator *patchEnumerator =
        entitySet_.createPatchSequenceEnumerator();

    RadiosityRenderer *renderer =
        new RadiosityRenderer(patchEnumerator, stepCount, formFactorTreshold, maxCacheSize);

    delete patchEnumerator;
    return renderer;
}

PatchSequenceEnumerator* Scene::createPatchSequenceEnumerator() {
    return entitySet_.createPatchSequenceEnumerator();
}

/**
 * @param  fileName
 */
void Scene::save (std::string fileName )
{
    XMLWriter writer;
    unsigned enCount = entitySet_.count();

    for( unsigned i = 0; i < enCount; i++)
    {
        // FIXME: Using reference instead of object copy (not tested)
        Entity &et = entitySet_[i];
        XMLNode tmp = et.serialize();
        writer.addEntity( tmp  );
    }	
    writer.writeToFile(fileName.c_str());
}


/**
 * Main method for visualization of scene in Glut window
 */
TriangleSetExt* Scene::dumpForVis()
{
    return entitySet_.computeVertexColors();
}
