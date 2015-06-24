#include "ElementsManager.h"

int ElementsManager::elementNums = 0;
std::vector<ElementsManager::ElementWrapper> ElementsManager::elementsContainer;

const int ElementsManager::getIndexByID(int id) {
	std::vector<ElementWrapper>::iterator it;
	for (it = elementsContainer.begin(); it != elementsContainer.end(); ++it) {
		if (it->id == id) return it - elementsContainer.begin();
	}
	return -1;
}

const ElementsManager::ETYPES ElementsManager::getTypeByID(int id) {
	int index = getIndexByID(id);
	return index > -1 ? elementsContainer[index].type : ETYPES::NONE;
}

const int ElementsManager::createElement(ETYPES type) {
	int id = ++elementNums;
	ElementWrapper element = { id, type };
	elementsContainer.push_back(element);
	return id;
}

const bool ElementsManager::removeElement(int id) {
	int index = getIndexByID(id);
	if (index < 0 || elementsContainer[index].type == ETYPES::NONE) {
		return false;
	}
	elementsContainer[index].type = ETYPES::NONE;
	return true;
}

const int ElementsManager::getElementsNumber() {
	return elementNums;
}
