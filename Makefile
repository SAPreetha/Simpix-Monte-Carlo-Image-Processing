ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTGLIBS  = $(shell root-config --glibs)
CXXFLAGS  += $(ROOTCFLAGS)
LIBS       = $(ROOTLIBS) -lASImage
GLIBS      = $(ROOTGLIBS)
GXX	   = g++ -Wall -O3

simpix_start:  simpix.cpp
	$(GXX) -o simpix simpix.cpp $(ROOTCFLAGS) $(LIBS) $(ROOTGLIBS)


clean:
	rm -f simpix out.png
