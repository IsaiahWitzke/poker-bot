#pragma once

#include <string>

using namespace std;

class Printable {
public:
	Printable();
	~Printable();

	virtual string toString(int tabSpaces = 0) const = 0;
 
	void writeToFile(string filePath) const;
};
