// [_jcode] Thread Example Source
// Ubuntu 16.04 LTS, GCC/G++ 8.1
// Operation Information & Communication Wing, System Dev Department
// Air-defense Guided Missile Team, KTMO-Cell Software Support, SukJoon Oh(17-70010114)
// Last update: 2019.03.10.


//#pragma once

#define __JCODE

#define _UNIT_TEST_MODE_ "Unit test mode"
//#define _PRIMARY_MODE_ "Primary mode" 
//#define _SECONDARY_MODE_ "Secondary mode"

// $ echo -e "\x1b[0m", disabling color setting
// 


#define _CONSOLE_DEBUG_

#include "../include/app.h"
#include "../include/JThreadManager.h"

#include "../include/getch.h"

namespace jc = _jcode;




#include <chrono>

// temporary includes
#include <vector>
// #include <random>

#include <exception>

// declaration scope



void thread_function() {
    
	APP_THREAD_START // head of a thread.
	
	std::cout << " > thread_function() running as a thread." << std::endl;
	
	APP_THREAD_TERMINATE // end!!
};


class ThreadFunctor {
public:
    void operator()() {
        APP_THREAD_START // head of a thread.
	
		std::cout << " > ThreadFunctor::operator() running as a thread." << std::endl;
	
		APP_THREAD_TERMINATE // end!!
    };
};




#if defined(_UNIT_TEST_MODE_)

