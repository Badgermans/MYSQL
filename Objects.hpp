#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

enum dataType {
	CHAR, //0, 1 byte
	VARCHAR, //1, 4 bytes (is a pointer)
	BOOL, //2, 1 byte
	SMALLINT, //3, 2 bytes
	MEDIUMINT, //4, 3 bytes
	INT, //5, 4 bytes
	BIGINT, //6, 12 bytes
	FLOAT //7, 4 bytes
};

struct columnInfo {
	string name;
	dataType type;
};

class table {
private:

	void* data = NULL;

public:
	table(vector<columnInfo> columns) {
		int size = 5;
		for (int i = 0; i < columns.size(); i++) {
			switch (columns[i].type) {
			case CHAR:
				size += 1;
				break;
			case VARCHAR:
				size += 4;
				break;
			case BOOL:
				size += 1;
				break;
			case SMALLINT:
				size += 2;
				break;
			case MEDIUMINT:
				size += 3;
				break;
			case INT:
				size += 4;
				break;
			case BIGINT:
				size += 12;
				break;
			case FLOAT:
				size += 4;
				break;
			}
		}
		size *= 50;
		data = malloc(size);
	}
};