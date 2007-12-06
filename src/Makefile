# ATTENTION: This is emergency Makefile
# Use cmake to build usable Makefile

ifeq ($(OS), Windows_NT)
  CXXFLAGS=-std=c++98 -W -Wall -pedantic -pipe -mmmx -fexpensive-optimizations -ffast-math -fomit-frame-pointer -DNDEBUG
else
  CXXFLAGS=-std=c++98 -W -Wall -pedantic -O3 -pipe -mmmx -fexpensive-optimizations -ffast-math -fomit-frame-pointer -fPIC -DNDEBUG
endif

BIN=rrv-visualize rrv-compute
OBJ=\
	ArgumentParser.o \
	BarrelEntity.o \
	ConsoleProgressIndicator.o \
	CuboidEntity.o \
	Entity.o \
	EntitySet.o \
	FormFactorEngine.o \
	GlobeEntity.o \
	PatchCache.o \
	PatchCacheLine.o \
	PatchSequenceEnumerator.o \
	PatchRandomAccessEnumerator.o \
	ProgressObserverMultiStep.o \
	ProgressSubjectMultiStep.o \
	RadiosityRenderer.o \
	Scene.o \
	TeapotEntity.o \
	TeapotEntityDef.o \
	TransformMatrix.o \
	TriangleEntity.o \
	TriangleSet.o \
	TriangleSetExt.o \
	xmlParser.o\
	XMLNames.o\
	XMLHelper.o\
	XMLBodyReader.o\
	XMLWrapper.o\
	XMLWriter.o

.PHONY:clean

all: clean
	$(MAKE) rrv-compute
	$(MAKE) rrv-visualize

ifeq ($(OS), Windows_NT)
  LIBS=-lopengl32 -lglu32 -lglut32 -lmsvcp60 -lgdi32
else
  LIBS=-lGL -lGLU -lglut -lXmu -lX11 -lm #-lXi -L/usr/X11R6/lib
endif

rrv-compute: $(OBJ) rrv-compute.o ComputeArguments.o 
	$(CXX) -o $@ rrv-compute.o ComputeArguments.o $(OBJ) $(LDFLAGS) $(LIBS)

rrv-visualize: $(OBJ) rrv-visualize.o VisualizeArguments.o Visualizer.o
	$(CXX) -o $@ rrv-visualize.o VisualizeArguments.o Visualizer.o $(OBJ) $(LDFLAGS) $(LIBS)

clean:
	$(RM) -vf $(OBJ) rrv-compute.o ComputeArguments.o rrv-visualize.o VisualizeArguments.o Visualizer.o
	$(RM) -vf $(BIN)