int main() try {
	
	// compilation options
	// g++ -o app ./include/*.h ./src/*.cpp -pthread -std=c++14 -g
	// g++ -o app /workspace/gen_blank/link/include/*.h /workspace/gen_blank/link/src/*.cpp -pthread -std=c++14 -g
	
	namespace jc = _jcode;
	
	APP_BANNER // jc namespace is declared in app.h, for visibility!!
	APP_THREAD_START
	
	auto thread_start = std::chrono::system_clock::now();
	
	
#define _WHAT_ 7
	std::cout << " > Unit test case: " << _WHAT_ << std::endl;
	
	std::cout << " > " << std::thread::hardware_concurrency() << " concurrent threads are supported." << std::endl;

	
#if(_WHAT_ == 1) // part 1 - Create Threads
		
	// What std::thread accepts in constructor?
	// We can attach a callback with the std::thread object, that will be executed when this new thread starts. These callbacks can be,

	// 1.) Function Pointer
	// 2.) Function Objects
	// 3.) Lambda functions
	
	// 1. Creating a thread using Function Pointer
	std::thread worker_thread_1(thread_function);
	
	// 2. Creating a thread using Function Objects
	std::thread worker_thread_2((ThreadFunctor()));
	
	// 3. Creating a thread using Lambda functions
	std::thread worker_thread_3(
		[]() {
			APP_THREAD_START
			std::cout << " > An anonymous lambda running as a thread." << std::endl;
			APP_THREAD_TERMINATE
		}
	);
	
	worker_thread_1.join();
	worker_thread_2.join();
	worker_thread_3.join();
	
	
#elif(_WHAT_ == 2) // part 2 - Joining and Detaching Threads
	
	int divider = 1;
	std::vector<std::thread> worker_threads;
	
	// Once a thread is started then another thread can wait for this new thread to finish. 
	// For this another need need to call join() function on the std::thread object.
		
	for(int i = 0; i < 10; i++) {
		worker_threads.push_back(
			std::thread(
				[=](int arg_div) { // catch by copying.
					APP_THREAD_START
					std::cout << " > An anonymous lambda #" << arg_div << " running as a thread." << std::endl;
					
					APP_THREAD_TERMINATE
				}
			, divider)
		);
		
		divider++;
	}
	
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	std::cout<<" > Waiting for all the worker thread to finish..."<<std::endl;
	for(auto& itor : worker_threads)
		if(itor.joinable())
			itor.join();
	
	
	
	
	
#elif(_WHAT_ == 3)

	
#elif(_WHAT_ == 4) // part 4 - Data Sharing and Race Conditions
	
		
	int counter = 0;
	
	std::thread worker_thread_1([&]() {
		APP_THREAD_START
		std::cout << " > An augmenting lambda running as a thread." << std::endl;
				
		for(int itor = 0; itor < 100; itor++) {
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			counter++;
		}
		
		APP_THREAD_TERMINATE
		
	});
	
	std::thread worker_thread_2([&]() {
		APP_THREAD_START
		std::cout << " > An decreasing lambda running as a thread." << std::endl;
				
		for(int itor = 0; itor < 100; itor++) {
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			counter--;
		}
		
		APP_THREAD_TERMINATE
		
	});
	
	
	if(worker_thread_1.joinable())
		worker_thread_1.join();
	
	if(worker_thread_2.joinable())
		worker_thread_2.join();

	std::cout << " > Racing condition result is " << counter << std::endl;
	
	
#elif(_WHAT_ == 5) // part 5 - Using mutex to fix Race Conditions
		
	int counter = 0;
	std::mutex mutex;
	
	std::thread worker_thread_1([&]() {
		APP_THREAD_START
		std::cout << " > An augmenting lambda running as a thread." << std::endl;
				
		mutex.lock();
		
		for(int itor = 0; itor < 100; itor++) {
			
			
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			counter++;
		}
		
		mutex.unlock();
		
		APP_THREAD_TERMINATE
		
	});
	
	std::thread worker_thread_2([&]() {
		APP_THREAD_START
		std::cout << " > An decreasing lambda running as a thread." << std::endl;
		
		// std::lock_guard is a class template, which implements the RAII for mutex.
		// It wraps the mutex inside it’s object and locks the attached mutex in its constructor. 
		// When it’s destructor is called it releases the mutex.
		
		std::lock_guard<std::mutex> local_lock_guard(mutex);
		
		for(int itor = 0; itor < 100; itor++) {
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			counter--;
		}
		
		APP_THREAD_TERMINATE
		
	});
	
	
	if(worker_thread_1.joinable())
		worker_thread_1.join();
	
	if(worker_thread_2.joinable())
		worker_thread_2.join();

	std::cout << " > Racing condition result is " << counter << std::endl;
	
	
		
	
#elif(_WHAT_ == 6) // part 6 - Need of Event Handling
	
	bool keep_listener_running = true;
	
	// customed example
	std::thread key_event_listener(
		[&]() {
			
			APP_THREAD_START
				
			int key = 0;
			
			while(keep_listener_running) {
				
				std::cout << " > Listening for 'y' key input..." << std::endl;
				
				key = getch();
				
				if(key != 'y')
					continue;
				
				else {
					keep_listener_running = false;
					break;
				}
				
			}
			
			APP_THREAD_TERMINATE
		}
	);
	
	std::thread listener_alert(
		[&]() {
			
			APP_THREAD_START
				
			while(keep_listener_running) {
				
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				std::cout << " > I am listening!!" << std::endl;
				
			}
				
			APP_THREAD_TERMINATE
		}
	);
	
	
	if(key_event_listener.joinable())
		key_event_listener.join();
	
	if(listener_alert.joinable())
		listener_alert.join();
	
	
	
	
	
	
	
#elif(_WHAT_ == 7)	// part 7 - Condition Variables 
	

	std::vector<std::thread> worker_threads; // list of threads

	std::mutex mutex;
	std::condition_variable cv;
	
	
	int wt_signal = 0;
	bool signal_received = false;
		
	std::thread key_event_listener(
		[&]() {
			
			APP_THREAD_START
			
			//while(1) {
				
				std::cout << " > Listening for id of a thread to run..." << std::endl;
				
				std::lock_guard<std::mutex> guard(mutex); // sender
				
				wt_signal = getch() - '0';
			
				signal_received = true;
				cv.notify_all();			
				
			//}
			
			APP_THREAD_TERMINATE
		}
	);
	
	
	
	for(int i = 0; i < 10; i++) {
		worker_threads.push_back(
			std::thread(
				[&](int arg_div) { // catch by copying.
					APP_THREAD_START
					
					const int self_signal_id = arg_div + 1;
						
					//while(1) {
						
						std::unique_lock<std::mutex> lock(mutex);
						// m_condVar.wait(mlock, std::bind(&Application::isDataLoaded, this));
						cv.wait(lock, [&]() { return signal_received; });
						
						if(wt_signal == self_signal_id) {
		
							std::cout << " > An worker thread #" << arg_div + 1 << " catched signal." << std::endl;
						}						
						
						lock.unlock();
					//}					
					
					APP_THREAD_TERMINATE
				}
			, i)
		);
	}
	
	for(auto& itor : worker_threads)
		if(itor.joinable())
			itor.join();
	
	
	if(key_event_listener.joinable())
		key_event_listener.join();	
	
	
	
#elif(_WHAT_ == 8) // part 8 - std::future , std::promise and Returning values from Thread
	// https://thispointer.com//c11-multithreading-part-8-stdfuture-stdpromise-and-returning-values-from-thread/
	
	
	
	
	
		
#else // finally
	
#endif
	
	APP_THREAD_TERMINATE
	
	auto thread_end = std::chrono::system_clock::now();		
		
	std::chrono::duration<double> thread_elapsed = thread_end - thread_start;
	jc::appThreadElapsed(thread_elapsed);
	
	return 0;
	
} catch(std::exception& arg_exception) {
	
	std::cout << " > Exception: " << arg_exception.what() << std::endl;
};




// _SECONDARY_MODE_
#elif defined(_SECONDARY_MODE_)

int main() try {
	
	namespace jc = _jcode;
	
	APP_BANNER // jc namespace is declared in app.h, for visibility!!
	APP_THREAD_START
	
	auto thread_start = std::chrono::system_clock::now();
	
	
	
	
	
	
	APP_THREAD_TERMINATE
	
	auto thread_end = std::chrono::system_clock::now();
	
	std::chrono::duration<double> thread_elapsed = thread_end - thread_start;	
	jc::appThreadElapsed(thread_elapsed);
	
	
	return 0;
	
} catch(std::exception& arg_exception) {
	
	std::cout << " > Exception: " << arg_exception.what() << std::endl;
};





#elif defined(_PRIMARY_MODE_)

int main() {
	
	namespace jc = _jcode;
	
	APP_BANNER // jc namespace is declared in app.h, for visibility!!
	APP_THREAD_START
	
	auto thread_start = std::chrono::system_clock::now();
	
	
	
	
	
	
	APP_THREAD_TERMINATE
	
	auto thread_end = std::chrono::system_clock::now();		
		
	std::chrono::duration<double> thread_elapsed = thread_end - thread_start;
	jc::appThreadElapsed(thread_elapsed);
	
	
	return 0;
}

#endif

