#include "drawgame.hpp"
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
	return (fs::current_path() /= filePath);
}
std::string getFullPath(std::vector<std::string> filePath) {
	fs::path retPath = fs::current_path();
	for (auto sub: filePath) {
		retPath /= sub;
	}
	return retPath.string();
}
namespace dg {


void baseDrawGame::initBoardItems(std::vector<std::vector<std::string>> imagePaths) {
	for (auto a: imagePaths) {
		std::vector<Mat> tempVec;
		for (auto b: a) {
			tempVec.push_back(imread(getFullPath(b),IMREAD_COLOR));
		}
		this->imgs.push_back(tempVec);
	}
}

}


