/*******************************************************************************
 *
 *	thread.cpp
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
#include <windows.h>
#include "thread.h"

/*******************************************************************************
 *
 *
 *
 *
 *
 ******************************************************************************/

thread::thread()
{
	execute = 0;

//	makeThread();
}

/*******************************************************************************
 *
 *
 *
 *
 *
 ******************************************************************************/

thread::thread(exe* x)
{
	execute = x;

	makeThread();
}



/*******************************************************************************
 *
 *
 *
 *
 *
 ******************************************************************************/

void thread::makeThread(void) 
{ 
    char szMsg[80];

#ifdef _WIN32
	dwThrdParam = 1;
    hThread = CreateThread( 
        NULL,                        // default security attributes 
        2000000,                           // use default stack size  
        ThreadFunc,                  // thread function 
        reinterpret_cast<LPVOID>(execute),   // argument to thread function 
        0,                           // use default creation flags 
        &thread_id);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (hThread == NULL) 
   {
	   throw ccd_exception("thread::makeThread CreateThrea() Failed");
   }
   else 
   {
     // _getch();
      //CloseHandle( hThread );
   }

#else
	rc = pthread_create(
		&thread_id, 
		NULL, 
		ThreadFunc, 
		(void *)execute);
	if (rc)
	{
	   throw ccd_exception("thread::makeThread pthread_create() Failed");
	}
#endif

}


/*******************************************************************************
 *
 *
 *
 *
 *
 ******************************************************************************/
#ifdef _WIN32
DWORD WINAPI thread::ThreadFunc( LPVOID lpParam ) 
{ 
	exe* run;

	run = reinterpret_cast<exe*>(lpParam);

	if (run != 0)
	{
		run->main(0);
	}


	return(0); 
} 


#else
void thread::ThreadFunc( void* lpParam ) 
{ 
	exe* run;

	run = reinterpret_cast<exe*>(lpParam);

	if (run != 0)
	{
		run->main(0);
	}


	return(0); 
} 


#endif


void thread::suspend(void)
{
#ifdef _WIN32
	SuspendThread(hThread);
#endif
}

void thread::resume(void)
{
#ifdef _WIN32
	ResumeThread(hThread);
#endif
}


void thread::setExe(exe *x)
{
	this->execute=x;

}