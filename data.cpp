// header inclusion
#include "data.h"
#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>


//define vectors
std::vector <std::vector<int> > dart4 = {{361,143,110,126}};
std::vector <std::vector<int> > dart5  = {{72,150,44,54}, {257,178,45,46},{383,201,51,46},{516,190,63,49},{650,199,47,51},{60,263,57,54},{196,230,53,53},{296,256,47,57},{436,238,47,65},{567,259,44,54},{683,262,45,51}};
std::vector <std::vector<int> > dart6 = {{294,148,322,157}};
std::vector <std::vector<int> > dart7 = {{349,213,59,71}};
std::vector <std::vector<int> > dart9 = {{101,237,87,104}};
std::vector <std::vector<int> > dart11 = {{342,91,38,50}};
std::vector <std::vector<int> > dart13 = {{429,150,84,105}};
std::vector <std::vector<int> > dart14 = {{480,240,66,83},{738,218,74,75}};
std::vector <std::vector<int> > dart15 = {{77,145,45,65},{549,144,39,66}};

std::map <std::string, std::vector <std::vector<int> > > dartMap = {
																													{"darts/dart4.jpg", dart4},
																													{"darts/dart5.jpg", dart5},
																													{"darts/dart6.jpg", dart6},
																													{"darts/dart7.jpg", dart7},
																													{"darts/dart9.jpg", dart9},
																													{"darts/dart11.jpg", dart11},
																													{"darts/dart13.jpg", dart13},
																													{"darts/dart14.jpg", dart14},
																													{"darts/dart15.jpg", dart15}
};

std::vector <std::vector<int> > getData (std::string file) {
	return dartMap[file];
}