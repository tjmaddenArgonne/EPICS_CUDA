/*******************************************************************************
 *
 *	mutex.cpp
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
#ifdef _WIN32
#include <Windows.h>
#endif


#include "ccd_exception.h"
#include "mutex.h"


/*******************************************************************************
 *
 *
 *
 *
 *
 ******************************************************************************/

mutex::mutex()
{
#ifdef _WIN32
	InitializeCriticalSection(&critical_section);
	#else
	pthread_mutex_init(&my_mutex, NULL);
#endif

}


/*******************************************************************************
 *
 *
 *	destroy mutex
 *
 *
 ******************************************************************************/

mutex::~mutex()
{
	#ifdef _WIN32

	DeleteCriticalSection(&critical_section);
#else
pthread_mutex_destroy(&my_mutex);
#endif

}


/*******************************************************************************
 *
 *
 * Function to get control of mutex. Blocks!
 *
 *
 ******************************************************************************/
	
void mutex::grabMutex(void)
{
	#ifdef _WIN32

	EnterCriticalSection(&critical_section); 
	#else
	pthread_mutex_lock (&my_mutex);
#endif

}


/*******************************************************************************
 *
 *	Function to try mutex, see if its available.
 *
 *
 *
 ******************************************************************************/
	
bool mutex::tryMutex(void)
{
	bool is_grab;
	
//	is_grab = TryEnterCriticalSection(&critical_section);
//	is_grab = false;//!! cant compile tryenter.....
	throw ccd_exception("mutex::tryMutex- could not compile it- oh well...");
	
	return(is_grab);
}


/*******************************************************************************
 *
 * Function to release mutex.
 *
 *
 *
 ******************************************************************************/

void mutex::releaseMutex(void)
{
	#ifdef _WIN32

	LeaveCriticalSection(&critical_section);
#else
	pthread_mutex_unlock (&my_mutex);
#endif
}
