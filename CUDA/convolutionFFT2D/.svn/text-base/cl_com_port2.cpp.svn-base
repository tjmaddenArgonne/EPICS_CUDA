
#include "cl_com_port2.h"



cl_com_port::cl_com_port(char *name)
{
	strcpy(port_name,name);
	 is_open = false;


}
cl_com_port::~cl_com_port()
{
  if (is_open)
	  close();
}

void cl_com_port::open(
				int baud, 
				int parity, 
				int nbits, 
				int nstop)
{
			char errmess[128];
		DWORD errcode;

	if (is_open)
	{
		throw ccd_exception("cl_com_port::open Already Open");
	}

	
	if ((comport = 
         CreateFile(port_name,                // open com5:
                    GENERIC_READ | GENERIC_WRITE, // for reading and writing
                    0,                            // exclusive access
                    NULL,                         // no security attributes
                    OPEN_EXISTING,              
                    FILE_ATTRIBUTE_NORMAL,//was 0
                    NULL)) == INVALID_HANDLE_VALUE)

    {

		errcode=GetLastError();
		sprintf(errmess,"cl_com_port::open CreateFile ERROR code  %i",(int)errcode);
		throw ccd_exception(errmess);
    }

	 is_open = true;


	    // Set Port parameters.
    // Make a call to GetCommState() first in order to fill
    // the comSettings structure with all the necessary values.
    // Then change the ones you want and call SetCommState().
	SecureZeroMemory(&comSettings, sizeof(DCB));
	comSettings.DCBlength = sizeof(DCB);
    bStatus = GetCommState(comport, &comSettings);
    if (bStatus == 0)
    {
		errcode=GetLastError();
		sprintf(errmess,"cl_com_port::open secureZeroMemory ERROR code  %i",(int)errcode);
		throw ccd_exception(errmess);
    }


    comSettings.BaudRate = baud;
    comSettings.StopBits = nstop;
    comSettings.ByteSize = nbits;
    comSettings.Parity   = parity;
   // comSettings.fParity  = FALSE;
    bStatus = SetCommState(comport, &comSettings);

    if (bStatus == 0)
    {
		errcode=GetLastError();
		sprintf(errmess,"cl_com_port::open setcommstate ERROR code  %i",(int)errcode);
		throw ccd_exception(errmess);

    }
	    // Set timeouts in milliseconds

    CommTimeouts.ReadIntervalTimeout         = 150; 
    CommTimeouts.ReadTotalTimeoutMultiplier  = 10; 
    CommTimeouts.ReadTotalTimeoutConstant    = 2500;
    CommTimeouts.WriteTotalTimeoutMultiplier = 10;
    CommTimeouts.WriteTotalTimeoutConstant   = 2500;
    bStatus = SetCommTimeouts(comport,&CommTimeouts);

    if (bStatus == 0)
    {
		errcode=GetLastError();
		sprintf(errmess,"cl_com_port::open setCommtimeouts ERROR code  %i",(int)errcode);
		throw ccd_exception(errmess);
    }


}

void cl_com_port::open(void)
{


	if (is_open)
	{
		throw ccd_exception("cl_com_port::open Already Open");
	}

	
	if ((comport = 
         CreateFile(port_name,                // open com5:
                    GENERIC_READ | GENERIC_WRITE, // for reading and writing
                    0,                            // exclusive access
                    NULL,                         // no security attributes
                    OPEN_EXISTING,              
                    FILE_ATTRIBUTE_NORMAL,//was 0
                    NULL)) == INVALID_HANDLE_VALUE)

    {

		throw ccd_exception("cl_com_port::open cannot open for some reason");
    }

	 is_open = true;


	    // Set Port parameters.
    // Make a call to GetCommState() first in order to fill
    // the comSettings structure with all the necessary values.
    // Then change the ones you want and call SetCommState().
	SecureZeroMemory(&comSettings, sizeof(DCB));
	comSettings.DCBlength = sizeof(DCB);
    bStatus = GetCommState(comport, &comSettings);
    if (bStatus == 0)
    {
        throw ccd_exception("cl_com_port::open GetCommState failed.");
    }


    comSettings.BaudRate = CBR_115200;
    comSettings.StopBits = ONESTOPBIT;
    comSettings.ByteSize = 8;
    comSettings.Parity   = NOPARITY;
   // comSettings.fParity  = FALSE;
    bStatus = SetCommState(comport, &comSettings);

    if (bStatus == 0)
    {
        throw ccd_exception("cl_com_port::open cannot set baud rate etc.");
    }
	    // Set timeouts in milliseconds

    CommTimeouts.ReadIntervalTimeout         = 15; 
    CommTimeouts.ReadTotalTimeoutMultiplier  = 1; 
    CommTimeouts.ReadTotalTimeoutConstant    = 250;
    CommTimeouts.WriteTotalTimeoutMultiplier = 1;
    CommTimeouts.WriteTotalTimeoutConstant   = 250;
    bStatus = SetCommTimeouts(comport,&CommTimeouts);

    if (bStatus == 0)
    {
   		throw ccd_exception("cl_com_port::open cannot set timeouts");
    }

}

