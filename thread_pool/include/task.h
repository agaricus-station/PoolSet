#ifndef POOL_SET_THREAD_POOL_TASKS_H_
#define POOL_SET_THREAD_POOL_TASKS_H_

#include <functional>
#include <future>
#include <string>

namespace PoolSet {

class Task {
 public:
	virtual int Run() = 0;
	std::string task_name() const;
	void set_task_name(const std::string& task_name);
	std::future<int> value();
	void set_value(int value);
	bool is_exit() const;
	void set_is_exit(std::function<bool()> func);

 private:
	std::string task_name_ = "default_task";
	std::promise<int> value_;
	std::function<bool()> is_exit_func_ = nullptr;
	
};

}; // namespace PoolSet

#endif // POOL_SET_THREAD_POOL_TASKS_H_
