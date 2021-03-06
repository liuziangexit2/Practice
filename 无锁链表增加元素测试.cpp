#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> 
#include <conio.h>

#include <iostream>
#include <Windows.h>
#include <vector>
#include <mutex>
#include <atomic>
#include <type_traits>

#include <algorithm>

#include "可能这就是手动管理内存吧.h"

void dosth()noexcept {}

int main() {
	{
		try {
			LockFreeLinkedList<std::string> ll;
			std::vector<int> record;
			std::atomic<bool> stopflag(false);
			std::thread th[10];
			std::thread th2 = std::thread([&record, &ll, &stopflag] {
				//std::chrono::milliseconds ss(1000);
				bool reallquit = false;
				while (true) {
					int count = 0;
					ll.traverse([&count](const std::string& str) {
						count++;
					});
					record.emplace_back(count);
					if (reallquit) break;
					if (stopflag.load(std::memory_order::memory_order_acquire)) reallquit = true;
					
					//std::this_thread::sleep_for(ss);
				}
			});
			for (int i = 0; i < 10; i++)
				th[i] = std::thread([&ll] {
				for (int p = 1; p <= 100; p++)
					ll.insert(DC::STR::toString(p));
			});
			
			for (int i = 0; i < 10; i++)
				th[i].join();
			stopflag.store(true, std::memory_order::memory_order_release);
			th2.join();
			int total = 0;
			ll.traverse([&total](const std::string& str) {
				//std::cout << str << "\n";
				total += DC::STR::toType<int>(str);
				return false;
			});
			dosth();
			_getch();
		}
		catch (DC::Exception& ex) {
			abort();
		}
	}
	_CrtDumpMemoryLeaks();
}