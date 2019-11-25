make:
	g++ $(pkg-config --cflags --libs $OCVLOC) face.cpp data.cpp 3darr.cpp houghTransform.cpp -std=c++11