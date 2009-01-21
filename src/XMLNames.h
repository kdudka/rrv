#ifndef XMLNAMES_H
#define XMLNAMES_H
	
namespace XML
{
	typedef enum XMLTagName
	{
		Root=0,
		Definition,
		Instantiate,
		ObjectDefinition,
		Cuboid,
		Barrel,
		Globe,
		Teapot,
		TriangleSetNode,
		TriangleNode,
		VertexNode,
		Rotate,
		Scale,
		Shear,
		Translate,
		Object,
		Trianglenext,
		angle
       	} XMLTN;
	
	typedef enum XMLTagAttributeName
	{
		Name=0,
		Emission,
		Reflectivity,
		Radiosity,
		VertexX,
		VertexY,
		VertexZ,
		TransformationX,
		TransformationY,
		TransformationZ,
		RotateAngle,
		IncludeFile,
		Spec,
		Refr,
		Refl
	} XMLTAN;
	
	struct XMLNames
	{
		public:
			static const char* TAGS[];

			static const char* ATTRIBUTES[];
	};
}	
#endif
