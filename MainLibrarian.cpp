#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <Windows.h>

#include "WorkBooks.h"
#include "WorkUsers.h"

using namespace std;

int statusWork();
void startSession(WorkBooks bank);
void startSession(WorkUsers bank);
void printHelpBooks();
void printHelpUsers();
void printUserInfo(Task t);
void printBook(Book b, WorkBooks bank);
void printBookInfo(Task t);
string getCommand(string* cmd);

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if (statusWork() == 1) {
		WorkBooks bank;
		startSession(bank);
	} else {
		WorkUsers bank;
		startSession(bank);
	}

	system("pause");
	return 0;
}


void startSession(WorkBooks bank) {
	HANDLE tsvt = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bazovyi;
	GetConsoleScreenBufferInfo(tsvt, &bazovyi);
	tsvt = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	string cmd = "";
	cout << "��� �������:" << endl;
	printHelpBooks();
	do {
		SetConsoleTextAttribute(console, FOREGROUND_GREEN);
		cout << "[cmd]~ ";
		SetConsoleTextAttribute(tsvt, bazovyi.wAttributes);
		getline(cin, cmd);
		string c = getCommand(&cmd);

		if (c == "books") {
			if (bank.getBooks().empty()) {
				cout << "� ���������� ��� ����!" << endl;
				continue;
			}
			c = getCommand(&cmd);
			if (c == "-f") {
				for (Book b : bank.getBooks()) {
					printBook(b, bank);
				}
			}
			else if (c == "-a") {
				for (Book b : bank.getBooks()) {
					if (b.getAuthor() == cmd) {
						printBook(b, bank);
					}
				}
			}
			else if (c == "-g") {
				for (Book b : bank.getBooks()) {
					if (b.getGenre() == cmd) {
						printBook(b, bank);
					}
				}
			}
			else {
				for (Book b : bank.getBooks()) {
					if (b.getCount() > 0) {
						printBook(b, bank);
					}
				}
			}
		}
		else if (c == "book") {
			try {
				Book b = bank.getBook(atoi(cmd.c_str()));
				for (auto t : bank.getTasks()) {
					if (t.getIdBook() == b.getId()) {
						printBookInfo(t);
					}
				}
			}
			catch (string e) {
				cout << e << endl;
			}
			catch (...) {
				cout << "���-�� �� ���!" << endl;
			}
		} else if (c == "help") {
			printHelpBooks();
		} else if (c == "add") {
			c = getCommand(&cmd);
			try {
				int n = atoi(cmd.c_str());
				bank.addBook(atoi(c.c_str()), n);
			}
			catch (string e) {
				cout << e << endl;
			}
			catch (...) {
				cout << "���-�� �� ���!" << endl;
			}
		}
		else if (c == "new") {
			string name, auhtor, publishing, genre;
			int year, count;
			while (true) {
				cout << "�������� �����: ";
				getline(cin, name);
				cout << "����� �����: ";
				getline(cin, auhtor);
				cout << "������������: ";
				getline(cin, publishing);
				cout << "��� �������: ";
				cin >> year;
				cin.get();
				cout << "���� �����: ";
				getline(cin, genre);
				cout << "���������� ����: ";
				cin >> count;
				cin.get();
				try {
					bank.newBook(name, auhtor, publishing, genre, year, count);
					cout << "������!" << endl;
					break;
				}
				catch (string e) {
					cout << e << endl;
				}
			}
		}
		else if (c == "delete") {
			c = getCommand(&cmd);
			try {
				int n = atoi(cmd.c_str());
				bank.deleteBook(atoi(c.c_str()), n);
			}
			catch (string e) {
				cout << e << endl;
			}
			catch (...) {
				cout << "���-�� �� ���!" << endl;
			}
		} else if (c == "exit") {
			break;
		} else {
			cout << "������� �� �������!" << endl;
		}

	} while (cmd != "exit");
}

