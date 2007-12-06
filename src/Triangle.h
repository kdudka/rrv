#ifndef TRIANGLE_H
#define TRIANGLE_H

/**
 * @file Triangle.h
 * @brief Structure Triangle and its component structures
 * @author xdudka00, xfilak01, xbarin02
 * @date 2007-11-17
 */

#ifndef NDEBUG
#	ifndef BUILDING_DOX
#		include <iostream>
#	endif
#endif

/**
 * @brief 3D vertex (3 axes - x, y, z)
 */
struct Vertex {
		float x;	///< x-axe of vertex
		float y;	///< y-axe of vertex
		float z;	///< z-axe of vertex
		
		/**
		 * @attention Default constructor is @b not @b implemented.
		 */
		Vertex() {}
		
		/**
		 * @param ix x-axe of vertex
		 * @param iy y-axe of vertex
		 * @param iz z-axe of vertex
		 */
		Vertex(float ix, float iy, float iz):
				x(ix),
				y(iy),
				z(iz)
				{}

		/**
		 * @param coords Array of axes (0 - x, 1 - y, 2 - z)
		 */
		Vertex(float coords[3]):
				x(coords[0]),
				y(coords[1]),
				z(coords[2])
				{}
};

#ifndef NDEBUG
/**
 * @brief Represent Vertex object in human readable format.
 * @note This function will not be built if NDEBUG macro is defined.
 * @param out Output stream to write to.
 * @param vertex Reference to Vertex object to write.
 * @return Return output stream given as parameter.
 */
inline std::ostream& operator<< (std::ostream &out, const Vertex &vertex) {
	out << "(" <<
			vertex.x << ", " <<
			vertex.y << ", " <<
			vertex.z << ")";
	return out;
}
#endif

/**
 * @brief Color representation - 3 float (r, g, b) in range <0.0, 1.0>
 * @note <0.0, 1.0> is effective range, but there are no range restriction.
 */
struct Color {
		float r;	///< red
		float g;	///< green
		float b;	///< blue

		/**
		 * @param cr red
		 * @param cg green
		 * @param cb blue
		 */
		Color(float cr, float cg, float cb):
			r(cr),
			g(cg),
			b(cb)
			{}

		/**
		 * @param rgb Array of colors (0 - red, 1 - green, 2 - blue)
		 */
		Color( float rgb[] ):
			r(rgb[0]),
			g(rgb[1]),
			b(rgb[2])
			{}
		
		/**
		 * @brief Default color is black (r=0.0, g=0.0, b=0.0)
		 */
		Color():
				r(0.0),
				g(0.0),
				b(0.0)
				{}
};

/**
 * @brief Addition of another color.
 * Behaves as usual vector arithmetic.
 * @param dest Destination color structure.
 * @param src Color to add.
 * @return Return destination color given as parameter.
 */
inline Color& operator+= (Color &dest, const Color &src) {
	dest.r += src.r;
	dest.g += src.g;
	dest.b += src.b;
	return dest;
}

/**
 * @brief Multiplication by another color.
 * Component after component.
 * @param dest Destination color structure.
 * @param src Color to multiplicate by.
 * @return Return destination color given as parameter.
 */
inline Color& operator*= (Color &dest, const Color &src) {
	dest.r *= src.r;
	dest.g *= src.g;
	dest.b *= src.b;
	return dest;
}

/**
 * @brief Multiplication by constant.
 * Behaves as usual vector arithmetic.
 * @param c Destination color structure.
 * @param ratio Constant to multiplicate by.
 * @return Return destination color given as parameter.
 */
inline Color& operator*= (Color &c, float ratio) {
	c.r *= ratio;
	c.g *= ratio;
	c.b *= ratio;
	return c;
}

/**
 * @brief Multiplication by constant.
 * Behaves as usual vector arithmetic.
 * @param c Color to multiplicate.
 * @param ratio Constant to multiplicate by.
 * @return Return result of multiplication.
 */
inline Color operator* (Color c, float ratio) {
	c *= ratio;
	return c;
}

/**
 * @brief Compare two color structures.
 * Component after component.
 * @param a Color to compare.
 * @param b Color to compare.
 * @return Return true if colors are equal.
 */
inline bool operator== (const Color &a, const Color &b) {
	return
			a.r == b.r &&
			a.g == b.g &&
			a.b == b.b;
}

#ifndef NDEBUG
/**
 * @brief Represent Color object in human readable format.
 * @note This function will not be built if NDEBUG macro is defined.
 * @param out Output stream to write to.
 * @param color Reference to Color object to write.
 * @return Return output stream given as parameter.
 */
inline std::ostream& operator<< (std::ostream &out, const Color &color) {
	out << "RGB(" <<
			color.r << ", " <<
			color.g << ", " <<
			color.b << ")";
	return out;
}
#endif

/**
 * @brief Low-level triangle representation - plain structure.
 * This structure contains 3 Vertexes and 3 Colors (needed for radiosity rendering)
 */
struct Triangle
{
		Vertex vertex[3];     ///< Array of vertexes
		Color emission;       ///< Triangle's own emission (for light sources)
		Color reflectivity;   ///< Triangle's reflectivity (material property)
		Color radiosity;      ///< Triangle's computed radiosity
		Color radiosityLast;	///< Working variable of RadiosityRenderer

		/**
		 * @brief Compute center of triangle.
		 * @return Return triangle's center as Vertex object
		 */
		static Vertex centerOf(const Triangle &t)
		{
			float x = (t.vertex[0].x+t.vertex[1].x+t.vertex[2].x)/3.0f;
			float y = (t.vertex[0].y+t.vertex[1].y+t.vertex[2].y)/3.0f;
			float z = (t.vertex[0].z+t.vertex[1].z+t.vertex[2].z)/3.0f;
			return(Vertex(x,y,z));
		}
};

#ifndef NDEBUG
/**
 * @brief Represent Triangle object in human readable format.
 * @note This function will not be built if NDEBUG macro is defined.
 * @param out Output stream to write to.
 * @param t Reference to Triangle object to write.
 * @return Return output stream given as parameter.
 */
inline std::ostream& operator<< (std::ostream &out, const Triangle &t) {
	out << "Triangle:" << std::endl << "  " <<
			t.vertex[0] << " " << std::endl << "  " <<
			t.vertex[1] << " " << std::endl << "  " <<
			t.vertex[2] << " " << std::endl << "  " <<
			"    Emission: " << t.emission << std::endl << "  " <<
			"Reflectivity: " << t.reflectivity << std::endl << "  " <<
			"   Radiosity: " << t.radiosity << std::endl <<
			std::endl;
	return out;
}
#endif

#endif // TRIANGLE_H
