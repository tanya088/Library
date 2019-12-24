#pragma once
#include "Bank.h"

class BankForUser : public Bank
{
public:
	User registerUser(string login, string password, string name, int birth, string phone) {
		for (auto u : users) {
			if (login == u.getLogin()) {
				throw (string)"Пользователь с таким логином уже существует!";
			}
		}
		int newId = newUserId();
		User user = User(newId, login, password, name, birth, phone, "Неизвестно!", 0, list<int>(), 3);
		users.push_back(user);
		writeUser(newId, login, password, name, birth, phone, user.getReaderDepartment(), user.getReaderCourse(), user.getIdBooks(), user.getCountBooks());
		return getUser(newId);
	}

	User getUser(int id) {
		updateUsers();
		for (auto u : users) {
			if (u.getId() == id) {
				return u;
			}
		}
		throw (string)"Не нашелся пользователь с таким id!";
	}

	User signIn(string login, string password) {
		for (auto u : users) {
			if (login == u.getLogin() && password == u.getPassword()) {
				return u;
			}
		}
		throw (string)"Неверный логин или пароль!";
	}

	list<Book> getBooks() {
		updateBooks();
		return books;
	}

	void request(int idBook, int idUser) {
		updateTasks();
		updateBooks();
		Book b = getBook(idBook);
		for (auto t : tasks) {
			if (t.getIdBook() == idBook && t.getIdUser() == idUser && t.getStatus() == PROCESSED) {
				throw (string)"Ваш запрос еще в обработке!";
			}
		}
		int newId = newTaskId();
		tasks.push_back(Task(newId, idBook, idUser, PROCESSED));
		writeTask(newId, idBook, idUser, PROCESSED);
		cout << "Запрос отправлен!" << endl;
	}

	void returnBook(int idBook, int idUser) {
		updateBooks();
		updateTasks();
		User u = getUser(idUser);
		Book b = getBook(idBook);
		for (int i : u.getIdBooks()) {
			if (i == idBook) {
				u.returnBook(idBook);
				b.returnBook();
				for (auto t : tasks) {
					if (t.getIdBook() == idBook && t.getIdUser() == idUser && t.getStatus() == ALLOWED) {
						t.setStatus(RETURNED);
						writeTasks(t);
						writeBooks(b);
						writeUsers(u);
						cout << "Вы вернули книгу!" << endl;
						return;
					}
				}
				throw (string)"Запрос не найден!";
			}
		}
		throw (string)"У вас нет данной книги!";
	}

private:
	int newUserId() {
		int firstid = 1000;
		if (users.empty()) {
			return firstid;
		}
		for (int i = firstid; i < 9999; i++) {
			bool result = true;
			for (auto u : users) {
				if (u.getId() == i) {
					result = false;
					break;
				}
			}
			if (result) return i;
		}
		throw (string)"Невозможно добавить нового пользователя!";
	}

	int newTaskId() {
		int firstid = 6000;
		if (tasks.empty()) {
			return firstid; 
		}
		for (int i = firstid; i < 9999; i++) {
			bool result = true;
			for (auto t : tasks) {
				if (t.getId() == i) {
					result = false;
					break;
				}
			}
			if (result) return i;
		}
		throw (string)"Невозможно добавить новый запрос!";
	}
};