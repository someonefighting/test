#ifndef XProjection_HEAD 
#define XProjection_HEAD 

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "YProjection.h"
using namespace std;
using namespace cv;


namespace omr_engine
{
	class XProjection
	{
	public:
		Mat buffImage;
		int size;
		int *xProjection;

		XProjection();
		~XProjection();
		XProjection(Mat buffImage);
		void calcXProjection(int startH, int endH, int startW, int endW);
		int* getXProjection();
		void printXProjection();
	
	};
}



#endif 