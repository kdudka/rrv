#include "Scene.h"
#include "XMLBodyReader.h"
#include "PatchSequenceEnumerator.h"
#include "RadiosityRenderer.h"
#include "TriangleSetExt.h"

#ifndef NDEBUG
#	include "PatchRandomAccessEnumerator.h"
#endif

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
#ifndef NDEBUG
	PatchRandomAccessEnumerator *iter=
			PatchRandomAccessEnumerator::create(&entitySet_);
	std::cout << "--- Count of patch: "	<< iter->count() << std::endl;
	std::cout << "--- Max. patch size: " << size << std::endl;
	delete iter;
	
	std::cout << ">>> Starting patch division ... " << std::flush;
#endif
	entitySet_.divide(size);
#ifndef NDEBUG
	std::cout << "ok" << std::endl;
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
