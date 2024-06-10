#include <task.h>

namespace PoolSet {

std::string Task::task_name() const {
	return task_name_;
}

void Task::set_task_name(const std::string& task_name) {
	this->task_name_ = task_name;
}

std::future<int> Task::value() {
	return value_.get_future();
}

void Task::set_value(int value) {
	value_.set_value(value);
}

bool Task::is_exit() const {
	return is_exit_func_();
}

void Task::set_is_exit(std::function<bool()> func) {
	is_exit_func_ = func;
}


}; // namespace PoolSet
