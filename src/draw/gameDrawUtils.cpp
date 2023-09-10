#include "drawGame.hpp"
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

using namespace cv;
namespace fs = std::filesystem;
using json = nlohmann::json;



std::string getFullPath(std::string filePath) {
	return (fs::current_path().parent_path() /= filePath);
}
std::string getFullPath(std::vector<std::string> filePath) {
	fs::path retPath = fs::current_path().parent_path();
	for (auto sub: filePath) {
		retPath /= sub;
	}
	return retPath.string();
}
namespace dg {


void baseDrawGame::initBoardItems(json boardJson) {
	this->spaceSizeX = boardJson["isize"][0].get<int>();
	this->spaceSizeY = boardJson["isize"][1].get<int>();
	this->alternates = (boardJson["cycle"].get<int>() > 1); 
	for (auto a: boardJson["files"]) {
		std::vector<Mat> tempVec;
		for (auto b: a["imgs"]) {
			std::vector<std::string> cPath = { a["path"] , (b.get<std::string>() + ".png") };
			Mat curImg = imread(getFullPath(cPath),IMREAD_COLOR);
			//imshow("Display window", curImg);
			tempVec.push_back(curImg);
		}
		this->imgs.push_back(tempVec);
	}

	if (this->hasLetters) {
		std::string letterPath = boardJson["letters"]; //Retrive directory of letters
		json letterInfo = json::parse(std::ifstream(getFullPath({letterPath,"letterConfig.json"}))); //Get the info on the files
		for (auto a : letterInfo["files"]) {
			std::vector<Mat> tempVec;
			for (auto b : a) {
				std::string cPath = getFullPath({ letterPath, (b.get<std::string>() + ".png") });	
				Mat curImg = imread(getFullPath(cPath),IMREAD_COLOR);
				
				tempVec.push_back(curImg);
			}
			this->letterImgs.push_back(tempVec);
		}
	}
}

}


