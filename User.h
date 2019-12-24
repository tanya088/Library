#pragma once
#include <string>
#include <list>

using namespace std;

class User {
private:
	int id;
	string login;
	string password;
	string name;
	int birth;
	string phone;
	string readerDepartment;
	int readerCourse;
	list<int> idBooks;
	int countBooks;

public:
	User(int id,
		string login,
		string password,
		string name,
		int birth,
		string phone,
		string readerDepartment,
		int readerCourse,
		list<int> idBooks,
		int countBooks)
	{
		this->id = id;
		this->login = login;
		this->password = password;
		this->name = name;
		this->birth = birth;
		this->phone = phone;
		this->readerDepartment = readerDepartment;
		this->readerCourse = readerCourse;
		this->idBooks = idBooks;
		this->countBooks = countBooks;
	}

	int getId() {
		return id;
	}

	string getLogin() {
		return login;
	}

	string getPassword() {
		return password;
	}

	string getName() {
		return name;
	}

	int getBirth() {
		return birth;
	}

	string getPhone() {
		return phone;
	}

	string getReaderDepartment() {
		return readerDepartment;
	}

	int getReaderCourse() {
		return readerCourse;
	}

	list<int> getIdBooks() {
		return idBooks;
	}

	int getCountBooks() {
		return countBooks;
	}

	void returnBook(int idBook) {
		for (auto i : idBooks) {
			if (i == idBook) {
				idBooks.remove(idBook);
				return;
			}
		}
		throw (string)"Такой киниги у пользователя нет!";
	}

	void addBook(int idBook) {
		if (idBooks.size() >= countBooks) throw (string)"В данный момент пользователю нельзя выдать еще книг!";
		else {
			idBooks.push_back(idBook);
		}
	}
};

