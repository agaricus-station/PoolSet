#include <thread_pool.h>
#include <chrono>
#include <stdexcept>
#include <iostream>

namespace PoolSet {

ThreadPool::ThreadPool(const std::vector<std::thread>::size_type& size) {
	std::unique_lock<std::mutex> lock(thread_num_mtx_);
	this->thread_num_ = size;
	this->threads_.clear();
	task_run_count_ = 0;
}

ThreadPool::~ThreadPool() {
	Stop();
}

void ThreadPool::Init(int num) {
	std::unique_lock<std::mutex> lock(thread_num_mtx_);
	this->thread_num_ = num;
}

void ThreadPool::Start() {
	std::unique_lock<std::mutex> lock(threads_mtx_);
	if (this->thread_num_ <= 0) {
		std::cerr << "Please Init ThreadPool first!" << std::endl;
		return ;
	}

	if (!threads_.empty()) {
		std::cerr << "Thread pool has started!" << std::endl;
		return ;
	}

	for (auto index = 0; index < thread_num_; ++index) {
		std::shared_ptr<std::thread> th =
				std::make_shared<std::thread>(&ThreadPool::Run, this);
		threads_.push_back(th);
	}
}

void ThreadPool::Stop() {
	is_exit_ = true;
	cv_.notify_all();
	for (auto& th : threads_) {
		th->join();
	}
	std::unique_lock<std::mutex> lock(threads_mtx_);
	threads_.clear();
}

void ThreadPool::AddTask(std::shared_ptr<Task> task) {
	std::unique_lock<std::mutex> lock(task_mtx_);
	std::cout << "add new task, task_name=" << task->task_name() << std::endl;
	tasks_.push_back(task);
	task->set_is_exit([this]()->bool{ return is_exit_; });
	lock.unlock();
	cv_.notify_one();
}

std::shared_ptr<Task> ThreadPool::GetTask() {
	std::unique_lock<std::mutex> lock(task_mtx_);
	if (tasks_.empty()) {
		cv_.wait(lock);
	}
	if (is_exit_ || tasks_.empty()) {
		return nullptr;
	}

	std::shared_ptr<Task> task = tasks_.front();
	tasks_.pop_front();
	return task;
}

bool ThreadPool::is_exit() const {
	return is_exit_;
}

void ThreadPool::set_is_exit(bool is_exit) {
	is_exit_ = is_exit;
}

size_t ThreadPool::task_run_count() {
	return task_run_count_;
}

void ThreadPool::Run() {
	while (!is_exit()) {
		std::shared_ptr<Task> task = GetTask();
		if (!task) {
			continue;
		}
		++task_run_count_;
		try {
			task->Run();
		} catch (...) {
			std::cerr << "somethings run error!" << std::endl;
		}
		--task_run_count_;
	}
}




}; // namespace PoolSet
