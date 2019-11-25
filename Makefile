make:
	g++ $(pkg-config --cflags --libs $OCVLOC) face.cpp data.cpp -std=c++11