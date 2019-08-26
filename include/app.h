// [_jcode] Thread Example Source
// Ubuntu 16.04 LTS, GCC/G++ 8.1
// Operation Information & Communication Wing, System Dev Department
// Air-defense Guided Missile Team, KTMO-Cell Software Support, SukJoon Oh(17-70010114)
// Last update: 2019.03.10.


// #pragma once

// Lists version
#define _ALPHA_ "Alpha"
#define _BETA_ "Beta"

#define _WEEKLY_ "Weekly"
#define _MONTHLY_ "Monthly"

#define _MILESTONE_ "Milestone"
#define _SNAPSHOT_ "Snapshot"
#define _RC_ "Release candidate"
#define _STABLE_ "Stable"


#include <cstdio>
#include <iostream>

//
#include <thread>
#include <chrono>

namespace _jcode {
	
	void appBannerConsole() {
		
		std::cout << " [link_study] Thread Library Example Code" << std::endl;
		std::cout << " Latest update: '19.03.17." << std::endl;
		std::cout << "\x1b[32m > Ubuntu 16.04 LTS, GCC/G++ 8.1" << std::endl;
		std::cout << " > Operation Information & Communication Wing, System Dev Department" << std::endl;
		std::cout << " > Air-defense Guided Missile Team, KTMO-Cell Software Support, SukJoon Oh(17-70010114)\x1b[0m" << std::endl;
		
	};
	
	void appThreadStartConsole() {
		
		std::cout << "\x1b[33m   [thread_id:\x1b[0m" << std::this_thread::get_id() << "\x1b[33m] Thread start.\x1b[0m" << std::endl;
		
	};
	
	void appThreadTerminateConsole() {
		
		std::cout << "\x1b[33m   [thread_id:\x1b[0m" << std::this_thread::get_id() << "\x1b[33m] Thread terminated.\x1b[0m" << std::endl;
		
	};
	
	void appThreadElapsed(std::chrono::duration<double>& arg_time) {
		
		std::cout << "\x1b[33m   [thread_id:\x1b[0m" << std::this_thread::get_id() << "\x1b[33m] " << arg_time.count() << "s elapsed.\x1b[0m" << std::endl;
	};
};


#define APP_BANNER	_jcode::appBannerConsole();
#define APP_THREAD_START	_jcode::appThreadStartConsole();
#define APP_THREAD_TERMINATE	_jcode::appThreadTerminateConsole();
