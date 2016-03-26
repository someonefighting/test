#ifndef YProjection_HEAD 
#define YProjection_HEAD 


#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include"show.h"
using namespace std;
using namespace cv;


namespace omr_engine
{
	class YProjection
	{
	public:
		Mat buffImage;
		int row;
		int *yProjection;

		YProjection();
		~YProjection();
		YProjection(Mat buffImage);
		void calcYProjection(int startH, int endH, int startW, int endW);
		int* getYProjection();
		void printYProjection();
		
		int getrow();
	};
}



#endif

	