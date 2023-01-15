#pragma once

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>

using namespace cv;
using json = nlohmann::json;

namespace dg {
class baseDrawGame {
	public:
		std::vector<std::vector<int>> boardItems;
		baseDrawGame(std::vector<std::vector<int>> userItems,std::string boardName,std::string conFile = "baseconfig.json");
	private:
		std::vector<std::vector<Mat>> imgs;

};

class basicDrawGame : public baseDrawGame {
	public:
		basicDrawGame(std::vector<std::vector<int>> userItems,std::string boardName,std::string conFile = "baseconfig.json");

};

class advDrawGame : public baseDrawGame {
	public:
		advDrawGame(std::vector<std::vector<int>> userItems,std::string boardName, std::string conFile = "baseconfig.json");

};








}

