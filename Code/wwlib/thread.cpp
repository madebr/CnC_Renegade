/*
**	Command & Conquer Renegade(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "thread.h"
#include "Except.h"
#include "wwdebug.h"

#include <cassert>


ThreadClass::ThreadClass(const char *thread_name) :mRunning(false)
{
	if (thread_name) {
		assert(strlen(thread_name) < sizeof(mThreadName) - 1);
		strcpy(mThreadName, thread_name);
	} else {
		strcpy(mThreadName, "No name");
	}
}

ThreadClass::~ThreadClass()
{
	Stop();
}

void ThreadClass::Internal_Thread_Function(void *params)
{
	ThreadClass *tc = static_cast<ThreadClass *>(params);
	tc->mRunning = true;

	tc->mThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

	Register_Thread_ID(tc->mThreadID, tc->mThreadName);

#ifdef _MSC_VER
	__try {
		tc->Thread_Function();
	} __except(Exception_Handler(GetExceptionCode(), GetExceptionInformation())) {
	}
#else
	tc->Thread_Function();
#endif

	Unregister_Thread_ID(tc->mThreadID, tc->mThreadName);
	tc->mThreadID = 0;
	tc->mRunning = false;
}

void ThreadClass::Execute()
{
	WWASSERT(!mThread.joinable());

	mThread = std::thread{Internal_Thread_Function, this};

	WWASSERT(mThread.joinable());

	mThreadID = std::hash<std::thread::id>{}(mThread.get_id());

	WWDEBUG_SAY(("ThreadClass::Execute: Started thread \"%s\", thread ID is 0x%X\n", mThreadName, mThreadID));
}

void ThreadClass::Set_Priority(int /* priority */)
{
	/* Not supported by c++ threads */
}

void ThreadClass::Stop()
{
	mRunning = false;
	if (mThread.joinable()) {
		mThread.join();
	}
	mThreadID = 0;
}

void ThreadClass::Sleep_Ms(unsigned ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void ThreadClass::Switch_Thread()
{
	std::this_thread::yield();
}

// Return calling thread's unique thread id
unsigned ThreadClass::Get_Current_Thread_ID()
{
	return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

bool ThreadClass::Is_Running()
{
	return mRunning;
}
