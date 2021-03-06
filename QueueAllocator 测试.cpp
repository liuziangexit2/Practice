#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> 
#include <conio.h>

#include <thread>
#include <condition_variable>

#include <iostream>
#include <Windows.h>
#include "可能这就是手动管理内存吧.h"
#include "DC_timer.h"

void dosth()noexcept {}

struct test {
	int number;
	void* mem;
	DC::timer timer;

	test() :number(9710), mem(nullptr) {
		mem = malloc(1024);
		timer.start();
		if (mem == NULL)
			throw DC::Exception("malloc failed", "");
	}

	~test() {
		number = -9710;
		if (mem != NULL&&mem != nullptr)
			free(mem);
		mem = nullptr;
	}
};

std::condition_variable cv;

void producter(QueueAllocator<test>& pool, std::atomic<bool>& should_return) {
	while (true) {
		if (should_return.load(std::memory_order_acquire))
			break;
		if (pool.make() == nullptr) {
			should_return.store(true, std::memory_order_release);
			return;
		}
	}
}


void cleaner(QueueAllocator<test>& pool) {
	while (true) {
		if (pool.remove_if([](const test& item) {
			if (item.timer.getms() > 1000) return true;
			return false;
		}) == false) {
			if (pool.empty())
				return;
		}
	}
}

int main() {
	{
		try {
			QueueAllocator<test> f_alloc(0);
			std::atomic<bool> *should_return(new std::atomic<bool>(false));

			std::thread *producter_thread;
			std::thread *cleaner_thread;

			producter_thread = new std::thread[5];
			cleaner_thread = new std::thread;
			should_return->store(false, std::memory_order_release);

			for (int i = 0; i < 5; i++)
				producter_thread[i] = std::thread(producter, std::ref(f_alloc), std::ref(*should_return));

			for (int i = 0; i < 5; i++)
				producter_thread[i].detach();

			Sleep(500);
			*cleaner_thread = std::thread(cleaner, std::ref(f_alloc));

			Sleep(3000);
			should_return->store(true, std::memory_order_release);

			for (int i = 0; i < 5; i++)
				if (producter_thread[i].joinable())
					producter_thread[i].join();
			if (cleaner_thread->joinable())
				cleaner_thread->join();

			delete[] producter_thread;
			delete cleaner_thread;

			delete should_return;

			std::cout << "QueueAllocator::empty(): " << f_alloc.empty();
			_getch();
		}
		catch (std::exception& ex) {
			std::cout << ex.what();
			_getch();
		}
	}
	std::cout << "正在输出内存泄露信息...";
	_CrtDumpMemoryLeaks();
}