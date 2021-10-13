#include <fstream>
#include "Printable.h"

Printable::Printable() {}
Printable::~Printable() {}

ostream& operator << (ostream& out, const Printable& pObj) {
	out << pObj.toString();	// to be implemented by inheritors
	return out;
}

void Printable::writeToFile(string filePath) const {
    ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << *this;
        outFile.close();
    }
    else {
		throw std::runtime_error("Unable to open file " + filePath);
    }
}