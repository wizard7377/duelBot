#pragma once

#include <dpp/dpp.h>
#include <vector>
#include <string>

using namespace dpp;

namespace utl {


class point {
	public:
		point(int inX,int inY,int inSize = 64);
		point(int inIndex,int inSize, bool xIsOne);
		std::string tString();
		int getVal(bool xIsOne = false);
		int x, y, size;
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
