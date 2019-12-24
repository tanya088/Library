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
	cout << "Все команды:" << endl;
	printHelpBooks();
	do {
		SetConsoleTextAttribute(console, FOREGROUND_GREEN);
		cout << "[cmd]~ ";
		SetConsoleTextAttribute(tsvt, bazovyi.wAttributes);
		getline(cin, cmd);
		string c = getCommand(&cmd);

		if (c == "books") {
			if (bank.getBooks().empty()) {
				cout << "В библиотеке нет книг!" << endl;
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
				cout << "Что-то не так!" << endl;
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
				cout << "Что-то не так!" << endl;
			}
		}
		else if (c == "new") {
			string name, auhtor, publishing, genre;
			int year, count;
			while (true) {
				cout << "Название книги: ";
				getline(cin, name);
				cout << "Автор книги: ";
				getline(cin, auhtor);
				cout << "Издательство: ";
				getline(cin, publishing);
				cout << "Год выпуска: ";
				cin >> year;
				cin.get();
				cout << "Жанр книги: ";
				getline(cin, genre);
				cout << "Количество книг: ";
				cin >> count;
				cin.get();
				try {
					bank.newBook(name, auhtor, publishing, genre, year, count);
					cout << "Готово!" << endl;
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
				cout << "Что-то не так!" << endl;
			}
		} else if (c == "exit") {
			break;
		} else {
			cout << "Команда не найдена!" << endl;
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
	cout << "Все команды:" << endl;
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
					if (b.getCount() < 1) throw (string)"Книги закончились!";
					u.addBook(task.getIdBook());
					b.giveBook();
					task.setStatus(ALLOWED);
					bank.writeBooks(b);
					bank.writeTasks(task);
					bank.writeUsers(u);
					cout << "Книга выдана пользователю!" << endl;
				} else {
					throw (string)"Заявка уже обработана!";
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
				cout << "В выдаче отказано!" << endl;
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
					<< ", логин: " << u.getLogin()
					<< ", имя: " << u.getName()
					<< ", взятых книг: " << u.getIdBooks().size() << endl;
			}
		}
		else if (c == "exit") {
			break;
		}
		else {
			cout << "Команда не найдена!" << endl;
		}

	} while (cmd != "exit");
}

int statusWork() {
	int i;
	do {
		cout << "Работа с книгами(1) | Работа с пользователями(2): ";
		if (!(cin >> i)) {
			cin.clear();
			while (cin.get() != '\n') continue;
		}
	} while (i != 1 && i != 2);
	cin.get();
	return i;
}

void printHelpBooks() {
	cout << "\thelp [Справка по командам]" << endl;
	cout << "\tbooks [Список всех книг]" << endl;
	cout << "\tbooks -f [Список всех книг с учетом читаемых]" << endl;
	cout << "\tbooks -a <author> [Список книг по автору]" << endl;
	cout << "\tbooks -g <genre> [Список книг по жанру]" << endl;
	cout << "\tbook <id book> [История по книге]" << endl;
	cout << "\tadd <id book> <count> [Добавить книги]" << endl;
	cout << "\tnew [Добавить новую книгу]" << endl;
	cout << "\tdelete <id book> <count> [Списать кол. книг]" << endl;
	cout << "\texit [Завершить работу]" << endl;
}

void printHelpUsers() {
	cout << "\thelp [Справка по командам]" << endl;
	cout << "\tusers [Список пользователей]" << endl;
	cout << "\ttasks [Список всех заявок]" << endl;
	cout << "\ttasks -p [Список заявок на получение книг]" << endl;
	cout << "\tallowed <id task> [Выдать книгу]" << endl;
	cout << "\trejected <id task> [Отказать в выдаче книги]" << endl;
	cout << "\tinfo <id user> [Посмотреть историю по пользователю]" << endl;
	cout << "\texit [Завершить работу]" << endl;
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
	cout << "Пользователь запрашивает книгу: [" << t.getIdBook() << "]" << endl;
	if (t.getStatus() == ALLOWED) {
		cout << "Пользователю выдана книга: [" << t.getIdBook() << "]" << endl;
	}
	if (t.getStatus() == REJECTED) {
		cout << "Пользователю отказано в выдаче книги: [" << t.getIdBook() << "]" << endl;
	}
	if (t.getStatus() == RETURNED) {
		cout << "Пользователю выдана книга: [" << t.getIdBook() << "]" << endl;
		cout << "Пользователь вернул книгу: [" << t.getIdBook() << "]" << endl;
	}
}

void printBookInfo(Task t) {
	cout << endl;
	cout << "Пользователь [" << t.getIdUser() << "] запрашивает книгу" << endl;
	if (t.getStatus() == ALLOWED) {
		cout << "Пользователю [" << t.getIdUser() << "] выдана книга" << endl;
	}
	if (t.getStatus() == REJECTED) {
		cout << "Пользователю [" << t.getIdUser() << "] отказано в запросе" << endl;
	}
	if (t.getStatus() == RETURNED) {
		cout << "Пользователю [" << t.getIdUser() << "] выдана книга" << endl;
		cout << "Пользователь [" << t.getIdUser() << "] вернул книгу" << endl;
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
		<< ", автор: " << b.getAuthor()
		<< ", количество: " << b.getCount()
		<< ", жанр: " << b.getGenre()
		<< ", год: " << b.getYear()
		<< ", сейчас читают: " << rn
		<< ", уже прочитали: " << n << endl;
}