void startSession(WorkUsers bank) {
	HANDLE tsvt = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bazovyi;
	GetConsoleScreenBufferInfo(tsvt, &bazovyi);
	tsvt = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	string cmd = "";
	cout << "��� �������:" << endl;
	printHelpUsers();
	do {
		SetConsoleTextAttribute(console, FOREGROUND_GREEN);
		cout << "[cmd]~ ";
		SetConsoleTextAttribute(tsvt, bazovyi.wAttributes);
		getline(cin, cmd);
		string c = getCommand(&cmd);

		if (c == "help") {
			printHelpUsers();
		}
		else if (c == "tasks") {
			c = getCommand(&cmd);
			if (c == "-p") {
				for (auto t : bank.getTasksProcessed()) {
					cout << "[" << t.getId() << "] id_book: " << t.getIdBook() << ", id_user: " << t.getIdUser() << ", status: PROCESSED" << endl;
				}
			} else {
				for (auto t : bank.getTasks()) {
					Status status = t.getStatus();
					string s;
					if (status == PROCESSED) s = "PROCESSED";
					if (status == ALLOWED) s = "ALLOWED";
					if (status == REJECTED) s = "REJECTED";
					if (status == RETURNED) s = "RETURNED";
					cout << "[" << t.getId() << "] id_book: " << t.getIdBook() << ", id_user: " << t.getIdUser() << ", status: " << s << endl;
				}
			}
		}
		else if (c == "allowed") {
			try {
				Task task = bank.getTask(atoi(cmd.c_str()));
				if (task.getStatus() == PROCESSED) {
					User u = bank.getUser(task.getIdUser());
					Book b = bank.getBook(task.getIdBook());
					if (b.getCount() < 1) throw (string)"����� �����������!";
					u.addBook(task.getIdBook());
					b.giveBook();
					task.setStatus(ALLOWED);
					bank.writeBooks(b);
					bank.writeTasks(task);
					bank.writeUsers(u);
					cout << "����� ������ ������������!" << endl;
				} else {
					throw (string)"������ ��� ����������!";
				}
			} catch (string e) {
				cout << e << endl;
			}
		}
		else if (c == "rejected") {
			try {
				Task t = bank.getTask(atoi(cmd.c_str()));
				t.setStatus(REJECTED);
				bank.writeTasks(t);
				cout << "� ������ ��������!" << endl;
			}
			catch (string e) {
				cout << e << endl;
			}
		}
		else if (c == "info") {
			try {
				User u = bank.getUser(atoi(cmd.c_str()));
				for (auto t : bank.getTasks()) {
					if (t.getIdUser() == u.getId()) {
						printUserInfo(t);
					}
				}
			}
			catch (string e) {
				cout << e << endl;
			}
		}
		else if (c == "users") {
			for (auto u : bank.getUsers()) {
				cout << "[" << u.getId() << "] "
					<< ", �����: " << u.getLogin()
					<< ", ���: " << u.getName()
					<< ", ������ ����: " << u.getIdBooks().size() << endl;
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

int statusWork() {
	int i;
	do {
		cout << "������ � �������(1) | ������ � ��������������(2): ";
		if (!(cin >> i)) {
			cin.clear();
			while (cin.get() != '\n') continue;
		}
	} while (i != 1 && i != 2);
	cin.get();
	return i;
}

void printHelpBooks() {
	cout << "\thelp [������� �� ��������]" << endl;
	cout << "\tbooks [������ ���� ����]" << endl;
	cout << "\tbooks -f [������ ���� ���� � ������ ��������]" << endl;
	cout << "\tbooks -a <author> [������ ���� �� ������]" << endl;
	cout << "\tbooks -g <genre> [������ ���� �� �����]" << endl;
	cout << "\tbook <id book> [������� �� �����]" << endl;
	cout << "\tadd <id book> <count> [�������� �����]" << endl;
	cout << "\tnew [�������� ����� �����]" << endl;
	cout << "\tdelete <id book> <count> [������� ���. ����]" << endl;
	cout << "\texit [��������� ������]" << endl;
}

void printHelpUsers() {
	cout << "\thelp [������� �� ��������]" << endl;
	cout << "\tusers [������ �������������]" << endl;
	cout << "\ttasks [������ ���� ������]" << endl;
	cout << "\ttasks -p [������ ������ �� ��������� ����]" << endl;
	cout << "\tallowed <id task> [������ �����]" << endl;
	cout << "\trejected <id task> [�������� � ������ �����]" << endl;
	cout << "\tinfo <id user> [���������� ������� �� ������������]" << endl;
	cout << "\texit [��������� ������]" << endl;
}

string getCommand(string* cmd) {
	string c = "";
	int i;
	for (i = 0; i < (*cmd).length() && (*cmd)[i] != ' '; i++) {
		c += (*cmd)[i];
	}
	*cmd = (*cmd).erase(0, ++i);
	return c;
}

void printUserInfo(Task t) {
	cout << endl;
	cout << "������������ ����������� �����: [" << t.getIdBook() << "]" << endl;
	if (t.getStatus() == ALLOWED) {
		cout << "������������ ������ �����: [" << t.getIdBook() << "]" << endl;
	}
	if (t.getStatus() == REJECTED) {
		cout << "������������ �������� � ������ �����: [" << t.getIdBook() << "]" << endl;
	}
	if (t.getStatus() == RETURNED) {
		cout << "������������ ������ �����: [" << t.getIdBook() << "]" << endl;
		cout << "������������ ������ �����: [" << t.getIdBook() << "]" << endl;
	}
}

void printBookInfo(Task t) {
	cout << endl;
	cout << "������������ [" << t.getIdUser() << "] ����������� �����" << endl;
	if (t.getStatus() == ALLOWED) {
		cout << "������������ [" << t.getIdUser() << "] ������ �����" << endl;
	}
	if (t.getStatus() == REJECTED) {
		cout << "������������ [" << t.getIdUser() << "] �������� � �������" << endl;
	}
	if (t.getStatus() == RETURNED) {
		cout << "������������ [" << t.getIdUser() << "] ������ �����" << endl;
		cout << "������������ [" << t.getIdUser() << "] ������ �����" << endl;
	}
}

void printBook(Book b, WorkBooks bank) {
	int n = 0, rn = 0;
	for (auto t : bank.getTasks()) {
		if (t.getIdBook() == b.getId()) {
			if (t.getStatus() == ALLOWED) rn++;
			if (t.getStatus() == RETURNED) n++;
		}
	}
	cout << "[" << b.getId() << "]"
		<< " \"" << b.getName() << "\""
		<< ", �����: " << b.getAuthor()
		<< ", ����������: " << b.getCount()
		<< ", ����: " << b.getGenre()
		<< ", ���: " << b.getYear()
		<< ", ������ ������: " << rn
		<< ", ��� ���������: " << n << endl;
}