// header inclusion
#include "data.h"
#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>


//define face vectors
std::vector <std::vector<int> > face4 = {{361,143,110,126}};
std::vector <std::vector<int> > face5  = {{72,150,44,54}, {257,178,45,46},{383,201,51,46},{516,190,63,49},{650,199,47,51},{60,263,57,54},{196,230,53,53},{296,256,47,57},{436,238,47,65},{567,259,44,54},{683,262,45,51}};
std::vector <std::vector<int> > face6 = {{294,148,322,157}};
std::vector <std::vector<int> > face7 = {{349,213,59,71}};
std::vector <std::vector<int> > face9 = {{101,237,87,104}};
std::vector <std::vector<int> > face11 = {{342,91,38,50}};
std::vector <std::vector<int> > face13 = {{429,150,84,105}};
std::vector <std::vector<int> > face14 = {{480,240,66,83},{738,218,74,75}};
std::vector <std::vector<int> > face15 = {{77,145,45,65},{549,144,39,66}};

//define darts vectors
std::vector <std::vector<int> > dart0 = {{423,0,194,217}};

std::map <std::string, std::vector <std::vector<int> > > faceMap = {
																													{"darts/dart4.jpg", face4},
																													{"darts/dart5.jpg", face5},
																													{"darts/dart6.jpg", face6},
																													{"darts/dart7.jpg", face7},
																													{"darts/dart9.jpg", face9},
																													{"darts/dart11.jpg", face11},
																													{"darts/dart13.jpg", face13},
																													{"darts/dart14.jpg", face14},
																													{"darts/dart15.jpg", face15}
};

std::map <std::string, std::vector <std::vector<int> > > dartMap = {
																													{"darts/dart0.jpg", dart0}
};

std::vector <std::vector<int> > getFaceData (std::string file) {
	return faceMap[file];
}

std::vector <std::vector<int> > getDartData (std::string file) {
	return dartMap[file];
}