#pragma once
#include <mutex>
#include <queue>

template <typename Data>
class ConcurrentQueue
{
private:
	std::queue<Data>        the_queue{};
	mutable std::mutex      the_mutex{};
	std::condition_variable the_condition_variable{};
public:
	void Push(Data const& data)
	{
		{
			std::scoped_lock lock(the_mutex);
			the_queue.push(data);
		}
		the_condition_variable.notify_one();
	}

	bool Empty() const
	{
		std::scoped_lock lock(the_mutex);
		return the_queue.empty();
	}

	bool TryPop(Data& popped_value)
	{
		std::scoped_lock lock(the_mutex);
		if (the_queue.empty())
		{
			return false;
		}

		popped_value = the_queue.front();
		the_queue.pop();
		return true;
	}

	void WaitAndPop(Data& popped_value)
	{
		std::unique_lock lock(the_mutex);
		while (the_queue.empty())
		{
			the_condition_variable.wait(lock);
		}

		popped_value = the_queue.front();
		the_queue.pop();
	}
};
