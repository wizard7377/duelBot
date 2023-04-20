#include "databaselogic.hpp"
#include <iostream>
#include <mysql.h>
#include <thread>
#include <functional>
#include <exception>
#include "colorstuff.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>


extern std::string getFullPath(std::string filePath);
extern std::string getFullPath(std::vector<std::string> filePath);



using json = nlohmann::json;

using namespace std;
namespace fs = std::filesystem;








class noResult : public exception {};

namespace mData {



dataHandle::dataHandle() {
	this->dataCon = NULL;
	std::ifstream jFile(getFullPath("secrets/config.json"));
	json gameConfig = json::parse(jFile)["MYSQL"];
	try {
		mysql_library_init(0,NULL,NULL);
		this->dataCon = mysql_init(this->dataCon);
	} catch (...) {
		std::cout << "error occured in database init \n";
		exit(0);
	}
	std::cout << "Database lib init succeded" << std::endl;

	//CLEAN
	//CLEAN JSON ACCESS UP
	if (mysql_real_connect(this->dataCon,gameConfig["MYOP_HOST"].get<string>().c_str(),gameConfig["MYOP_USER"].get<string>().c_str(),gameConfig["MYOP_PASS"].get<string>().c_str(),gameConfig["MYOP_DATA"].get<string>().c_str(),gameConfig["MYOP_PORT"],NULL,gameConfig["MYOP_FLAG"]) == NULL) throw noResult();
	
		
	
	if (mysql_stat(this->dataCon) == NULL) { 
		std::cout << "connection failed\n";
		throw 0; 
	} else {
		std::cout << "Final checks succeded\n";
	}
	
	//this->dataCon = NULL;
 
}

reSet dataHandle::makeGUserRaw(int gameId,uint64_t totalId) {
	reSet retAc = {};
	try {
		std::string curQ = ("INSERT INTO userGameInfo (userId, gameId) VALUES (" + to_string(totalId) + ", " + to_string(gameId) + ");");	
		mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		retAc = {std::to_string(mysql_insert_id(this->dataCon))};
	} catch (...) {
		retAc = {nullptr};
		std::cout << colorForm("An error has occured\n",RED_TERM);

	}
	return retAc;
			
			
		
}

std::vector<std::vector<std::string>> dataHandle::execQ(std::string query, bool retType) {
	try {
		std::vector<std::vector<std::string>> retVec = {};
		mysql_real_query(this->dataCon,query.c_str(),query.length());
		MYSQL_RES * result = mysql_store_result(this->dataCon);
		if (result != 0) {
		if (mysql_num_rows(result) > 0) {

			for (int i = 0; i < mysql_num_rows(result); i++) { 
				retVec.push_back({});
				MYSQL_ROW resRow = mysql_fetch_row(result);
				for (int ii = 0; ii < mysql_field_count(this->dataCon); ii++) { retVec[ii].push_back(resRow[i]); }
			}
		}

		if (retType) {
			retVec.push_back({
				std::to_string(mysql_insert_id(this->dataCon)),
				std::to_string(mysql_num_rows(result)),
				std::string(mysql_error(this->dataCon))
			});
		}
		} else {
			throw noResult();
		}
		mysql_free_result(result);
		return retVec;
	} catch (noResult) {
		throw;
	} catch (...) { 
		std::cout << "A error has occured\n"; 
		//return (std::vector<std::vector<std::string>>({}));
		throw exception();
	}
	
	



}

reSet dataHandle::getUser(uint64_t userId, uint64_t guildId) {
	std::string curQ = "";
	reSet retAc;
	if (guildId > 0) {
		try {
			curQ = ("SELECT duelId FROM userGuildIds WHERE guildId = "  + std::to_string(guildId) + " AND userId = " + std::to_string(userId) + ";");
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
			MYSQL_RES * result = mysql_store_result(this->dataCon);
			if (result != 0) {
			if (mysql_num_rows(result) > 0) {
				
				MYSQL_ROW resRow = mysql_fetch_row(result);
				//std::cout << mysql_num_rows(result) << std::endl;
				//std::cout << mysql_num_fields(result) << std::endl;
				//idk why the -48 is needed 
				retAc.push_back(std::stoull(resRow[0]));
				mysql_free_result(result);
				return retAc;
			} else {
				curQ = ("INSERT INTO userGuildIds (userId,guildId) VALUES (" + std::to_string(userId) + "," + std::to_string(guildId) + ");");
				mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
				
				retAc.push_back(mysql_insert_id(this->dataCon));
				mysql_free_result(result);
				return retAc;
			}
			}
				
		
		} catch (...) {
			std::cout << "An error has occured \n";
		}



	} else {
		try {
			curQ = ("SELECT duelId FROM userGuildIds WHERE guildId IS NULL AND userId = " + std::to_string(userId) + ";");
			//std::cout << curQ << std::endl;
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
			MYSQL_RES * result = mysql_store_result(this->dataCon);
			if (result != 0) {
			if (mysql_num_rows(result) > 0) {
				
				MYSQL_ROW resRow = mysql_fetch_row(result);
				//std::cout << mysql_num_rows(result) << std::endl;
				//std::cout << mysql_num_fields(result) << std::endl;
				retAc.push_back(std::stoull(resRow[0]));
				mysql_free_result(result);
				return retAc;

			} else {
				curQ = ("INSERT INTO userGuildIds (userId) VALUES (" + std::to_string(userId) + ");");
				mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
				retAc.push_back(mysql_insert_id(this->dataCon));
				mysql_free_result(result);
				return retAc;
			
			}
			} else {
				mysql_free_result(result);
				throw noResult();
			}

				
		
		} catch (noResult) {
			throw;
		} catch (...) {
			std::cout << "An error has occured \n";
		}





	}

	return {(uint64_t)(0)};
}

//if negative, should be hidden

reSet dataHandle::getRate(int gameId,uint64_t userId,uint64_t guildId) {
	reSet retAc;
	uint64_t totalId = get<uint64_t>(this->getUser(userId,guildId)[0]);
	std::string curQ = "";
	MYSQL_RES * result;
	try {
		if (gameId < 0) {
			curQ = ("SELECT gameRate, showRate FROM userGameInfo WHERE gameId IS NULL AND userId = " + std::to_string(totalId) + ";");
		} else {
			curQ = ("SELECT gameRate, showRate FROM userGameInfo WHERE gameId = " + std::to_string(gameId) + " AND userId = " + std::to_string(totalId) + ";");
		}
		mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		result = mysql_store_result(this->dataCon);
		
		if (mysql_num_rows(result) <= 0) {
			string tempCurQ = ("INSERT INTO userGameInfo (userId, gameId) VALUES (" + to_string(totalId) + ", " + to_string(gameId) + ");");
			mysql_real_query(this->dataCon,tempCurQ.c_str(),tempCurQ.length());
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
			//could props optimize with last insert but too lazy rn
			result = mysql_store_result(this->dataCon);
			//std::cout << "res is: " << result << std::endl;
			
		}
		MYSQL_ROW curRow = mysql_fetch_row(result);
		if (curRow == NULL) {
			//std::cout << "Retrived last\n";
		}
		retAc.push_back(atoi(curRow[0]));
		bool isAllow = false;
		std::function<bool(uint64_t,int)> canSee; 
		
		canSee = ([&] (uint64_t guildIdPar,int gameIdPar) {
			//std::cout << "\n val is null \n\n";
			bool isGuildDef = (guildIdPar != 0);
			bool isGameDef = (gameIdPar < 0);
			if (not (isGameDef or isGuildDef)) {
					return true;
				
			} else if (((not isGameDef) and isGuildDef) or  (isGameDef and (not isGuildDef))) {

				uint64_t tempTotalId = get<uint64_t>(this->getUser(userId)[0]);
				string tempCurQT = ("SELECT showRate FROM userGameInfo WHERE gameId IS NULL AND userId = " + std::to_string(tempTotalId) + ";");
				mysql_real_query(this->dataCon,tempCurQT.c_str(),tempCurQT.length());
				result = mysql_store_result(this->dataCon);
				curRow = mysql_fetch_row(result);
				if (curRow[0] == NULL) {
					return true;
				} else {
					return (*curRow[0] != '0');

					//if (
				}
					
			} else {
				return canSee(guildIdPar,-1);
			}

		});

		if (curRow[1] == NULL) {
			isAllow = canSee(guildId,gameId);
		} else {
			isAllow = (*curRow[1] != '0');
		}
		
		


		
		retAc.push_back(isAllow);
		mysql_free_result(result);
		return (retAc);
		

	} catch (...) {
		return {0};

	}
	

	
}





void dataHandle::setRate(int reqRate,int gameId,uint64_t userId,uint64_t guildId) {
	reSet retAc;
	uint64_t totalId = get<uint64_t>(this->getUser(userId,guildId)[0]);
	std::string curQ = "";
	MYSQL_RES * result;
	try {
		if (gameId < 0) {
			curQ = ("SELECT gameRate, showRate FROM userGameInfo WHERE gameId IS NULL AND userId = " + std::to_string(totalId) + ";");
		} else {
			curQ = ("SELECT gameRate, showRate FROM userGameInfo WHERE gameId = " + std::to_string(gameId) + " AND userId = " + std::to_string(totalId) + ";");
		}
		
		mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		result = mysql_store_result(this->dataCon);
		
		if (mysql_num_rows(result) <= 0) {
			string tempCurQ = ("INSERT INTO userGameInfo (userId, gameId) VALUES (" + to_string(totalId) + ", " + to_string(gameId) + ");");
			mysql_real_query(this->dataCon,tempCurQ.c_str(),tempCurQ.length());
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
			//could props optimize with last insert but too lazy rn
			result = mysql_store_result(this->dataCon);
			//std::cout << "res is: " << result << std::endl;
			
		}

		curQ = ("UPDATE userGameInfo SET gameRate = " + std::to_string(reqRate) + " WHERE userId = " + std::to_string(totalId) + " AND gameId = " + std::to_string(gameId) + ([&] { if (guildId != 0) { return ("AND guildId = " + std::to_string(guildId)); } else { return std::string(""); }})() + ";"); 
		//std::cout << colorForm("curQ is: ",CYAN_TERM) << colorForm(curQ,PURPLE_TERM) << std::endl;
		//std::cout << curQ << std::endl;
		mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		
		
		
		
		

		
		
		


		
		
		
		return;
	} catch (...) {
		return;

	}
	

	
}

//this
//global
//both
//all
//allbut
bool dataHandle::editSetting (string setName, string val, int gameId, uint64_t userId, uint64_t guildId, int scopeSet) {

	try {
		string curQ = "SELECT duelId FROM userGuildIds WHERE ";

		if (scopeSet == 0) { curQ.append(("userId = " + to_string(userId) + " AND guildId = " + to_string(guildId) + ";")); } 
		else if (scopeSet == 1) { curQ.append(("userId = " + to_string(userId) + " AND guildId IS NULL;")); } 
		else if (scopeSet == 2) { curQ.append(("userId = " + to_string(userId) + " AND (guildId = " + to_string(guildId) + " OR guildId IS NULL);")); } 
		else if (scopeSet == 3) { curQ.append(("userId = " + to_string(userId) + ";")); } 
		else if (scopeSet == 4) { curQ.append(("userId = " + to_string(userId) + " AND guildId IS NOT NULL;")); } 
		else { return false; }

		//std::cout << "\n curQ cur Is: " << curQ << std::endl;
		mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		MYSQL_RES * result = mysql_store_result(this->dataCon);
		MYSQL_ROW curRow;
		while ((curRow = mysql_fetch_row(result)) != NULL) {
			//std::cout << string(curRow[0]) << std::endl;
			curQ = ("UPDATE userGameInfo SET " + setName + " = " + val + " WHERE userId = " + string(curRow[0]) + " AND gameId = " + to_string(gameId) + ";"); 
			//std::cout << curQ << std::endl;
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		}
		return true;
	
	} catch (...) { return false; }




}
}
