#pragma once
#include <iostream>
#include "Bank.h"

using namespace std;

class WorkBooks : public Bank {
public:
	list<Book> getBooks() {
		updateBooks();
		return books;
	}

	list<Task> getTasks() {
		updateTasks();
		return tasks;
	}

	void deleteBook(int idBook, int n) {
		Book b = getBook(idBook);
		if (n == 0) throw (string)"Вы не указали сколько книг списать!";
		if (n > b.getCount()) throw (string)"Вы хотите списать книг больше, чем их есть в наличии!";
		b.giveBook(n);
		writeBooks(b);
		cout << "Готово" << endl;
	}

	void addBook(int idBook, int n) {
		Book b = getBook(idBook);
		if (n == 0) throw (string)"Вы не указали сколько книг добавить!";
		if (n < 1) throw (string)"Число меньше 1!";
		b.addBook(n);
		writeBooks(b);
		cout << "Готово" << endl;
	}

	void newBook(string name, string author, string publishing, string genre, int year, int count) {
		if (count < 0) throw (string)"Проблемы с количеством книг!";
		int newId = newBookId();
		Book b = Book(newId, name, author, year, publishing, genre, count);
		books.push_back(b);
		writeBook(b);
	}

private:
	int newBookId() {
		int firstid = 3000;
		if (books.empty()) {
			return firstid;
		}
		for (int i = firstid; i < 9999; i++) {
			bool result = true;
			for (auto b : books) {
				if (b.getId() == i) {
					result = false;
					break;
				}
			}
			if (result) return i;
		}
		throw (string)"Невозможно добавить новую книгу!";
	}
};