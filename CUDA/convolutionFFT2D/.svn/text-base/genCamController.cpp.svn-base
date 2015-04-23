#include "genCamController.h"
using namespace genCamControllerEnum;

int genCamController::id_counter=0;

genCamController::genCamController() :
	exe(),
	putGetParameters(true)// w
{
	this_id=genCamController::id_counter;
	genCamController::id_counter++;

	// this is base class for detector classes...
	num_inherits=0;

	sleepy_time=10;

	wait_counter=0;
	pre_wait_counter=0;
	wait_param=0;

	attribute_name= new char[80];
	attribute_description=new char[512];
	attribute_datatype=(genCamControllerEnum::NDAttrDataType_t)0;
	attribute_pvalue=0;



	// this base class does not need this. the int str arrau is in ARead Detector.
	paramIntStrArray[num_inherits]=ADStdDriverParamString;
	num_params[num_inherits] = num_gen_params;


	callbackFunction = defaultCallback;
	putStringParam(NDPortNameSelf,"Null");

	putStringParam(ADManufacturer,"Null");
	putStringParam(ADModel,"Null");
	putDoubleParam(ADGain,0.0);
	putIntParam(ADBinX,0);
	putIntParam(ADBinY,0);
	putIntParam(ADMinX,0);
	putIntParam(ADMinY,0);
	putIntParam(ADSizeX,0);
	putIntParam(ADSizeY,0);
	putIntParam(ADMaxSizeX,0);
	putIntParam(ADMaxSizeY,0);
	putIntParam(ADReverseX,0);
	putIntParam(ADReverseY,0);
	putIntParam(NDArraySizeX,0);
	putIntParam(NDArraySizeY,0);
	putIntParam(NDArraySizeZ,0);
	putIntParam(NDArraySize,0);
	putIntParam(NDDataType,0);
	putIntParam(ADFrameType,0);
	putIntParam(ADImageMode,0);
	putIntParam(ADTriggerMode,0);
	putIntParam(ADNumExposures,0);
	putIntParam(ADNumImages,0);

	putDoubleParam(ADAcquireTime,0.0);
	putDoubleParam(ADAcquirePeriod,0.0);

	putIntParam(ADStatus,0);
	putIntParam(ADAcquire,0);
	putIntParam(ADShutterControl,0);
	putIntParam(ADShutterControlEPICS,0);
	putIntParam(ADShutterStatus,0);
	putIntParam(ADShutterMode,0);
	putIntParam(ADShutterOpenDelay,0);
	putIntParam(ADShutterCloseDelay,0);

	putDoubleParam(ADTemperature,0.0);

	putIntParam(NDArrayCounter,0);
	putIntParam(ADNumImagesCounter,0);
	putIntParam(ADNumExposuresCounter,0);
	putDoubleParam(ADTimeRemaining,0.0);



	putStringParam(NDFilePath,"NULL");
	putStringParam(NDFileName,"NULL");
	putIntParam(NDFileNumber,0);
	putStringParam(NDFileTemplate,"NULL");

	putIntParam(NDAutoIncrement,0);
	putIntParam(NDFileFormat,0);
	putIntParam(NDAutoSave,0);
	putIntParam(NDWriteFile,0);
	putIntParam(NDReadFile,0);
	putIntParam(NDFileWriteMode,0);
	putIntParam(NDFileNumCaptured,0);
	putIntParam(NDFileNumCapture,0);
	putIntParam(NDFileCapture,0);
	putIntParam(ADReadStatus,0);
	putIntParam(NDArrayCallbacks,0);

	putStringParam(NDFullFileName,"NULL");
	putStringParam(ADStatusMessage,"NULL");
	putStringParam(ADStringToServer,"NULL");
	putStringParam(ADStringFromServer,"NULL");

	putPtrParam(NDArrayData,0);
	
	putIntParam(gpu_img_sizex,1024);
	putIntParam(gpu_img_sizey,1024);
	putIntParam(gpu_ret_new_img,1);



}

