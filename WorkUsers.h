#pragma once
#include "Bank.h"
#include "Status.h"

using namespace std;

class WorkUsers : public Bank {
private:

public:
	Task getTask(int id) {
		updateTasks();
		for (auto t : tasks) {
			if (t.getId() == id) {
				return t;
			}
		}
		throw (string)"Заявки с таким id нет!";
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

	list<User> getUsers() {
		updateUsers();
		return users;
	}

	list<Task> getTasksProcessed() {
		updateTasks();
		list<Task> _tasks;
		for (auto t = tasks.begin(); t != tasks.end(); t++) {
			if (t->getStatus() == PROCESSED) {
				_tasks.push_back(*t);
			}
		}
		return _tasks;
	}

	list<Task> getTasks() {
		updateTasks();
		return tasks;
	}
};