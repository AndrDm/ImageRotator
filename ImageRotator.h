// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the IMAGEROTATOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// IMAGEROTATOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef IMAGEROTATOR_EXPORTS
#define IMAGEROTATOR_API extern "C" __declspec(dllexport)
#else
#define IMAGEROTATOR_API __declspec(dllimport)
#endif


typedef struct {
	int32_t dimSizes[2];
	uint8_t pixel[1];
} ImageArr, ** ImageArrHdl;

//============================================================================
//  Error Codes
//============================================================================
#define OCV_SUCCESS		 0 // No error.
#define OCV_CAM_OPEN_ERROR  5000 // Unable to open Camera.
#define OCV_FRAME_ERROR	 5001 // Unable to get Frame.
#define OCV_NULL_POINTER	5002 // Null pointer.
#define OCV_WRONG_VIDEO	5003 // Unable to open video file.


typedef struct {
	LVBoolean status;
	int32_t code;
	LStrHandle source;
} LVError;

MgErr SetError(LVError* Error, LVBoolean Status, int32_t Code, const char* Source)
{
	Error->status = Status;
	Error->code = Code;

	MgErr err = NumericArrayResize(uB, 1, (UHandle*)&(Error->source), strlen(Source));
	if (!err) {
		MoveBlock(Source, LStrBuf(*(Error->source)), strlen(Source));
		LStrLen(*(Error->source)) = (int32)strlen(Source);
	}
	return err;
}


IMAGEROTATOR_API void fnImageRotator(ImageArrHdl src, ImageArrHdl dst, double angle, LVError* Error);
IMAGEROTATOR_API void CreateVideo(char* file, ImageArrHdl src, LVError* Error);
IMAGEROTATOR_API void AddFrame(ImageArrHdl src, LVError* Error);
IMAGEROTATOR_API void VideoClose(LVError* Error);
