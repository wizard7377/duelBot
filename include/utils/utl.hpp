#pragma once

#include <dpp/dpp.h>
#include <vector>
#include <string>

using namespace dpp;

namespace utl {


class point {
	public:
		point(int inX,int inY) { this->x = inX; this->y = inY; }
		int x, y;
};

class bigSelect {

	public:
		bigSelect(std::vector<std::string> startVals);
		std::vector<component*> selectMenus;
		int curPage = 0;
		int numPage = 0;
		component pageUp();
		component pageDown();
		component pageStay();
		bool canPage[2] = {true,true};
	private:
		void changePage();
};

}
