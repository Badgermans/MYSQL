#include <iostream>
#include <sstream>

using namespace std;


static string getQuery() {

	cout << "Enter your query" << endl;

	string line;
	string query;
	bool queryRun = false;

	while (!queryRun) {
		while (1) {
			getline(cin, line);

			query.append(line + '\n');

			if (line.back() == ';')
				break;
		}

		cout << "Do you wish to run this:\n---------------------------------\n" 
			<< query << "\n---------------------------------\ntype y/n" << endl;

		char ans = ' ';

		cin >> ans;

		if (ans == 'y') {
			queryRun = true;
		}
		else {

			cout << "Do you wish to exit? type y/n" << endl;

			cin >> ans;

			if (ans == 'y') {
				cout << "exiting program..." << endl;
				exit(0);
			}
			else {
				cout << "Enter in your new query" << endl;
				query.clear();
				cin.ignore();
			}
		}
	}

	return query;
}

int main() {
	try {
		string query = getQuery();


	}
	catch (exception except) {
		cout << except.what() << endl;
	}
	catch (...) {
		
	}
}