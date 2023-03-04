#pragma once

#include <dpp/dpp.h>
#include <unordered_map>
#include <thread>
#include <functional>
#include <string>
#include "databaselogic.hpp"

using namespace dpp;


namespace evt {
	class eventhandle {
		public:
			eventhandle(cluster* bot);
			bool addSelectCmd(std::string compid,std::function<void(const select_click_t&)> newCmd);
			bool addSlashCmd(std::string compid,std::function<void(const slashcommand_t&)> newCmd);
			bool addFormCmd(std::string compid,std::function<void(const form_submit_t&)> newCmd);
			bool addButtonCmd(std::string compid,std::function<void(const button_click_t&)> newCmd);
			
			bool deleteSelectCmd(std::string compid);
			bool deleteSlashCmd(std::string compid);
			bool deleteFormCmd(std::string compid);
			bool deleteButtonCmd(std::string compid);
		

			mData::dataHandle * testCon;

		private:
			std::unordered_map<std::string,std::function<void(const select_click_t&)>> selectCmds;
			std::unordered_map<std::string,std::function<void(const button_click_t&)>> buttonCmds;
			std::unordered_map<std::string,std::function<void(const slashcommand_t&)>> slashCmds;
			std::unordered_map<std::string,std::function<void(const form_submit_t&)>> formCmds;
			
	};
}
