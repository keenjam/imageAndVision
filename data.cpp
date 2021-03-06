// header inclusion
#include "data.h"
#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <opencv/cv.h>


//define face vectors
std::vector <cv::Rect> face4 = {{361,143,110,126}};
std::vector <cv::Rect> face5  = {{72,150,44,54}, {257,178,45,46},{383,201,51,46},{516,190,63,49},{650,199,47,51},{60,263,57,54},{196,230,53,53},{296,256,47,57},{436,238,47,65},{567,259,44,54},{683,262,45,51}};
std::vector <cv::Rect> face6 = {{294,121,35,35}};
std::vector <cv::Rect> face7 = {{349,213,59,71}};
std::vector <cv::Rect> face9 = {{101,237,87,104}};
std::vector <cv::Rect> face11 = {{342,91,38,50}};
std::vector <cv::Rect> face13 = {{429,150,84,105}};
std::vector <cv::Rect> face14 = {{480,240,66,83},{738,218,74,75}};
std::vector <cv::Rect> face15 = {{77,145,45,65},{549,144,39,66}};

//define darts vectors
std::vector <cv::Rect> dart0 = {{423,0,194,217}};
std::vector <cv::Rect> dart1 = {{165,104,252,250}};
std::vector <cv::Rect> dart2 = {{89,84,115,115}};
std::vector <cv::Rect> dart3 = {{313,138,86,92}};
std::vector <cv::Rect> dart4 = {{151,63,248,257}};
std::vector <cv::Rect> dart5 = {{419,124,125,127}};
std::vector <cv::Rect> dart6 = {{203,107,78,83}};
std::vector <cv::Rect> dart7 = {{234,150,180,185}};
std::vector <cv::Rect> dart8 = {{60,242,74,109},{826,199,148,159}};
std::vector <cv::Rect> dart9 = {{168,11,300,307}};
std::vector <cv::Rect> dart10 = {{74,89,125,142},{576,119,70,106},{913,143,41,79}};
std::vector <cv::Rect> dart11 = {{167,94,75,96}};
std::vector <cv::Rect> dart12 = {{150,57,76,177}};
std::vector <cv::Rect> dart13 = {{255,105,169,165}};
std::vector <cv::Rect> dart14 = {{101,82,166,164},{969,78,160,161}};
std::vector <cv::Rect> dart15 = {{130,34,171,185}};

std::map <std::string, std::vector <cv::Rect> > faceMap = {
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

std::map <std::string, std::vector <cv::Rect> > dartMap = {
																													{"darts/dart0.jpg", dart0},
																													{"darts/dart1.jpg", dart1},
																													{"darts/dart2.jpg", dart2},
																													{"darts/dart3.jpg", dart3},
																													{"darts/dart4.jpg", dart4},
																													{"darts/dart5.jpg", dart5},
																													{"darts/dart6.jpg", dart6},
																													{"darts/dart7.jpg", dart7},
																													{"darts/dart8.jpg", dart8},
																													{"darts/dart9.jpg", dart9},
																													{"darts/dart10.jpg", dart10},
																													{"darts/dart11.jpg", dart11},
																													{"darts/dart12.jpg", dart12},
																													{"darts/dart13.jpg", dart13},
																													{"darts/dart14.jpg", dart14},
																													{"darts/dart15.jpg", dart15}

};

std::vector <cv::Rect> getFaceData (std::string file) {
	return faceMap[file];
}

std::vector <cv::Rect> getDartData (std::string file) {
	return dartMap[file];
}