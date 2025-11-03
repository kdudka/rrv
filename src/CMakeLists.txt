# Copyright (C) 2007-2009 Kamil Dudka <rrv@dudka.cz>
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

cmake_minimum_required(VERSION 2.6)
project(rrv CXX C)

# find -lGLEW and -lX11 using pkg-config
include(FindPkgConfig)
pkg_check_modules(GLEW glew x11)

# explicitly set OpenGL_GL_PREFERENCE to eliminate a warning
set(OpenGL_GL_PREFERENCE "LEGACY")

# check for -lglfw and <glm/glm.hpp> and initialize BUILD_GL3 accordingly
set(BUILD_GL3 0)
pkg_check_modules(GLFW glfw3)
if(GLFW_FOUND)
    include(CheckIncludeFileCXX)
    CHECK_INCLUDE_FILE_CXX(glm/glm.hpp GLM_FOUND)
    if(GLM_FOUND)
        set(BUILD_GL3 1)
    else()
        message(WARNING
            "not building rrv-compute-gl3 because <glm/glm.hpp> was not found")
    endif()
else()
    message(WARNING "not building rrv-compute-gl3 because glfw was not found")
endif()

# Check OpenGL availability
include(FindOpenGL)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "Cannot find OpenGL")
endif()
if(NOT OPENGL_GLU_FOUND)
	message(FATAL_ERROR "Cannot find GLU")
endif()

# Check GLUT availability
include(FindGLUT)
if(NOT GLUT_FOUND)
	message(STATUS "Warning: GLUT not found by cmake")
endif()

# Check for MINGW libraries
# TODO: cleanup this and check if it still works
find_library(FOUND_GLUT32 glut32)
if(FOUND_GLUT32)
	set(MINGW_LIBRARIES ${MINGW_LIBRARIES} glut32)
endif()
find_library(FOUND_MSVCP60 msvcp60)
if(FOUND_MSVCP60)
	set(MINGW_LIBRARIES ${MINGW_LIBRARIES} msvcp60)
endif()
find_library(FOUND_GDI32 gdi32)
if(FOUND_GDI32)
	set(MINGW_LIBRARIES ${MINGW_LIBRARIES} gdi32)
endif()

# Check for C++ compiler flags
include(CheckCXXCompilerFlag)
macro(ADD_CXX_OPT opt_name opt)
    check_cxx_compiler_flag(${opt} HAVE_${opt_name})
    if(HAVE_${opt_name})
        add_definitions(${opt})
    endif()
endmacro()

# tweak warnings
ADD_CXX_OPT("PEDANTIC"   "-pedantic")
ADD_CXX_OPT("W_ALL"      "-Wall")
ADD_CXX_OPT("W_EXTRA"    "-Wextra")
ADD_CXX_OPT("W_D4305"    "/wd4305")

option(RRV_DEBUG "Set to ON to disable optimizations" OFF)
if(RRV_DEBUG)
    add_definitions("-O0 -g")
else()
    add_definitions("-O3 -DNDEBUG")

    # tweak optimizations
    ADD_CXX_OPT("M_MMX"                         "-mmmx")
    ADD_CXX_OPT("F_EXPENSIVE_OPTIMIZATIONS"     "-fexpensive-optimizations")
    ADD_CXX_OPT("F_FAST_MATH"                   "-ffast-math")
    ADD_CXX_OPT("F_OMIT_FRAME_POINTER"          "-fomit-frame-pointer")
    ADD_CXX_OPT("F_OPENMP"                      "-fopenmp")
endif()

# Static library rrv
add_library(rrv STATIC
    ArgumentParser.cpp
    BarrelEntity.cpp
    ConsoleProgressIndicator.cpp
    CuboidEntity.cpp
    Entity.cpp
    EntitySet.cpp
    FormFactorEngine.cpp
    FormFactorHemicube.cpp
    FormFactorView.cpp
    GlobeEntity.cpp
    PatchCache.cpp
    PatchCacheLine.cpp
    PatchSequenceEnumerator.cpp
    PatchRandomAccessEnumerator.cpp
    ProgressObserverMultiStep.cpp
    ProgressSubjectMultiStep.cpp
    RadiosityRenderer.cpp
    Scene.cpp
    TeapotEntity.cpp
    TeapotEntityDef.cpp
    TransformMatrix.cpp
    TriangleEntity.cpp
    TriangleSet.cpp
    TriangleSetExt.cpp
    xmlParser.cpp
    XMLNames.cpp
    XMLHelper.cpp
    XMLBodyReader.cpp
    XMLWrapper.cpp
    XMLWriter.cpp)
target_link_libraries(rrv
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${MINGW_LIBRARIES})

# Executable binary rrv-compute
add_executable(rrv-compute
    rrv-compute.cpp
    ComputeArguments.cpp
    Visualizer.cpp)
target_link_libraries(rrv-compute rrv -fopenmp)

if(${BUILD_GL3})
    # Executable binary rrv-compute-gl3
    add_executable(rrv-compute-gl3 rrv-compute-gl3.cpp)
    target_link_libraries(rrv-compute-gl3 rrv
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        -fopenmp)
endif()

# Executable binary rrv-visualize
add_executable(rrv-visualize
    rrv-visualize.cpp
    Visualizer.cpp
    VisualizeArguments.cpp)
target_link_libraries(rrv-visualize rrv -fopenmp)
