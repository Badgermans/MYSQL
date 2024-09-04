#pragma once
#include <Parser.hpp>
#include <Objects.hpp>

#define debug


static vector<string> lexer(string str) {
	string token;
	vector<string> querytokens;
	for (int i = 0; i < str.size(); i++) {
		char flag = str[i];
		if (isalpha(flag)) {
			token.push_back(flag);
		}
		else if ((flag == ' ' || flag == ',') && !token.empty()) {
			querytokens.push_back(token);
			token.clear();
		}
		else if (flag == '(' || flag == ')') {
			querytokens.push_back(string(1, flag));
			token.clear();
		}
	}
	return querytokens;
}

static parseObj parser(string query, dataBase* db) {
#ifdef debug
	query = "INSERT INTO People (Forename, Surname, Age, Height, Alive)VALUES ('Rory', 'Price', (10 + 7), 1.75, true)";
#endif
	
	vector<string> tokens = lexer(query);
	vector<parseObj> ast = parse(tokens);

	return;
}

static vector<parseObj> parse(vector<string> tokens) {
	vector<parseObj> parsedObjs;
	if (tokens[0] == "INSERT") {
		parsedObjs.push_back(parseObj(GENERIC, (uint8_t*) new string("INSERT")));
	}
	else if (tokens[0] == "INTO") {
		parsedObjs.push_back(parseObj(GENERIC, (uint8_t*) new string("INTO")));
	}
	else if (tokens[0] == "DELETE") {
		parsedObjs.push_back(parseObj(GENERIC, (uint8_t*) new string("DELETE")));
	}
	else if (tokens[0] == "SELECT") {
		parsedObjs.push_back(parseObj(GENERIC, (uint8_t*) new string("SELECT")));
	}
	else if (tokens[0] == "UPDATE") {
		parsedObjs.push_back(parseObj(GENERIC, (uint8_t*) new string("UPDATE")));
	}
	else if (tokens[0] == "(") {
		int i = 1;
		vector<string> temp;
		while (tokens[i] != ")") {
			temp.push_back(tokens[i]);
			i++;
		}
		parseObj tempObj(LISTCOLS, (uint8_t*) & temp);
	}
}