# Copyright (C) 2007 Kamil Dudka
#
# This file is part of rrv (Radiosity Renderer and Visualizer).
#
# rrv is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# rrv is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with rrv.  If not, see <http://www.gnu.org/licenses/>.

project(rrv CXX C)

# Debug mode switch
SET(DEBUG_MODE 0 CACHE BOOL "Turn off to supress debug outputs")
IF(NOT DEBUG_MODE)
	ADD_DEFINITIONS(-DNDEBUG)
ENDIF(NOT DEBUG_MODE)

# Check OpenGL availability
INCLUDE(FindOpenGL)
IF(NOT OPENGL_FOUND)
	MESSAGE(FATAL_ERROR "Cannot find OpenGL")
ENDIF(NOT OPENGL_FOUND)
IF(NOT OPENGL_GLU_FOUND)
	MESSAGE(FATAL_ERROR "Cannot find GLU")
ENDIF(NOT OPENGL_GLU_FOUND)

# Check GLUT availability
INCLUDE(FindGLUT)
IF(NOT GLUT_FOUND)
	MESSAGE(STATUS "Warning: GLUT not found by cmake")
ENDIF(NOT GLUT_FOUND)

# Check for MINGW libraries
FIND_LIBRARY(FOUND_GLUT32 glut32)
IF(FOUND_GLUT32)
	SET(MINGW_LIBRARIES ${MINGW_LIBRARIES} glut32)
ENDIF(FOUND_GLUT32)
FIND_LIBRARY(FOUND_MSVCP60 msvcp60)
IF(FOUND_MSVCP60)
	SET(MINGW_LIBRARIES ${MINGW_LIBRARIES} msvcp60)
ENDIF(FOUND_MSVCP60)
FIND_LIBRARY(FOUND_GDI32 gdi32)
IF(FOUND_GDI32)
	SET(MINGW_LIBRARIES ${MINGW_LIBRARIES} gdi32)
ENDIF(FOUND_GDI32)

# Check for C++ compiler flags
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-std=c++98 HAVE_STD)
IF(HAVE_STD)
	SET(STD_FLAG "-std=c++98")
ENDIF(HAVE_STD)
CHECK_CXX_COMPILER_FLAG("-pedantic -W -Wall" HAVE_PEDANTIC)
IF(HAVE_PEDANTIC)
	SET(PEDANTIC_FLAG "-pedantic -W -Wall")
ENDIF(HAVE_PEDANTIC)
CHECK_CXX_COMPILER_FLAG("/wd4305" HAVE_WD4305)
IF(HAVE_WD4305)
	SET(WD4305_FLAG "/wd4305")
ENDIF(HAVE_WD4305)
CHECK_CXX_COMPILER_FLAG(-O3 HAVE_O3)
IF(HAVE_O3)
	SET(O3_FLAG "-O3")
ENDIF(HAVE_O3)
CHECK_CXX_COMPILER_FLAG(-pipe HAVE_PIPE)
IF(HAVE_PIPE)
	SET(PIPE_FLAG "-pipe")
ENDIF(HAVE_PIPE)
CHECK_CXX_COMPILER_FLAG(-mmmx HAVE_MMX)
IF(HAVE_MMX)
	SET(MMX_FLAG "-mmmx")
ENDIF(HAVE_MMX)
CHECK_CXX_COMPILER_FLAG(-fexpensive-optimizations HAVE_EXPENSIVE_OPTIMALIZATIONS)
IF(HAVE_EXPENSIVE_OPTIMALIZATIONS)
	SET(EXPENSIVE_OPTIMALIZATIONS_FLAG "-fexpensive-optimizations")
ENDIF(HAVE_EXPENSIVE_OPTIMALIZATIONS)
CHECK_CXX_COMPILER_FLAG(-ffast-math HAVE_FAST_MATH)
IF(HAVE_FAST_MATH)
	SET(FAST_MATH_FLAG "-ffast-math")
ENDIF(HAVE_FAST_MATH)
CHECK_CXX_COMPILER_FLAG(-fomit-frame-pointer HAVE_OMIT_FRAME_POINTER)
IF(HAVE_OMIT_FRAME_POINTER)
	SET(OMIT_FRAME_POINTER_FLAG "-fomit-frame-pointer")
ENDIF(HAVE_OMIT_FRAME_POINTER)

# Set C++ compiler flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STD_FLAG} ${PEDANTIC_FLAG} ${WD4305_FLAG} ${O3_FLAG} ${PIPE_FLAG} ${MMX_FLAG} ${EXPENSIVE_OPTIMALIZATIONS_FLAG} ${FAST_MATH_FLAG} ${OMIT_FRAME_POINTER_FLAG}" CACHE STRING "C++ compiler flags" FORCE)

# Static library rrv
ADD_LIBRARY(rrv STATIC ArgumentParser.cpp BarrelEntity.cpp ConsoleProgressIndicator.cpp CuboidEntity.cpp Entity.cpp EntitySet.cpp FormFactorEngine.cpp GlobeEntity.cpp PatchCache.cpp PatchCacheLine.cpp PatchSequenceEnumerator.cpp PatchRandomAccessEnumerator.cpp ProgressObserverMultiStep.cpp ProgressSubjectMultiStep.cpp RadiosityRenderer.cpp Scene.cpp TeapotEntity.cpp TeapotEntityDef.cpp TransformMatrix.cpp TriangleEntity.cpp TriangleSet.cpp TriangleSetExt.cpp xmlParser.cpp XMLNames.cpp XMLHelper.cpp XMLBodyReader.cpp XMLWrapper.cpp XMLWriter.cpp)
TARGET_LINK_LIBRARIES(rrv ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${MINGW_LIBRARIES})

# Executable binary rrv-compute
ADD_EXECUTABLE(rrv-compute rrv-compute.cpp ComputeArguments.cpp Visualizer.cpp)
TARGET_LINK_LIBRARIES(rrv-compute rrv)

# Executable binary rrv-visualize
ADD_EXECUTABLE(rrv-visualize rrv-visualize.cpp Visualizer.cpp VisualizeArguments.cpp)
TARGET_LINK_LIBRARIES(rrv-visualize rrv)

# make install
INSTALL(TARGETS rrv-compute rrv-visualize DESTINATION bin)
