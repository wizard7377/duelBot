#include "drawgame.hpp"
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <format>
#include <source_location>

#ifndef IMG_SIZE
#define IMG_SIZE 960
#endif
//TODO Add board notation markings to images
static unsigned long long int currentImgBasic = 1;

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

int boolToInt(bool input) {
	if (input) {
		return 1;
	} else {
		return 0;
	}
}

Mat * basicDrawGame::changeBoard(std::vector<std::vector<int>> newBoard) {
	
	
	
	int sizeX = newBoard.size();
	int sizeY = newBoard[0].size();
	Mat * currentImg = new Mat(IMG_SIZE,IMG_SIZE,CV_8UC3,Scalar(69,42,32));
	int spaceSizeX = IMG_SIZE/sizeX;
	int spaceSizeY = IMG_SIZE/sizeY;
	bool spaceColor = true;
	for (int i = 0; i < sizeX; i++) {
		for (int ii = 0; ii< sizeY; ii++) {
			Rect space(spaceSizeY*ii,spaceSizeX*i,spaceSizeY,spaceSizeX);
			
			Mat tarSpace = (*currentImg)(space);
			
			
			
			if (this->alternates) {
				(this->imgs[boolToInt(spaceColor)][newBoard[i][ii]]).copyTo(tarSpace);
				spaceColor = !(spaceColor);
			} else {
				this->imgs[0][newBoard[i][ii]].copyTo(tarSpace);
			}

			
			
			
		}
		spaceColor = !(spaceColor);
	}
	
	return currentImg;
}

/**
 * @brief Returns path to a new board drawn
 * @return Path to board
 * @param newBoard Board state
*/
std::string basicDrawGame::getBoard(std::vector<std::vector<int>> newBoard) {
	Mat * retMat = this->changeBoard(newBoard);
	std::string retPath = getFullPath({"output","basic",((std::to_string(currentImgBasic))+".png")});
	
	currentImgBasic++;
	imwrite(retPath,*retMat);
	delete retMat;
	return retPath;
}


			



basicDrawGame::basicDrawGame(std::string boardName,std::string conFile) {
	
	std::ifstream jFile(getFullPath({"config",conFile}));
	
	/*
	std::string linestr;
	while (std::getline (jFile, linestr)) {
  		std::cout << linestr << std::endl;
	}
	*/

	

	json gameconfig = json::parse(jFile)[boardName];
	if (gameconfig.contains("letters")) {
		this->hasLetters = true;
	}
	/*for (auto a: (gameconfig["imgpaths"]).get<std::vector<std::vector<std::string>>>()) {
		for (auto b: a) {
			std::cout << b << std::endl;
		}
	}*/
	
	this->initBoardItems(gameconfig);
	//std::cout << std::to_string(this->imgs.size()) << std::endl;
	//this->boardItems = std::vector<std::vector<int>>(userItems.size(),std::vector<int>(userItems[0].size(),0));
	//this->changeBoard(userItems);
	//this->boardItems = userItems;

	
	

	
	
	
}

}

