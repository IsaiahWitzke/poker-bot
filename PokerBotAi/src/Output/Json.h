/* WORK ON ME IF REALLY NEEDED... RIGHT NOW IS JUST A WASTE OF TIME
#pragma once

#include "Printable.h"
#include <string>
#include <vector>

using namespace std;

class JsonException {
public:
	const string e;
	JsonException(string e = "Json Error") : e{e} {}
	~JsonException() {}
}

class Json : public Printable {
private:
	Json::Type t;
	map<Json *> children;
	string vStr;
	int vInt;
	float vFloat;

public:
	enum Type {
		OBJECT,
		ARRAY,
		VALUE,
		STRING,
		INT,
		FLOAT
	};

	Json(string v);
	Json(int v);
	Json(float v);
	Json
	~Json();

	/**
	 * @brief add Json object to the tree
	 * @note will delete reference to obj when this goes out of scope
	 * 
	 * @param obj
	 * /
	void push_back(Json *obj);

	void emplace_back
};

Json::push_back(Json *obj) {
	this->children.push_back(obj);
}

Json::emplace_back() {
	this->children.push_back(obj);
}
*/