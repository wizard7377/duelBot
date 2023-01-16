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
		virtual std::string getBoard(std::vector<std::vector<int>> newBoard) {return "";}

		
		//baseDrawGame(std::vector<std::vector<int>> userItems,std::string boardName,std::string conFile = "baseconfig.json");
	protected:
		void initBoardItems(std::vector<std::vector<std::string>> imagePaths);
		std::vector<std::vector<Mat>> imgs;
		

};

class basicDrawGame : public baseDrawGame {
	public:
		basicDrawGame(std::string boardName,std::string conFile = "basegameconfig.json");
		Mat * changeBoard(std::vector<std::vector<int>> newBoard);
		std::string getBoard(std::vector<std::vector<int>> newBoard);
};

class advDrawGame : public baseDrawGame {
	public:
		//advDrawGame(std::vector<std::vector<int>> userItems,std::string boardName, std::string conFile = "baseconfig.json");

};








}

