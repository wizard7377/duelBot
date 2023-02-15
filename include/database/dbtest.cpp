#include "databaselogic.hpp"
#include <iostream>
#include <mysql.h>
#include "config.hpp"
#include <thread>


	

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

uint64_t dataHandle::getUser(uint64_t userId, uint64_t guildId) {
	std::string curQ = "";
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
				return std::stoull(resRow[0]);
			} else {
				curQ = ("INSERT INTO userGuildIds (userId,guildId) VALUES (" + std::to_string(userId) + "," + std::to_string(guildId) + ");");
				mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
				
				return mysql_insert_id(this->dataCon);
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
				
				return std::stoull(resRow[0]);
			} else {
				curQ = ("INSERT INTO userGuildIds (userId) VALUES (" + std::to_string(userId) + ");");
				mysql_real_query(this->dataCon,curQ.c_str(),curQ.length());
				
				return mysql_insert_id(this->dataCon);
			}
				
		
		} catch (...) {
			std::cout << "An error has occured \n";
		}



	}

	return 2635534;
}




}
