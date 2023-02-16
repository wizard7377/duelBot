#include "databaselogic.hpp"
#include <iostream>
#include <mysql.h>
#include "config.hpp"
#include <thread>
#include <functional>

using namespace std;
	

namespace mData {

dataHandle::dataHandle() {
	this->dataCon = NULL;
	try {
		mysql_library_init(0,NULL,NULL);
		this->dataCon = mysql_init(this->dataCon);
	} catch (...) {
		std::cout << "error occured in database init \n";
		exit(0);
	}
	std::cout << "Database init succeded" << std::endl;
	try {
		mysql_real_connect(this->dataCon,MYOP_HOST,MYOP_USER,MYOP_PASS,MYOP_DATA,MYOP_PORT,MYOP_SOCK,MYOP_FLAG);
	} catch (...) {
		std::cout << "Error conn \n";
	}
	
	//this->dataCon = NULL;
 
}


reSet dataHandle::getUser(uint64_t userId, uint64_t guildId) {
	std::string curQ = "";
	reSet retAc;
	if (guildId > 0) {
		try {
			curQ = ("SELECT duelId FROM userGuildIds WHERE guildId = "  + std::to_string(guildId) + " AND userId = " + std::to_string(userId) + ";");
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
			MYSQL_RES * result = mysql_store_result(this->dataCon);
			
			if (mysql_num_rows(result) > 0) {
				
				MYSQL_ROW resRow = mysql_fetch_row(result);
				std::cout << mysql_num_rows(result) << std::endl;
				std::cout << mysql_num_fields(result) << std::endl;
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
				
		
		} catch (...) {
			std::cout << "An error has occured \n";
		}



	} else {
		try {
			curQ = ("SELECT duelId FROM userGuildIds WHERE guildId IS NULL AND userId = " + std::to_string(userId) + ";");
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
			MYSQL_RES * result = mysql_store_result(this->dataCon);
			
			if (mysql_num_rows(result) > 0) {
				
				MYSQL_ROW resRow = mysql_fetch_row(result);
				std::cout << mysql_num_rows(result) << std::endl;
				std::cout << mysql_num_fields(result) << std::endl;
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
				
		
		} catch (...) {
			std::cout << "An error has occured \n";
		}



	}

	return {0};
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
			std::cout << "res is: " << result << std::endl;
			
		}
		MYSQL_ROW curRow = mysql_fetch_row(result);
		if (curRow == NULL) {
			std::cout << "Retrived last\n";
		}
		retAc.push_back(atoi(curRow[0]));
		bool isAllow = false;
		std::function<bool(uint64_t,int)> canSee; 
		
		canSee = ([&] (uint64_t guildIdPar,int gameIdPar) {
			std::cout << "\n val is null \n\n";
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

		std::cout << "\n curQ cur Is: " << curQ << std::endl;
		mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		MYSQL_RES * result = mysql_store_result(this->dataCon);
		MYSQL_ROW curRow;
		while ((curRow = mysql_fetch_row(result)) != NULL) {
			std::cout << string(curRow[0]) << std::endl;
			curQ = ("UPDATE userGameInfo SET " + setName + " = " + val + " WHERE userId = " + string(curRow[0]) + " AND gameId = " + to_string(gameId) + ";"); 
			std::cout << curQ << std::endl;
			mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
		}
		return true;
	
	} catch (...) { return false; }




}
}