genCamController::~genCamController()
{
	delete(attribute_name);
	delete(attribute_description);


}
int_string* genCamController::getParamIntStringArray()
{
	return(paramIntStrArray[num_inherits]);
}
int genCamController::getNumParams()
{
	return(num_params[num_inherits]);
}

int genCamController::getTotalNumParams()
{
	int tot_num_params=0;
	for (int k=0;k<=num_inherits; k++)
		tot_num_params+=num_params[k];

	return(tot_num_params);
}

int_string* genCamController::getCompleteParamIntStrArray()
{
	int k,m,n;
	int_string *complete_params;
	int tot_num_params=getTotalNumParams();
	int_string *param_ptr;

	complete_params=new int_string[tot_num_params];
	n=0;
	for (k=0;k<=num_inherits; k++)
	{
		for (m=0;m<num_params[k]; m++)
		{
			//paramIntStrArray[k] points to 1st element of int_string array.
			//paramIntStrArray[k][m] may point to sizeof(int_string)*m + paramIntStrArray[k],
			//or it may point to sizeof(my_whole_int_str_array)*m + paramIntStrArray[k].
			// who knows...
			// to be safe let us explicitly get the correct address.
			param_ptr = paramIntStrArray[k] +m;
			complete_params[n].x =param_ptr->x;
			complete_params[n].str =param_ptr->str;
			n++;
			
		}
	}
	return(complete_params);
}


int genCamController::getThisID()
{
  return(this_id);
}

void genCamController::imageCallback(void *ptr)
{
	(*callbackFunction)(genCamControllerEnum::thread_resume, this);

}

// Threading and mutexes etc.. wrappers for the callback function.
void genCamController::sleep(int ms)
{
	this->sleepy_time = ms;
	(*callbackFunction)(genCamControllerEnum::sleep, this);
}

void genCamController::suspend(void)
{
		this->sleepy_time = 20000;

	(*callbackFunction)(genCamControllerEnum::thread_suspend, this);

}
void genCamController::suspend(int ms)
{	
	this->sleepy_time = ms;
	(*callbackFunction)(genCamControllerEnum::thread_suspend, this);

}

void genCamController::wakeUp(void)
{
	(*callbackFunction)(genCamControllerEnum::thread_resume, this);

}

void genCamController::wakeUpEveryone(void)
{
	(*callbackFunction)(genCamControllerEnum::wakeup_everyone, this);

}


void genCamController::preWait(int param)
{
	pre_wait_counter=wait_counter;
	wait_param=param;
}

void genCamController::waitDone(int timeout_ms)
{
	int time = 0;
	// make sure that controller is not suspended...
	// the CALLED thead will wake up. that is the one associated with this.
	// this is not necessarily the calling thread of this function.
	// example: if coreco card is on this, then coreco thread wakes up.
	// serial port thread is the calling thread.
	this->wakeUp();

	// this object should call notifyWauters to inc wait_counter, thus
	// breaking the while loop.
	while(wait_counter==pre_wait_counter && time<timeout_ms)
	{
		// the CALLIONG thread will sleep, not the one associate with this poiunter.
		// example: if coreco card is this, then the coreco thread just runs.
		// the caller is the serial port code. the serial port thread then sleeps.
		this->sleep(100);
		time+= 100;
	}
}

void genCamController::notifyWaiters(int param)
{
	if (param==wait_param)
		wait_counter++;
}



void genCamController::addAttribute(
			const char *pName,
			const char *pDescription,
            genCamControllerEnum::NDAttrDataType_t dataType,
            void *pValue)
{
	strcpy(this->attribute_name,pName);
	strcpy(this->attribute_description, pDescription);
	this->attribute_datatype=dataType;
	this->attribute_pvalue=pValue;

	callbackFunction(genCamControllerEnum::add_image_attribute,this);

}