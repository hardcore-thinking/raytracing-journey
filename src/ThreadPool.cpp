#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t numberOfThreads = std::thread::hardware_concurrency() | 1) {
	if (numberOfThreads <= 0) {
		throw std::invalid_argument("The number of threads cannot be 0 or negative.");
	}

	_threads.reserve(numberOfThreads);

	for (size_t i = 0; i < numberOfThreads; i++) {
		_threads.emplace_back(
			[this] {
				while (true) {
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(_tasksMutex);

						_condition.wait(lock,
							[this] {
								return !_tasks.empty() || _mustStop;
							}
						);

						if (_mustStop && _tasks.empty()) {
							return;
						}

						task = std::move(_tasks.front());
						_tasks.pop();
					}

					task();
				}
			}
		);
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock lock(_tasksMutex);
		_mustStop = true;
	}

	_condition.notify_all();

	for (auto& thread : _threads) {
		thread.join();
	}
}

void ThreadPool::enqueueTask(std::function<void()> task) {
	{
		std::unique_lock<std::mutex> lock(_tasksMutex);
		_tasks.emplace(std::move(task));
	}

	_condition.notify_one();
}