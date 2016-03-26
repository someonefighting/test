#ifndef NoteHeadDetection_HEAD
#define NoteHeadDetection_HEAD
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include"StaveDetection.h"
#include"L2_Segment.h"
#include"Staves.h"
#include"NoteStem.h"
#include"XProjection.h"
#include"StaveParameters.h"
#include"YProjection.h"
#include"NoteHead.h"
using namespace std;
using namespace cv;

namespace omr_engine
{
	class NoteHeadDetection
	{
	public:
		StaveParameters staveParams;
		int* yPos;
		int* yPosBeforeFilter;
		int* xProjection;
		int* xProjectionBeforeFilter;
		int* yProjection;
		YProjection yProj;
		int height;
		int width;
		Mat buffImage;

		NoteHeadDetection(Mat buffImage, StaveParameters staveParams);
		vector<NoteHead> findNotes(Mat img,int startH, int endH, int startW, int endW);
		int locateStartNote();
		NoteStem locateStem(int x, int startH, int endH);
		void Xprojection(int startH, int endH, int i, int startW);
		void YProjRLE(int startH, int endH, int i, int startW);
		void filteryProj();
		int findyPos(int tempPos);
		void filterXproj();
		void filterMeanXProjection();
		void filterYpos(int startW, int startH);
		int* getYPos();
		int* getYposBeforeFilter();
		int* getXProjection();
		int* getXProjectionBeforeFilter();
		void printYPos(int width, int startH);
		void printXProj(int width);
		NoteHeadDetection();
		~NoteHeadDetection();
	};
}
#endif
