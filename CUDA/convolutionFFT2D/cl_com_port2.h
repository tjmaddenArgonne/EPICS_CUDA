/*******************************************************************************
 *
 *	cl_com_port.h
 *
 *	Author: Tim Madden
 *	Date:	8/01/06
 *	Project: CCD Detector DAQ.
 *
 *
 ******************************************************************************/


/*
 * Include files.
 */
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "time.h"

#include "CCD_exception.h"
/*
 * Double incluson protection.
 */
#ifndef _CL_COM_PORT_H
	#define _CL_COM_PORT_H

/*
		 * Class for 
		 */

		class cl_com_port
		{
		public:	
			cl_com_port(char *name);
			~cl_com_port();
			/*
			 * ports we can write to
			 */

		
			

			void open(void);
			void open(
				int baud, 
				int parity, 
				int nbits, 
				int nstop);

			void write(unsigned char *buffer, int length);
			void read(unsigned char *buffer, int length);
			void write(unsigned char c);
			unsigned char read(void);
			void close(void);
			void flush(void);
			void clearPipe();



			void wait(int us);

			// for timing things. call tic. toc returns time in s since last tic.
			void tic();
			double toc();

		protected:

	bool is_open;


	double currenttime,elapsedtime;

    char INBUFFER[500];

    char OUTBUFFER[20];

    DWORD        bytes_read;    // Number of bytes read from port

    DWORD        bytes_written;    // Number of bytes written to the port

    HANDLE       comport;  // Handle COM port

	int   bStatus;

    DCB          comSettings;          // Contains various port settings

    COMMTIMEOUTS CommTimeouts;
  
	char port_name[64];


		};

	

#endif
