#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <stdexcept>
#include <vector>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <future>


class ThreadPool {
	public:
		explicit ThreadPool(size_t numberOfThreads);
		~ThreadPool();

		void enqueueTask(std::function<void()> task);

	private:
		std::vector<std::thread> _threads = {};
		std::condition_variable _condition = {};
		std::mutex _tasksMutex = {};
		std::queue<std::function<void()>> _tasks = {};
		bool _mustStop = false;
};

#endif // !THREADPOOL_HPP