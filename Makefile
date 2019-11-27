make:
	g++  face.cpp data.cpp 3darr.cpp houghTransform.cpp $(pkg-config --cflags --libs $OCVLOC) -std=c++11