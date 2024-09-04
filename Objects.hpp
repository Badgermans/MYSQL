#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdint.h>

using namespace std;


//enums
enum dataType {
	CHAR, VARCHAR, BOOL, INT, FLOAT
};

enum order {
	ASC, DESC
};

enum SQLOperationType {
	INSERT, UPDATE, DELETE, CREATE
};

enum objType {
	GENERIC, LISTCOLS, LISTVALS, SUBEXPR, QUERY
};

struct parseObj {
	uint8_t* value;
	objType type;

	parseObj(objType type, uint8_t* value) : type(type), value(value) {}
};

struct typedValue {
public:
	dataType type;
	void *value = nullptr;
	typedValue(void *val, dataType ty) {
		type = ty;
		value = val;
	}

	string getStringRep() const {
		switch (type) {
			case CHAR: {
				return string(1, *(char*)value);
			}
			case VARCHAR: {
				string* test = *(string**)value;
				if (*(string **)value == NULL) return string("EMPTY");
				return **(string **)value;
			}
			case BOOL:
				if (*(bool*)value) {
					return string("true");
				}
				else {
					return string("false");
				}
			case FLOAT: {
				ostringstream ss;
				ss << *(float*)value;
				return string(ss.str());
			}
			case INT: {
				ostringstream ss;
				ss << *(int*)value;
				return string(ss.str());
			}
		}
	}
};

struct columnInfo {
	string name;
	dataType type;
	int size = 0;
	int pos = 0;

	columnInfo(string nm, dataType ty) {
		name = nm;
		type = ty;
		switch (type) {
			case CHAR:    size = sizeof(char);    break;
			case BOOL:    size = sizeof(bool);    break;
			case VARCHAR: size = sizeof(string*); break;
			case INT:     size = sizeof(int);     break;
			case FLOAT:   size = sizeof(float);   break;
		}
	}
};

class row;

struct node {
	void* data = new void*;
	node* next = NULL;

	node(void* data) : data(data) {}
	node();

	~node();
};

class table;

class linkedList {
	int numNodes = 0;
public:
	node* head = NULL;
	linkedList(size_t size, int numNodes);
	~linkedList();
};

class table {
private:
	linkedList* data = NULL;
	vector<columnInfo> columns;
	int recs = 0;
	int stride = 0;
public:
	int getStride() const {
		return stride;
	}

	vector<columnInfo>& getColumns() {
		return columns;
	}

	table(vector<columnInfo> columns, int recs);
	uint8_t* getBase(int index);
	row operator[](int index);
	void printColumns();
	~table();
};

class row {
	uint8_t* ptr;
	int index = 0;
	table* parent;

public:
	row(uint8_t* ptr, table* parent, int index) : ptr(ptr), parent(parent), index(index) {}
	typedValue operator[](int index);

	void insert(vector<int> columns, vector<typedValue*> values);

	void print();

	row operator++(int);
	row operator--(int);
	operator bool();
};

class SQLOperation {
public:
	string query;
	SQLOperationType type;
	vector<string> columns;
	vector<typedValue> values;
	string table;
	vector<string> tables;
	string GROUPBY;
	order ORDER = ASC;
};

class dataBase {
private:
	vector<table>* tables;
	vector<SQLOperation> queries;
public:

};