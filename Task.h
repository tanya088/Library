#pragma once
#include "Status.h"

class Task
{
private:
	int id;
	int idBook;
	int idUser;
	Status status;
public:
	Task(int id, int idBook, int idUser, Status status) {
		this->id = id;
		this->idBook = idBook;
		this->idUser = idUser;
		this->status = status;
	}
	int getId() {
		return id;
	}
	int getIdBook() {
		return idBook;
	}
	int getIdUser() {
		return idUser;
	}
	Status getStatus() {
		return status;
	}
	void setStatus(Status status) {
		this->status = status;
	}
};
