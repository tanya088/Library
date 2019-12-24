#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <windows.h>

using namespace std;

#include "BankForUser.h"

User signIn(BankForUser bank);
void startSession(BankForUser bank, User user);
void printHelp();
string getCommand(string *cmd);
void printBook(Book b);

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	BankForUser bank;
	User user = signIn(bank);
	startSession(bank, user);

	system("pause");
	return 0;
}


void startSession(BankForUser bank, User user) {
	HANDLE tsvt = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bazovyi;
	GetConsoleScreenBufferInfo(tsvt, &bazovyi);
	tsvt = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	string cmd = "";
	cout << "��� �������:" << endl;
	printHelp();
	do {
		SetConsoleTextAttribute(console, FOREGROUND_GREEN);
		cout << "[cmd]~ ";
		SetConsoleTextAttribute(tsvt, bazovyi.wAttributes);
		getline(cin, cmd);
		string c = getCommand(&cmd);
		user = bank.getUser(user.getId()); // �������� ���������� � ����

		if (c == "books") {
			if (bank.getBooks().empty()) {
				cout << "� ���������� ��� ����!" << endl;
				continue;
			}
			c = getCommand(&cmd);
			if (c == "-my") {
				if (user.getIdBooks().empty()) {
					cout << "� ��� ��� �������� ����!" << endl;
				} else {
					for (int id : user.getIdBooks()) {
						try {
							Book b = bank.getBook(id);
							printBook(b);
						}
						catch (string e) {
							cout << e << endl;
						}
					}
				}
			} else if (c == "-a") {
				for (Book b : bank.getBooks()) {
					if (b.getAuthor() == cmd) {
						printBook(b);
					}
				}
			} else if (c == "-g") {
				for (Book b : bank.getBooks()) {
					if (b.getGenre() == cmd) {
						printBook(b);
					}
				}
			} else {
				for (Book b : bank.getBooks()) {
					printBook(b);
				}
			}
		}
		else if (c == "help") {
			printHelp();
		}
		else if (c == "request") {
			try {
				bank.request(atoi(cmd.c_str()), user.getId());
			} catch (string e) {
				cout << e << endl;
			} catch (...) {
				cout << "���-�� �� ���!" << endl;
			}
		}
		else if (c == "return") {
			try {
				bank.returnBook(atoi(cmd.c_str()), user.getId());
			}
			catch (string e) {
				cout << e << endl;
			}
			catch (...) {
				cout << "���-�� �� ���!" << endl;
			}
		}
		else if (c == "exit") {
			break;
		}
		else {
			cout << "������� �� �������!" << endl;
		}

	} while (cmd != "exit");
}

User signIn(BankForUser bank) {
	int i;
	do {
		cout << "����(1) | �����������(2): ";
		if (!(cin >> i)) {
			cin.clear();
			while (cin.get() != '\n') continue;
		}
	} while (i != 1 && i != 2);
	cin.get();

	if (i == 1) {
		string login, password;
		while (true) {
			cout << "������� �����: ";
			getline(cin, login);
			cout << "������� ������: ";
			getline(cin, password);
			try {
				return bank.signIn(login, password);
			} catch (string e) {
				cout << e << endl;
			}
		};
	} else {
		string login, password, name, phone;
		int birth;
		while (true) {
			cout << "���������� �����: ";
			getline(cin, login);
			cout << "���������� ������: ";
			getline(cin, password);
			cout << "���� ���: ";
			getline(cin, name);
			cout << "��� �������: ";
			cin >> birth;
			cin.get();
			cout << "��� ����� ��������: ";
			getline(cin, phone);
			try {
				return bank.registerUser(login, password, name, birth, phone);
			}
			catch (string e) {
				cout << e << endl;
			}
		}
	}
}

void printHelp() {
	cout << "\thelp [������� �� ��������]" << endl;
	cout << "\tbooks [������ ���� ����]" << endl;
	cout << "\tbooks -my [������ ������ ����]" << endl;
	cout << "\tbooks -a <author> [������ ���� �� ������]" << endl;
	cout << "\tbooks -g <genre> [������ ���� �� �����]" << endl;
	cout << "\trequest <id book> [��������� �����]" << endl;
	cout << "\treturn <id book> [������� �����]" << endl;
	cout << "\texit [��������� ������]" << endl;
}

void printBook(Book b) {
	cout << "[" << b.getId() << "]"
		<< " \"" << b.getName() << "\""
		<< ", �����: " << b.getAuthor()
		<< ", ����������: " << b.getCount()
		<< ", ����: " << b.getGenre()
		<< ", ���: " << b.getYear() << endl;
}

string getCommand(string *cmd) {
	string c = "";
	int i;
	for (i = 0; i < (*cmd).length() && (*cmd)[i] != ' '; i++) {
		c += (*cmd)[i];
	}
	*cmd = (*cmd).erase(0, ++i);
	return c;
}