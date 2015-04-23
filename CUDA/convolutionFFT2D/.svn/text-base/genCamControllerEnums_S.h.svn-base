



#ifndef _GEN_CAM_CONTROLLERENUM_H
	#define _GEN_CAM_CONTROLLERENUM_H

#include <ADDriver.h>
#include <asynNDArrayDriver.h>
#include <NDArray.h>
//#include <paramAttribute.h>


//#define genCamControllerEnum

namespace genCamControllerEnum
{

struct int_string
{
	int x;
	char *str;
};



// here are some enums for why we call a callback

	enum callbackReasons
	{
		general,
		before_image,
		new_image,
		thread_suspend,
		make_new_thread,
		thread_resume,
		exiting,
		task_done,
		grab_mutex,
		release_mutex,
		call_error,
		sleep,
		wakeup_everyone,
		add_image_attribute

	};






	enum moreEnumStuff
	{
		subclassSerial = 300,
		subclassSerial2 = 400,
		subclassCoreco = 512,
		subclassExt = 768,
		maxEnum = 1023

	};


enum controller_params {

		callback_ptr = 256,
		image_mem_ptr
};




//
// ND Params
//




/** Enumeration of parameters that affect the behaviour of the detector.
  * These are the values that asyn will place in pasynUser->reason when the
  * standard asyn interface methods are called. */
typedef enum
{
    /*    Name          asyn interface  access   Description  */
    NDPortNameSelf,     /**< (asynOctet,    r/o) Asyn port name of this driver instance */

    /* Parameters defining the size of the array data from the detector.
     * NDArraySizeX and NDArraySizeY are the actual dimensions of the array data,
     * including effects of the region definition and binning */
    NDArraySizeX,       /**< (asynInt32,    r/o) Size of the array data in the X direction */
    NDArraySizeY,       /**< (asynInt32,    r/o) Size of the array data in the Y direction */
    NDArraySizeZ,       /**< (asynInt32,    r/o) Size of the array data in the Z direction */
    NDArraySize,        /**< (asynInt32,    r/o) Total size of array data in bytes */
    NDDataType,         /**< (asynInt32,    r/w) Data type (NDDataType_t) */
    NDColorMode,        /**< (asynInt32,    r/w) Color mode (NDColorMode_t) */

    /* Statistics on number of arrays collected */
    NDArrayCounter,     /**< (asynInt32,    r/w) Number of arrays since last reset */

    /* File name related parameters for saving data.
     * Drivers are not required to implement file saving, but if they do these parameters
     * should be used.
     * The driver will normally combine NDFilePath, NDFileName, and NDFileNumber into
     * a file name that order using the format specification in NDFileTemplate.
     * For example NDFileTemplate might be "%s%s_%d.tif" */
    NDFilePath,         /**< (asynOctet,    r/w) The file path */
    NDFileName,         /**< (asynOctet,    r/w) The file name */
    NDFileNumber,       /**< (asynInt32,    r/w) The next file number */
    NDFileTemplate,     /**< (asynOctet,    r/w) The file format template; C-style format string */
    NDAutoIncrement,    /**< (asynInt32,    r/w) Autoincrement file number; 0=No, 1=Yes */
    NDFullFileName,     /**< (asynOctet,    r/o) The actual complete file name for the last file saved */
    NDFileFormat,       /**< (asynInt32,    r/w) The data format to use for saving the file.  */
    NDAutoSave,         /**< (asynInt32,    r/w) Automatically save files */
    NDWriteFile,        /**< (asynInt32,    r/w) Manually save the most recent array to a file when value=1 */
    NDReadFile,         /**< (asynInt32,    r/w) Manually read file when value=1 */
    NDFileWriteMode,    /**< (asynInt32,    r/w) File saving mode (NDFileMode_t) */
    NDFileNumCapture,   /**< (asynInt32,    r/w) Number of arrays to capture */
    NDFileNumCaptured,  /**< (asynInt32,    r/o) Number of arrays already captured */
    NDFileCapture,      /**< (asynInt32,    r/w) Start or stop capturing arrays */

    NDAttributesFile,   /**< (asynOctet,    r/w) Attributes file name */

    /* The detector array data */
   //we cannont deal with gen pointers in asyn. the get and set functiohns are private to asyn
	// NDArrayData,        /**< (asynGenericPointer,   r/w) NDArray data */
    NDArrayCallbacks,   /**< (asynInt32,    r/w) Do callbacks with array data (0=No, 1=Yes) */

    NDLastStdParam      /**< The last standard ND driver parameter;
                          *  Derived classes must begin their specific parameter enums with this value */
} NDStdDriverParam_t;


//
// ADParams
//

/** Enumeration of parameters that affect the behaviour of the detector.
  * These are the values that asyn will place in pasynUser->reason when the
  * standard asyn interface methods are called. */
typedef enum
{
    /*    Name          asyn interface  access   Description  */
    ADManufacturer      /**< (asynOctet,    r/o) Detector manufacturer name */
      = NDLastStdParam,
    ADModel,            /**< (asynOctet,    r/o) Detector model name */

    ADGain,             /**< (asynFloat64,  r/w) Gain */

    /* Parameters that control the detector binning */
    ADBinX,             /**< (asynInt32,    r/w) Binning in the X direction */
    ADBinY,             /**< (asynInt32,    r/w) Binning in the Y direction */

    /* Parameters the control the region of the detector to be read out.
    * ADMinX, ADMinY, ADSizeX, and ADSizeY are in unbinned pixel units */
    ADMinX,             /**< (asynInt32,    r/w) First pixel in the X direction; 0 is the first pixel on the detector */
    ADMinY,             /**< (asynInt32,    r/w) First pixel in the Y direction; 0 is the first pixel on the detector */
    ADSizeX,            /**< (asynInt32,    r/w) Size of the region to read in the X direction */
    ADSizeY,            /**< (asynInt32,    r/w) Size of the region to read in the Y direction */
    ADMaxSizeX,         /**< (asynInt32,    r/o) Maximum (sensor) size in the X direction */
    ADMaxSizeY,         /**< (asynInt32,    r/o) Maximum (sensor) size in the Y direction */


    /* Parameters that control the orientation of the image */
    ADReverseX,         /**< (asynInt32,    r/w) Reverse image in the X direction (0=No, 1=Yes) */
    ADReverseY,         /**< (asynInt32,    r/w) Reverse image in the Y direction (0=No, 1=Yes) */

    /* Parameters defining the acquisition parameters. */
    ADFrameType,        /**< (asynInt32,    r/w) Frame type (ADFrameType_t) */
    ADImageMode,        /**< (asynInt32,    r/w) Image mode (ADImageMode_t) */
    ADTriggerMode,      /**< (asynInt32,    r/w) Trigger mode (ADTriggerMode_t) */
    ADNumExposures,     /**< (asynInt32,    r/w) Number of exposures per image to acquire */
    ADNumImages,        /**< (asynInt32,    r/w) Number of images to acquire in one acquisition sequence */
    ADAcquireTime,      /**< (asynFloat64,  r/w) Acquisition time per image */
    ADAcquirePeriod,    /**< (asynFloat64,  r/w) Acquisition period between images */
    ADStatus,           /**< (asynInt32,    r/o) Acquisition status (ADStatus_t) */
    ADAcquire,          /**< (asynInt32,    r/w) Start(1) or Stop(0) acquisition */

    /* Shutter parameters */
    ADShutterControl,   /**< (asynInt32,    r/w) (ADShutterStatus_t) Open (1) or Close(0) shutter */
    ADShutterControlEPICS, /**< (asynInt32, r/o) (ADShutterStatus_t) Open (1) or Close(0) EPICS shutter */
    ADShutterStatus,    /**< (asynInt32,    r/o) (ADShutterStatus_t) Shutter Open (1) or Closed(0) */
    ADShutterMode,      /**< (asynInt32,    r/w) (ADShutterMode_t) Use EPICS or detector shutter */
    ADShutterOpenDelay, /**< (asynFloat64,  r/w) Time for shutter to open */
    ADShutterCloseDelay,/**< (asynFloat64,  r/w) Time for shutter to close */

    /* Temperature parameters */
    ADTemperature,      /**< (asynFloat64,  r/w) Detector temperature */

    /* Statistics on number of images collected and the image rate */
    ADNumImagesCounter, /**< (asynInt32,    r/o) Number of images collected in current acquisition sequence */
    ADNumExposuresCounter, /**< (asynInt32, r/o) Number of exposures collected for current image */
    ADTimeRemaining,    /**< (asynFloat64,  r/o) Acquisition time remaining */

    /* Status reading */
    ADReadStatus,      /**< (asynInt32,     r/w) Write 1 to force a read of detector status */

    /* Status message strings */
    ADStatusMessage,    /**< (asynOctet,    r/o) Status message */
    ADStringToServer,   /**< (asynOctet,    r/o) String sent to server for message-based drivers */
    ADStringFromServer, /**< (asynOctet,    r/o) String received from server for message-based drivers */

    ADLastStdParam      /**< The last standard driver parameter;
                          * Drivers must begin their detector-specific parameter enums with this value */
} ADStdDriverParam_t;





static int_string ADStdDriverParamString[] = {
   {NDPortNameSelf,   "PORT_NAME_SELF"},
    {NDArraySizeX,     "ARRAY_SIZE_X"},
    {NDArraySizeY,     "ARRAY_SIZE_Y"},
    {NDArraySizeZ,     "ARRAY_SIZE_Z"},
    {NDArraySize,      "ARRAY_SIZE"  },
    {NDDataType,       "DATA_TYPE"   },
    {NDColorMode,      "COLOR_MODE"  },

    {NDArrayCounter,   "ARRAY_COUNTER" },

    {NDFilePath,       "FILE_PATH"     },
    {NDFileName,       "FILE_NAME"     },
    {NDFileNumber,     "FILE_NUMBER"   },
    {NDFileTemplate,   "FILE_TEMPLATE" },
    {NDAutoIncrement,  "AUTO_INCREMENT"},
    {NDFullFileName,   "FULL_FILE_NAME"},
    {NDFileFormat,     "FILE_FORMAT"   },
    {NDAutoSave,       "AUTO_SAVE"     },
    {NDWriteFile,      "WRITE_FILE"    },
    {NDReadFile,       "READ_FILE"     },
    {NDFileWriteMode,  "WRITE_MODE"    },
    {NDFileNumCapture, "NUM_CAPTURE"   },
    {NDFileNumCaptured,"NUM_CAPTURED"  },
    {NDFileCapture,    "CAPTURE"       },

    {NDAttributesFile, "ND_ATTRIBUTES_FILE"},

   // {NDArrayData,      "NDARRAY_DATA"  },
    {NDArrayCallbacks, "ARRAY_CALLBACKS"  },

    {ADManufacturer,   "MANUFACTURER"},
    {ADModel,          "MODEL"       },

    {ADGain,           "GAIN"        },

    {ADBinX,           "BIN_X"       },
    {ADBinY,           "BIN_Y"       },

    {ADMinX,           "MIN_X"       },
    {ADMinY,           "MIN_Y"       },
    {ADSizeX,          "SIZE_X"      },
    {ADSizeY,          "SIZE_Y"      },
    {ADMaxSizeX,       "MAX_SIZE_X"  },
    {ADMaxSizeY,       "MAX_SIZE_Y"  },
    {ADReverseX,       "REVERSE_X"   },
    {ADReverseY,       "REVERSE_Y"   },

    {ADFrameType,      "FRAME_TYPE"  },
    {ADImageMode,      "IMAGE_MODE"  },
    {ADNumExposures,   "NEXPOSURES"  },
    {ADNumExposuresCounter, "NEXPOSURES_COUNTER"  },
    {ADNumImages,      "NIMAGES"     },
    {ADNumImagesCounter, "NIMAGES_COUNTER"},
    {ADAcquireTime,    "ACQ_TIME"    },
    {ADAcquirePeriod,  "ACQ_PERIOD"  },
    {ADTimeRemaining,  "TIME_REMAINING"},
    {ADStatus,         "STATUS"      },
    {ADTriggerMode,    "TRIGGER_MODE"},
    {ADAcquire,        "ACQUIRE"     },

    {ADShutterControl,   "SHUTTER_CONTROL"},
    {ADShutterControlEPICS, "SHUTTER_CONTROL_EPICS"},
    {ADShutterStatus,    "SHUTTER_STATUS"},
    {ADShutterMode,      "SHUTTER_MODE"        },
    {ADShutterOpenDelay, "SHUTTER_OPEN_DELAY"  },
    {ADShutterCloseDelay,"SHUTTER_CLOSE_DELAY" },

    {ADTemperature,    "TEMPERATURE" },

    {ADReadStatus,     "READ_STATUS"     },

    {ADStatusMessage,  "STATUS_MESSAGE"     },
    {ADStringToServer, "STRING_TO_SERVER"   },
    {ADStringFromServer,"STRING_FROM_SERVER"},

		
   {callback_ptr,"callback_ptr"},
   {image_mem_ptr,"image_mem_ptr"}


};

enum {num_gen_params=62};



};




#endif
