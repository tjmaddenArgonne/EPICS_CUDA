#include "putGetParameters.h"
#include "ccd_exception.h"
//putGetParameters(){}
		//virtual ~putGetParameters() {}

		// Download serial stream to camera.
 // if all params are to be shared, we declare some static memory.
putGetParameters::paramVal putGetParameters::stat_vals[putGetParameters::def_num_param]; 	
bool putGetParameters::is_init_params=true;


/****************************************************************************************
 *
 *
 ***************************************************************************************/
putGetParameters::putGetParameters(const bool is_share_params)
{
	int k;
	
	if (is_share_params)
		vals = stat_vals;
	else
		vals = new paramVal[def_num_param];
	
	num_parameters = def_num_param;

	if (is_share_params&&is_init_params || !is_share_params)
	{
		for (k=0; k<num_parameters; k++)
		{
			vals[k].is_changed=false;
			vals[k].callback_ptr=0;
			vals[k].type=paramUndef;
			vals[k].data.pval=0;
		}
		is_init_params=false;// for shared parsm we init only once.
	}


}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
putGetParameters::~putGetParameters()
{

	int k;
	for (k=0; k<num_parameters; k++)
	{
		if (vals[k].type==paramString)
		{
			if (vals[k].data.sval != 0)
				delete [] (vals[k].data.sval);
		}
	}


	// delete all params.
	delete[] vals;
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::putIntParam(const int index, const int value)
{
	//!!DEBUG
	int xxx;
	if (index>=278 && index<300)
	{
		xxx=1;//trap here
	}
	//!!
	putIsChanged(index);

	if (index>=0 && index < num_parameters)
	{
		vals[index].data.ival = value;
		vals[index].type=paramInt;
		return 0;
	}
	return -1;

}
/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getIntParam(const int index, int &value)
{
	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramInt)
		{
			value = vals[index].data.ival;
			return 0;
		}
	}
	return -1;
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getIntParam(const int index)
{
	int value;

	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramInt)
		{
			value = vals[index].data.ival;
			return value;
		}

	}
	return -1;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::putBoolParam(const int index, const bool value)
{ 
	throw ccd_exception("bool not supported");
	putIsChanged(index);

	if (index>=0 && index < num_parameters)
	{
		vals[index].data.bval = value;
		vals[index].type=paramBool;


		return 0;
	}
	return -1;

}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getBoolParam(const int index, bool &value)
{
		throw ccd_exception("bool not supported");

	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramBool)
		{
			value = vals[index].data.bval;
			return 0;
		}
	}
	return -1;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
bool putGetParameters::getBoolParam(const int index)
{
	bool value;
	throw ccd_exception("bool not supported");

	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramBool)
		{
			value = vals[index].data.bval;
			return value;
		}
	}
	return false;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::putFloatParam(const int index, const float value)
{
		throw ccd_exception("float not supported");

	putIsChanged(index);

	if (index>=0 && index < num_parameters)
	{
		vals[index].data.fval = value;
		vals[index].type=paramFloat;

		return 0;
	}
	return -1;

}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getFloatParam(const int index, float &value)
{		throw ccd_exception("float not supported");

	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramFloat)
		{
			value = vals[index].data.fval;
			return 0;
		}
	}
	return -1;

}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
float putGetParameters::getFloatParam(const int index)
{
	float value;
		throw ccd_exception("float not supported");

	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramFloat)
		{
			value = vals[index].data.fval;
			return value;
		}
	}
	return -1;

}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::putDoubleParam(const int index, const double value)
{
	putIsChanged(index);

	if (index>=0 && index < num_parameters)
	{
		vals[index].data.dval = value;
		vals[index].type=paramDouble;

		return 0;
	}
	return -1;

}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getDoubleParam(const int index, double &value)
{
	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramDouble)
		{
			value = vals[index].data.dval;
			return 0;
		}
	}
	return -1;

}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
double putGetParameters::getDoubleParam(const int index)
{
	double value;

	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramDouble)
		{
			value = vals[index].data.dval;
			return value;
		}
	}
	return -1;

}




/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::putPtrParam(const int index, const void* value)
{
	putIsChanged(index);

	if (index>=0 && index < num_parameters)
	{
		vals[index].data.pval = (void*) value;
		vals[index].type=paramPtr;


		return 0;
	}
	return -1;

}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getPtrParam(const int index, void* &value)
{
	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramPtr)
		{
			value = vals[index].data.pval;
			return 0;
		}
	}
	return -1;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
void* putGetParameters::getPtrParam(const int index)
{
	void* value;
	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramPtr)
		{
			value = vals[index].data.pval;
			return value;
		}
	}
	return 0;
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::putStringParam(const int index, const char* value)
{
	putIsChanged(index);
	char *newstr;
	if (index>=0 && index < num_parameters)
	{
	    if (vals[index].data.sval == 0)
			newstr = new char[param_str_len];
		else
			newstr = vals[index].data.sval;


		strcpy(newstr,value);
		vals[index].data.sval = newstr;
		vals[index].type=paramString;
		return 0;
	}
	return -1;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getStringParam(const int index, const char* value)
{
	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramString)
		{
			value = vals[index].data.sval;
			return 0;
		}
		return 0;
	}
	return -1;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
char* putGetParameters::getStringParam(const int index)
{
	char* value;
	if (index>=0 && index < num_parameters)
	{
		if (vals[index].type == paramString)
		{
			value = vals[index].data.sval;
			return value;
		}
	}
	return 0;
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::getParamType(const int index)
{

	if (index>=0 && index < num_parameters)
	{

			return((int)(vals[index].type));

	}
	return -1;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/

void putGetParameters::putIsChanged(const int index)
{
	if (index>=0 && index < num_parameters)
	{
		vals[index].is_changed = true;
	}
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::clearChanges(void)
{

	for (int k=0; k<num_parameters; k++)
	{
		vals[k].is_changed=false;

	}
	return 0;
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
int putGetParameters::clearChanges(const int index)
{

	if (index>=0 && index < num_parameters)
	{
		vals[index].is_changed = false;
	}
	return 0;
}

/****************************************************************************************
 *
 *
 ***************************************************************************************/
bool putGetParameters::getIsAnyChanges(void)
{

	return true;
}


/****************************************************************************************
 *
 *
 ***************************************************************************************/
bool putGetParameters::getIsChanged(const int index)
{
	if (index>=0 && index < num_parameters)
	{
		return (vals[index].is_changed);
	}

	return false;

}


