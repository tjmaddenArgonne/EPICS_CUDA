/*******************************************************************************
 *
 *	exe.h
 *
 *	Author: Tim Madden
 *	Date:	7/01/03
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


/*
 * Double incluson protection.
 */
#ifndef _EXE_H
	#define _EXE_H




/*******************************************************************************
 *
 *	Class exe
 *
 *	Pass to thread object for execution of a specific task. This is an
 *  abstract class. We overload the main function. exe::main is executed.
 ******************************************************************************/

class __declspec(dllimport) exe
{
	public:
		// make exe object.
		exe();

		// run this function.
		virtual int main(int arg);

	protected:

};

#endif
