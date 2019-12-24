#pragma once
#include "Book.h"
#include "User.h"
#include "Task.h"
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

class Bank
{
protected:
	list<Book> books;
	list<User> users;
	list<Task> tasks;
	string PATH_TASKS = "tasks.db";
	string PATH_USERS = "users.db";
	string PATH_BOOKS = "books.db";

public:
	Bank() {
		users = list<User>();
		books = list<Book>();
		tasks = list<Task>();

		updateUsers();
		updateBooks();
		updateTasks();
	}

	void updateTasks() {
		tasks.clear();
		ifstream in(PATH_TASKS);
		if (in.is_open()) {
			while (in) {
				string type, id, idBook, idUser, status;
				getline(in, type);
				if (type == "<task>") {
					getline(in, id);
					getline(in, idBook);
					getline(in, idUser);
					getline(in, status);
					Status s;
					if (status == "PROCESSED") s = PROCESSED;
					if (status == "ALLOWED") s = ALLOWED;
					if (status == "REJECTED") s = REJECTED;
					if (status == "RETURNED") s = RETURNED;
					tasks.push_back(Task(atoi(id.c_str()), atoi(idBook.c_str()), atoi(idUser.c_str()), s));
				}
				else {
					continue;
				}
			}
		}
		in.close();
	}

	void updateBooks() {
		books.clear();
		ifstream in(PATH_BOOKS);
		if (in.is_open()) {
			while (in) {
				string type, id, name, author, year, genre, count, publishing;
				getline(in, type);
				if (type == "<book>") {
					getline(in, id);
					getline(in, name);
					getline(in, author);
					getline(in, year);
					getline(in, publishing);
					getline(in, genre);
					getline(in, count);
					books.push_back(Book(atoi(id.c_str()), name, author, atoi(year.c_str()), publishing, genre, atoi(count.c_str())));
				}
				else {
					break;
				}
			}
		}
		in.close();
	}

	void updateUsers() {
		users.clear();
		ifstream in(PATH_USERS);
		if (in.is_open()) {
			while (in) {
				string type, id, login, password, birth, name, phone, readerDepartment, readerCourse, idBooks, countBooks;
				list<int> list_idBooks;
				getline(in, type);
				if (type == "<user>") {
					getline(in, id);
					getline(in, login);
					getline(in, password);
					getline(in, name);
					getline(in, birth);
					getline(in, phone);
					getline(in, readerDepartment);
					getline(in, readerCourse);
					getline(in, idBooks);
					getline(in, countBooks);

					string number = "";
					for (int i = 0; i < idBooks.length(); i++) {
						if (idBooks[i] != ' ') {
							number += idBooks[i];
						}
						else {
							list_idBooks.push_back(atoi(number.c_str()));
							number = "";
						}
					}

					users.push_back(User(atoi(id.c_str()), login, password, name, atoi(birth.c_str()), phone, readerDepartment, atoi(readerCourse.c_str()), list_idBooks, atoi(countBooks.c_str())));
				}
				else {
					break;
				}
			}
		}
		in.close();
	}
	
	void writeUser(int newId, string login, string password, string name, int birth, string phone, string readDepartment, int readCourse, list<int> idBooks, int countBooks) {
		ofstream out(PATH_USERS, ios_base::app);
		out << "<user>" << endl;
		out << newId << endl;
		out << login << endl;
		out << password << endl;
		out << name << endl;
		out << birth << endl;
		out << phone << endl;
		out << readDepartment << endl;
		out << readCourse << endl;
		for (auto i : idBooks) out << i << " ";
		out << endl;
		out << countBooks << endl;
		out.close();
	}

	void writeUsers(User user) {
		ofstream out(PATH_USERS, ios_base::trunc);
		for (auto u : users) {
			if (u.getId() == user.getId()) u = user;
			out << "<user>" << endl;
			out << u.getId() << endl;
			out << u.getLogin() << endl;
			out << u.getPassword() << endl;
			out << u.getName() << endl;
			out << u.getBirth() << endl;
			out << u.getPhone() << endl;
			out << u.getReaderDepartment()  << endl;
			out << u.getReaderCourse() << endl;
			for (auto i : u.getIdBooks()) out << i << " ";
			out << endl;
			out << u.getCountBooks() << endl;
		}
		out.close();
	}

	void writeUsers() {
		ofstream out(PATH_USERS, ios_base::trunc);
		for (auto u : users) {
			out << "<user>" << endl;
			out << u.getId() << endl;
			out << u.getLogin() << endl;
			out << u.getPassword() << endl;
			out << u.getName() << endl;
			out << u.getBirth() << endl;
			out << u.getPhone() << endl;
			out << u.getReaderDepartment() << endl;
			out << u.getReaderCourse() << endl;
			for (auto i : u.getIdBooks()) out << i << " ";
			out << endl;
			out << u.getCountBooks() << endl;
		}
		out.close();
	}

	void writeBook(Book b) {
		ofstream out(PATH_BOOKS, ios_base::app);
		out << "<book>" << endl;
		out << b.getId() << endl;
		out << b.getName() << endl;
		out << b.getAuthor() << endl;
		out << b.getYear() << endl;
		out << b.getPublishing() << endl;
		out << b.getGenre() << endl;
		out << b.getCount() << endl;
		out.close();
	}

	void writeBooks(Book book) {
		ofstream out(PATH_BOOKS, ios_base::trunc);
		for (auto b : books) {
			if (b.getId() == book.getId()) b = book;
			out << "<book>" << endl;
			out << b.getId() << endl;
			out << b.getName() << endl;
			out << b.getAuthor() << endl;
			out << b.getYear() << endl;
			out << b.getPublishing() << endl;
			out << b.getGenre() << endl;
			out << b.getCount() << endl;
		}
		out.close();
	}

	void writeTask(int newId, int idBook, int idUser, Status status) {
		string s = "PROCESSED";
		if (status == ALLOWED) s = "ALLOWED";
		if (status == REJECTED) s = "REJECTED";
		if (status == RETURNED) s = "RETURNED";
		ofstream out(PATH_TASKS, ios_base::app); 
		out << "<task>" << endl;
		out << newId << endl;
		out << idBook << endl;
		out << idUser << endl;
		out << s << endl;
		out.close();
	}

	void writeTasks(Task task) {
		ofstream out(PATH_TASKS, ios_base::trunc);
		for (auto t : tasks) {
			if (t.getId() == task.getId()) t = task;
			out << "<task>" << endl;
			out << t.getId() << endl;
			out << t.getIdBook() << endl;
			out << t.getIdUser() << endl;
			string s = "PROCESSED";
			if (t.getStatus() == ALLOWED) s = "ALLOWED";
			if (t.getStatus() == REJECTED) s = "REJECTED";
			if (t.getStatus() == RETURNED) s = "RETURNED";
			out << s << endl;
		}
		out.close();
	}

	Book getBook(int id) {
		updateBooks();
		for (auto b : books) {
			if (b.getId() == id) return b;
		}
		throw (string)"Нет книги с таким id!";
	}
};