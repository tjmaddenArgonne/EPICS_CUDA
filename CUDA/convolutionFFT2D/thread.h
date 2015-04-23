/*******************************************************************************
 *
 *	thread.h
 *
 *	Author: Tim Madden
 *	Date:	6/30/03
 *	Project:MBC CCD Detector DAQ.
 *
 *
 *
 *
 ******************************************************************************/


/*
 * Include files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else 
#include <pthread.h>
#endif

#include "ccd_exception.h"
#include "exe.h"

/*
 * Double incluson protection.
 */
#ifndef _THREAD_H
	#define _THREAD_H





/*******************************************************************************
 *
 *	Class THREAD
 *
 *
 *	Make a thread, and start it up.	No member data, so if object destructs, the
 *	thread still runs.
 ******************************************************************************/

class thread
{
	public:
		// make therad object.
		thread();

		// make thread and run exe
		thread(exe* x);

		// Function to make a thread.
		void makeThread(void);

		void setExe(exe* x);

		void suspend(void);
		void resume(void);


	protected:
    
	
#ifdef _WIN32
	DWORD thread_id; 
	DWORD dwThrdParam; 
    HANDLE hThread; 
		// Function that thrad executes.
		static DWORD WINAPI ThreadFunc( LPVOID lpParam ) ;

#else

	int rc;
	pthread_t thread_id;
		static void ThreadFunc( void* lpParam ) ;

#endif
		// pointer to exe
		exe* execute;
};


#endif