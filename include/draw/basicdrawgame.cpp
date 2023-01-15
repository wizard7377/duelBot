#include "drawgame.hpp"
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#ifndef IMG_SIZE
#define IMG_SIZE 960
#endif



using namespace cv;
namespace fs = std::filesystem;
using json = nlohmann::json;

extern std::string getFullPath(std::string filePath);
extern std::string getFullPath(std::vector<std::string> filePath);
namespace dg {



/*
	std::cout << conFile;
	fs::path appPath = fs::current_path();
	appPath /= ("config/"+conFile);
	std::cout << appPath;
*/

Mat * basicDrawGame::changeBoard(std::vector<std::vector<int>> newBoard) {
	Mat * currentImg = new Mat(IMG_SIZE,IMG_SIZE,CV_8UC3,Scalar(69,42,32));
	
	int sizeX = newBoard.size();
	int sizeY = newBoard[0].size();
	int spaceSizeX = IMG_SIZE/sizeX;
	int spaceSizeY = IMG_SIZE/sizeY;
	for (int i = 0; i < sizeX; i++) {
		for (int ii = 0; ii< sizeY; ii++) {
			Rect space(spaceSizeX*i,spaceSizeY*ii,spaceSizeX,spaceSizeY);
			Mat tarSpace = (*currentImg)(space);
			
			
			
			(this->imgs[0][newBoard[ii][i]]).copyTo(tarSpace);
			
			
		}
	}
	
	imshow("display window",*currentImg);
	waitKey(0);
	
	return currentImg;
}


			



basicDrawGame::basicDrawGame(std::vector<std::vector<int>> userItems,std::string boardName,std::string conFile) {
	
	std::ifstream jFile(getFullPath({"config",conFile}));

	/*
	std::string linestr;
	while (std::getline (jFile, linestr)) {
  		std::cout << linestr << std::endl;
	}
	*/

	

	json gameconfig = json::parse(jFile)[boardName];
	/*for (auto a: (gameconfig["imgpaths"]).get<std::vector<std::vector<std::string>>>()) {
		for (auto b: a) {
			std::cout << b << std::endl;
		}
	}*/
	
	this->initBoardItems((gameconfig["imgpaths"]).get<std::vector<std::vector<std::string>>>());
	//std::cout << std::to_string(this->imgs.size()) << std::endl;
	//this->boardItems = std::vector<std::vector<int>>(userItems.size(),std::vector<int>(userItems[0].size(),0));
	this->changeBoard(userItems);
	//this->boardItems = userItems;

	
	

	
	
	
}

}

