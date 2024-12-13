// ImageRotator.cpp : Defines the exported functions for the DLL.
//
#include <opencv2/opencv.hpp>
#include <cintools/extcode.h>
#include "framework.h"
#include "ImageRotator.h"

using namespace cv;
using namespace std;

static VideoWriter* vid;

IMAGEROTATOR_API void fnImageRotator(ImageArrHdl src, ImageArrHdl dst, double angle, LVError* Error)
{
    Mat  M, Dst;
    int width = (*(src))->dimSizes[0];
    int height = (*(src))->dimSizes[1];
    Size size(height, width); //rows, cols
    if (!((*(src))->pixel)) { SetError(Error, true, OCV_NULL_POINTER, "Null pointer"); return; };
    Mat Src(size, CV_8U, (*(src))->pixel, width);

    //https://docs.opencv.org/4.x/da/d6e/tutorial_py_geometric_transformations.html
    M = getRotationMatrix2D(Point((width - 1) / 2, (height - 1) / 2), angle, 1.0);
    warpAffine(Src, Dst, M, Size(width, height));

    MgErr err = NumericArrayResize(uL, 2, (UHandle*)&(dst), width * height);
    (*(dst))->dimSizes[0] = width;
    (*(dst))->dimSizes[1] = height;

    uint8_t* pixel_out = (*(dst))->pixel;
    uint8_t* pixel_in = Dst.data;

    if (!pixel_in || !pixel_out) { SetError(Error, true, OCV_NULL_POINTER, "Null pointer"); return; };
    // Copy,line by line, the image into the LabVIEW Arrray
    for (int y = 0; y < height; ++y) {
        memcpy_s(pixel_out, width, pixel_in, width);
        pixel_out += width; pixel_in += width;
    }
}


IMAGEROTATOR_API void CreateVideo(char* file, ImageArrHdl src, LVError* Error)
{
    Size size = Size((*(src))->dimSizes[1], (*(src))->dimSizes[0]);
    vid = new VideoWriter(file, VideoWriter::fourcc('m', 'p', '4', 'v'), 25, size, false); //False - Grays
    if (!vid) SetError(Error, true, 5000, "Unable to open Video File");
}


IMAGEROTATOR_API void AddFrame(ImageArrHdl src, LVError* Error)
{
    Size size((*(src))->dimSizes[1], (*(src))->dimSizes[0]); //rows, cols
    Mat frame(size, CV_8U, (*(src))->pixel, (*(src))->dimSizes[0]);
	vid->write(frame);
}


IMAGEROTATOR_API void VideoClose(LVError* Error)
{
    vid->release();
}
