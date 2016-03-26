#ifndef l2_Segment_HEAD
#define l2_Segment_HEAD
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;
using namespace cv;
namespace omr_engine
{
	class L2_Segment
	{
	public:
		int yPosition; //symbol position along y-axis
		String symbolName;	//Symbol name as classified by ANN
		double accuracy;	// % confidence that ANN gave
	//	L2_Segment(int yPosition, String symbolName, double accuracy);
		//void printInfo();
		//String getSymbolName();
	//	double getAccuracy();
		//int getyPosition();
		L2_Segment();
		~L2_Segment();
	};
}
#endif