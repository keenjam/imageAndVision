make:
	g++ -std=c++11 face.cpp data.cpp 3darr.cpp houghTransform.cpp $('pkg-config --cflags --libs $OCVLOC') 