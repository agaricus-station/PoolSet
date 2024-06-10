#ifndef POOL_SET_THREAD_POOL_THREAD_POOL_H_
#define POOL_SET_THREAD_POOL_THREAD_POOL_H_

#include <task.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <vector>
#include <list>
#include <atomic>

namespace PoolSet {

class ThreadPool {
 public:
	ThreadPool();
	explicit ThreadPool(const std::vector<std::thread>::size_type& size);
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void Init(int num);

	void Start();

	void Stop();

	void AddTask(std::shared_ptr<Task> task);
	std::shared_ptr<Task> GetTask();

	bool is_exit() const;
	void set_is_exit(bool is_exit);

	size_t task_run_count();

 private:
	void Run();
	std::vector<std::thread>::size_type thread_num_;
	std::mutex thread_num_mtx_;
	std::mutex threads_mtx_;
	std::mutex task_mtx_;

	std::vector<std::shared_ptr<std::thread>> threads_;
	std::list<std::shared_ptr<Task>> tasks_;

	std::condition_variable cv_;
	bool is_exit_;
	std::atomic<size_t> task_run_count_;

};

}; // namespace PoolSet


#endif // POOL_SET_THREAD_POOL_THREAD_POOL_H_
