/*******************************************************************************
 *
 *	mutex.h
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
#include <Windows.h>
#else
#include <pthread.h>
#endif


/*
 * Double incluson protection.
 */
#ifndef _MUTEX_H
	#define _MUTEX_H




/*******************************************************************************
 *
 *	Class mutex
 *
 *
 ******************************************************************************/

class mutex
{
	public:
		// make mutex object.
		mutex();

		// destroy mutex object.
		~mutex();

		// Function to get control of mutex. Blocks!
		void grabMutex(void);

		// Function to try mutex, see if its available.
		bool tryMutex(void);

		// Function to release mutex.
		void releaseMutex(void) ;


	protected:

		// Critical section object.
#ifdef _WIN32
		CRITICAL_SECTION critical_section; 
#else

		   pthread_mutex_t my_mutex;
#endif

};


#endif