void cl_com_port::write(unsigned char c)
{
	this->write(&c,1);
	flush();

}


void cl_com_port::write(unsigned char *buffer, int length)
{
	char errmess[128];
	DWORD errcode;
	//recordfile.puts(buffer,length);
	if (is_open)
	{
		bStatus = WriteFile(comport,              // Handle
			buffer,      // Outgoing data
			length,              // Number of bytes to write
			&bytes_written,  // Number of bytes written
			NULL);

				if (bStatus == 0)
				{
					errcode=GetLastError();
					sprintf(errmess,"cl_com_port::write cannot send data:  %i",(int)errcode);
					throw ccd_exception(errmess);
				}

				if (bytes_written < length)
				{
					throw ccd_exception("cl_com_port::write Did not send all bytes.");
				}

	}
	else
	{
		throw ccd_exception("cl_com_port::write Port is closed.");
	}

}


unsigned char cl_com_port::read(void)
{
	unsigned char buffer[10];
	read(buffer,1);
	flush();
	return(buffer[0]);

}
void cl_com_port::read(unsigned char *buffer, int length)
{
	char errmess[128];
	DWORD errcode;
	char xx='R';
		//recordfile.puts(&xx,1);
	if (is_open)
	{
			bStatus = ReadFile(comport,   // Handle
					buffer,            // Incoming data
					length,                  // Number of bytes to read
					&bytes_read,          // Number of bytes read
					NULL);

			if (bStatus == 0)
			{
					errcode=GetLastError();
					sprintf(errmess,"cl_com_port::read cannot read data:  %i",(int)errcode);
					throw ccd_exception(errmess);
			}
			// we assume EOF here...
			if (length==1 && bytes_read == 0)
			{

				buffer[0] = EOF;
			}
	}
	else
	{
			throw ccd_exception("cl_com_port::read Port is closed");
	}
}
void cl_com_port::flush(void)
{
if (is_open)
	FlushFileBuffers(comport);
else
	throw ccd_exception("cl_com_port::flush Port is closed");

}

void cl_com_port::close()
{
  if (is_open)
  {
	CloseHandle(comport);
	is_open = false;
  }
  else
	  throw ccd_exception("cl_com_port::close Port is not open");
}
/*
*
*/





void cl_com_port::clearPipe(void)
{
	
	char ret;
	int counter=0;
	ret = this->read();
	while (ret != EOF)
	{
		ret = this->read();
		counter++;
		if (counter>10)
			break;
	}


}


/**************************************************************************
 *
 * wait x us
 *
 **************************************************************************/
void cl_com_port::wait(int us)
{
	int t0,t1;

	t0 = clock();

	t1 = us*CLOCKS_PER_SEC/1000000;

	while ( (clock()-t0) < t1)
	{
	}


}



/**************************************************************************
 *
 * stat stop watch
 *
 **************************************************************************/
void cl_com_port::tic()
{

	 currenttime = (double)clock()/ (double)CLOCKS_PER_SEC;

}



/**************************************************************************
 *
 * read stop watch
 *
 **************************************************************************/
double cl_com_port::toc()
{
	     	  elapsedtime =((double)clock() / (double)CLOCKS_PER_SEC) - currenttime;
			  return(elapsedtime);


}


