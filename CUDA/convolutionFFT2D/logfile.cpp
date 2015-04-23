/*******************************************************************************
 *
 *	log_file.cpp
 *
 *	Author: Tim Madden
 *	Date:	7/27/03
 *	Project:MBC CCD Detector DAQ.
 *
 *
 *
 *
 ******************************************************************************/


/*
 * Include files.
 */
#include <time.h>
#include "logfile.h"




/*******************************************************************************
 *
 *	Class LOG_FILE
 *
 *
 ******************************************************************************/
log_file::log_file(char* filename)
{
	time_t tp;

	strcpy(log_file_name,filename);

	fp = fopen(filename,"w");
	if (fp!=0)
	{

		time(&tp);
		fprintf(fp,"%s  -CCD Camera Log file opened. \n",ctime(&tp));
		fflush(fp);
		//fclose(fp);
		//fp=0;
	}
	is_enabled = true;
}

/*******************************************************************************
 *
 *	Class LOG_FILE
 *
 *
 ******************************************************************************/
// destroy file object.
log_file::~log_file()
{
	if (fp!=0)
		fclose(fp);
}

/*******************************************************************************
 *
 *	Class LOG_FILE
 *
 *
 ******************************************************************************/
// output to log ile.
void log_file::log(char* message)
{
	time_t tp;
	
	if (is_enabled)
	{
		//fp = fopen(log_file_name,"a");
		if (fp!=0)
		{
			time(&tp);
			fprintf(fp,"%s  -%s \n",ctime(&tp),message);
			fflush(fp);
			//fclose(fp);
			//fp=0;
		}
	}
}

/*******************************************************************************
 *
 *	Class LOG_FILE
 *
 *
 ******************************************************************************/
// output to log ile.
void log_file::logNoDate(char* message)
{
	time_t tp;
	
	if (is_enabled)
	{
		//fp = fopen(log_file_name,"a");
		if (fp!=0)
		{
			time(&tp);
			fprintf(fp,"%s",message);
			fflush(fp);
			//fclose(fp);
			//fp=0;
		}
	}
}


void log_file::puts(char* message,int len)
{
	unsigned int ixx;
	unsigned char cxx;

	if (is_enabled)
	{
		//fp = fopen(log_file_name,"a");
		if (fp!=0)
		{
			for (int i=0; i<len;i++)
			{
				cxx=(unsigned char)message[i];
				ixx=(unsigned short)cxx;
				fprintf(fp,"%x \n",ixx);
			}
			fflush(fp);
			//fclose(fp);
			//fp=0;
		}
	}
}


void log_file::enableLog(bool is_en)
{
	is_enabled = is_en;
}