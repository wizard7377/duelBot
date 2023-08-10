#pragma once

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>


using namespace cv;
using json = nlohmann::json;
namespace dg {
/**
 * @brief Basic interface to openCV for drawing game boards
 * 
 */
class baseDrawGame {
	public:
		virtual std::string getBoard(std::vector<std::vector<int>> newBoard) {return "";}

		
		//baseDrawGame(std::vector<std::vector<int>> userItems,std::string boardName,std::string conFile = "baseconfig.json");
	protected:
		void initBoardItems(json imagePaths);
		std::vector<std::vector<Mat>> imgs;
		std::vector<std::vector<Mat>> letterImgs;
		bool hasLetters = false;
		bool alternates = false;	

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

