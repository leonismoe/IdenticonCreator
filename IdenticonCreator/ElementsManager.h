#ifndef _ELEMENTS_MANAGER_H
#define _ELEMENTS_MANAGER_H

#include <vector>

class ElementsManager {
	
public:
	const static enum ETYPES { NONE, BUTTON, TEXTINPUT, CANVAS };
	const static int getIndexByID(int id);
	const static ETYPES getTypeByID(int id);
	const static int createElement(ETYPES type);
	const static bool removeElement(int id);
	const static int getElementsNumber();

	struct ElementWrapper {
		int id;
		ETYPES type;
	};

private:
	static int elementNums;
	static std::vector<ElementWrapper> elementsContainer;

};

#endif
