#include "utl.hpp"
#include <dpp/dpp.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace dpp;

static unsigned long long int currentSelect = 1;
std::string genName() {
    currentSelect++;
    return std::to_string(currentSelect);
}
namespace utl {

bigSelect::bigSelect(std::vector<std::string> startVals) {
	component * curCom;

	//this solution is horrible but i'll change it later
	std::remove_if(startVals.begin(),startVals.end(), 
	[](std::string inStr) { return (inStr == ""); }
	);
	for (auto a : startVals) {
		std::cout << a;
		std::cout << " , ";
	} 
	std::cout << std::endl;
	

    if (startVals.size() > 0) { 
		for (int i = 0; i < startVals.size(); i++) {

			if ((i % 25) == 0) {
				if (i != 0) {
					this->selectMenus.push_back(curCom);
					this->numPage++;
				}
				curCom = new component();
				curCom->set_type(cot_selectmenu);
				curCom->set_placeholder("???");
				curCom->set_id(genName());
			}
			curCom->add_select_option(select_option("move from: ", startVals[i],startVals[i]));



		}
	} else {		
		curCom = new component();
		curCom->set_type(cot_selectmenu);
		curCom->set_placeholder("???");
		curCom->set_id(genName());
		curCom->add_select_option(select_option("No options", "N/A","N/A"));
	}
			

	this->selectMenus.push_back(curCom);
	
    this->changePage();

}

component bigSelect::pageStay() {
    this->changePage();
	return (*(this->selectMenus[this->curPage]));
}
component bigSelect::pageDown() {
    this->curPage++;
    this->changePage();
	return (*(this->selectMenus[this->curPage]));
}
component bigSelect::pageUp() {
    this->curPage--;
    this->changePage();
	return (*(this->selectMenus[this->curPage]));
}

void bigSelect::changePage() {
    
    if (this->numPage == 0) {
        this->canPage[0] = false;
        this->canPage[1] = false;
    } else if (this->curPage == 0) {
        this->canPage[0] = false;
        this->canPage[1] = true;
    } else if (this->curPage == this->numPage) {
        this->canPage[0] = true;
        this->canPage[1] = false;
    } else {
        this->canPage[0] = true;
        this->canPage[1] = true;
    }
}


}

