#pragma once
#include <string>

using namespace std;

class Book
{
private:
	int id;
	string name;
	string author;
	int year;
	string publishing;
	string genre;
	int count;

public:
	Book(int id, string name, string author, int year, string publishing, string genre, int count) {
		this->id = id;
		this->name = name;
		this->author = author;
		this->year = year;
		this->publishing = publishing;
		this->genre = genre;
		this->count = count;
	}

	int getId() {
		return id;
	}

	string getName() {
		return name;
	}

	string getAuthor() {
		return author;
	}

	int getYear() {
		return year;
	}

	string getPublishing() {
		return publishing;
	}

	string getGenre() {
		return genre;
	}

	int getCount() {
		return count;
	}

	void giveBook() {
		if (count < 1) throw (string)"Сейчас нет книги!";
		count -= 1;
	}

	void giveBook(int n) {
		if (count < n) throw (string)"Сейчас нет столько книг!";
		count -= n;
	}

	void addBook(int n) {
		if (n < 1) throw (string)"Число меньше 1!";
		count += n;
	}

	void returnBook() {
		count += 1;
	}
};