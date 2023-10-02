#include "drawGame.hpp"
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

Mat * basicDrawGame::changeBoard(std::vector<std::vector<int>> newBoard) {
	
	
	
	int sizeX = newBoard.size(); //Size of board in squares
	int sizeY = newBoard[0].size();
	//Note, the reason this is such a odd color is for debugging purposes, that is to say, if a square is very oddly colored, it is probaly not being rendered correctly
	Mat * currentImg = new Mat(this->spaceSizeX * sizeX,this->spaceSizeY * sizeY,CV_8UC3,Scalar(50,100,250)); //Current img being displayed
	
	bool spaceColor = true;
	for (int i = 0; i < sizeX; i++) {
		for (int ii = 0; ii< sizeY; ii++) {
			Rect space(this->spaceSizeY*ii,this->spaceSizeX*i,this->spaceSizeY,this->spaceSizeX);
			
			Mat tarSpace = (*currentImg)(space);
			
			
			
			if (this->alternates) {
				(this->imgs[(int)(spaceColor)][newBoard[i][ii]]).copyTo(tarSpace);
				spaceColor = !(spaceColor);
			} else {
				this->imgs[0][newBoard[i][ii]].copyTo(tarSpace);
			}
			//TODO Fix letter implementation
			if (this->hasLetters) {
				if (i == 0) {
					for (int x = 0; x < this->spaceSizeX; x++) {
						for (int y = 0; y < this->spaceSizeY; y++) {
							if (this->letterImgs[0][ii].at<Vec3b>(y,x) != Vec3b(0,0,0)) tarSpace.at<Vec3b>(y,x) = this->letterImgs[0][ii].at<Vec3b>(y,x);
						}
					}
				} if (ii == 0) {
					for (int x = 0; x < this->spaceSizeX; x++) {
						for (int y = 0; y < this->spaceSizeY; y++) {
							if (this->letterImgs[1][i].at<Vec3b>(y,x) != Vec3b(0,0,0)) tarSpace.at<Vec3b>(y,x) = this->letterImgs[1][i].at<Vec3b>(y,x);
						}
					}
				} 
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
	//std::cout << std::format("Board name is: {}\n",boardName);
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

