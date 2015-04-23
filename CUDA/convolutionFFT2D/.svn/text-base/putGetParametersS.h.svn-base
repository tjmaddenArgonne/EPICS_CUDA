/*******************************************************************************
 *
 *
 *	Author: Tim Madden
 *	Date:	7/16/03
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

#include <map>
#include "genCamControllerEnums_S.h"

#include "ADDriver.h"
#include "asynNDArrayDriver.h"
#include "NDArray.h"

using namespace genCamControllerEnum;
/*
 * Double incluson protection.
 */
#ifndef _PUT_GET_PARAMETERS_H
	#define _PUT_GET_PARAMETERS_H




/*******************************************************************************
 *
 * class to contain arrays of volatile bools, floats, ints, void pointers.
 * this is for thread safe communication between threads, by setting flags etc.
 *
 ******************************************************************************/

class __declspec(dllexport) putGetParameters
{
	public:
		typedef enum {
			paramUndef,     /**< Undefined */
			paramInt,       /**< int  parameter */
			paramDouble,    /**< double parameter */
			paramString,     /**< Dynamic length string parameter */
			paramPtr
		} paramType_S;

		putGetParameters(ADDriver *ad);
		virtual ~putGetParameters();

		virtual int getAsynParam(const int index);

		// Download serial stream to camera.
		virtual int putIntParam(const int index, const int value);

		virtual int getIntParam(const int index);
		virtual asynStatus getIntegerParam(const int index, int *value);
		virtual asynStatus setIntegerParam(int index, int value);

		virtual int putBoolParam(const int index, const bool value);
		virtual int getBoolParam(const int index, bool &value);
		virtual bool getBoolParam(const int index);

		virtual int putFloatParam(const int index, const float value);
		virtual int getFloatParam(const int index, float &value);
		virtual float getFloatParam(const int index);

		virtual asynStatus setDoubleParam( int index,  double value);
		virtual int putDoubleParam(const int index, const double value);
		virtual asynStatus getDoubleParam( int index, double *value);
		virtual double getDoubleParam(const int index);

		virtual int putPtrParam(const int index, const void* value);
		virtual int getPtrParam(const int index, void* &value);
		virtual void* getPtrParam(const int index);

		virtual asynStatus setStringParam( int index, const char* value);
		virtual int putStringParam(const int index, const char* value);
		virtual int getStringParam(const int index,  char* value);
		virtual char* getStringParam(const int index);
		virtual asynStatus getStringParam( int index,int maxchar,  char* value);


		virtual int getParamType(const int index);

		virtual int getNumParameters(void) {return num_parameters;};

		/*
		 * The idea here is that if we set a parameter, say, detector_gain, then some software
		 * must know that it must iupdate the gain on the detector. So it looks in the flags for
		 * any parameters that were put. After iuopdateding the detector,  the software calls
		 * clearChanges.
		 */
		// if we put a param, it flags that that param is changed.
		virtual void putIsChanged(const int index);

		// return true of param X is changed.
		virtual bool getIsChanged(const int index);
		// clears all flags that  params  changed.
		virtual int clearChanges(void);
		virtual int clearChanges(const int index);
		// if any params changed, return true
		virtual bool getIsAnyChanges(void);
		virtual char* getStringFromParam(const int index);
		virtual int createIfNotExist(const int index, paramType_S ptype);


		asynStatus callParamCallbacks();


		// These datatypes shamelessly copued and modified from asynPortDriver.h from the EPICS Gurus.

// from asynPortDriver.h we get this:

/** Parameter data types for the parameter library */
//typedef enum {
//    paramUndef,     /**< Undefined */
//    paramInt,       /**< int  parameter */
//    paramDouble,    /**< double parameter */
//    paramString     /**< Dynamic length string parameter */
//} paramType;
// we wish to add the pointer type. so we have a new enum...



	// this subclass based on asynPortDriver.h paramlist class.
	class paramVal
	{
		public:
		paramType_S type;
		// is changed to trigger device
		bool is_changed;
		//AD1-6 has integer params from asyn. done by create param. We must map the local 
		//enum param number to the asyn integer param in ad1-6. also we need a reverse mapping.
		// this is done by a STL:set class to find the enum param here, given the asyn param..
		//we can then hide all the AD1-6 wierd stuff and not have to rewrite tons of AD1-5 code.
		int asyn_param_id;
		// just for testing add a time stamp as to when param was changed
		// this way we can tell if some thread is dead and not responding to params.
		double timestamp;
	};
	enum {
		def_num_param = 2048

	};

		 // if all params are to be shared, we declare some static memory.
 static paramVal stat_vals[def_num_param];
		// set that param X is changed.
		//void putIsChanged(const int index);

 bool is_share_params;
static bool is_init_params;

 //reverse lookup map- given the asyn int param, find the enum param used here...
 std::map <int, int> *asynToEnum;

protected:
		//
		//keep track of params and param strings in inheritance chain.
		//
		enum {max_inherits=32};
		// we keep track of how many classes inherit from this class.
		// this counts GENERATATIONs, not how many calsses are consturcted.
		// for example, a serial download obj inherit from this. then something else
		//. inherit from serial download, that gives num_inherits = 2.
		int num_inherits;
		// each inherited class has its own set of parameters. To allow
		// inherited calsss to simply add to the list of parameters we
		// keep track of all the parameter lists in an array. each derived
		// class gets a slot in the array.
		int_string *paramIntStrArray[max_inherits];
		int num_params[max_inherits];

		//
		// keep track of all param lists and strings regardless of inheritance chain.
		//
		enum {max_objects=128};
		static int num_objects;
		static int_string *obj_paramIntStrArray[];
		static int obj_num_params[];



ADDriver *my_addriver;

 int num_parameters;
 // in most cases we do NOT share params between objs. so we have independant pointer...
 paramVal* vals;

 
 static char scratch_string[512];

};

#endif
