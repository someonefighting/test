#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include"YProjection.h"
#include"StaveParameters.h"
#include"StaveDetection.h"
#include"StaveBoundaries.h"
#include"show.h"
#include"NoteHeadDetection.h"
using namespace cv;
using namespace std;
using namespace omr_engine;

int main()
{		
	Mat img = imread("summer.jpg");
	IplImage p_img(img);
	IplImage *g_pGrayImage = cvCreateImage(cvGetSize(&p_img), IPL_DEPTH_8U, 1);
		
	cvCvtColor(&p_img, g_pGrayImage, CV_BGR2GRAY);
	
	 // 创建二值图  
	 IplImage *g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
	 cvThreshold(g_pGrayImage, g_pBinaryImage, 200, 255, CV_THRESH_BINARY);
	 //水平投影

	YProjection yproj(img);
	cv::Mat m = cv::cvarrToMat(g_pBinaryImage);
	yproj.calcYProjection(0, m.rows , 0, m.cols );
	//yproj.printYProjection();
	//staveParam
	//show(&m);
	StaveParameters staveparam(m);
	staveparam.calcParameters();
	//staveparam.printHistogram();

	//StaveDetection
	StaveDetection stavedetection(yproj,staveparam);
	stavedetection.locateStaves();
	stavedetection.calcNoteDistance();

	//staveboundaries
	StaveBoundaries staveBoundary(m,stavedetection);
	staveBoundary.findBoundaries();
	staveBoundary.findMeasures();
	staveBoundary.findGroupsOfNotes();
	int num = 0;
	//cut 
	for (int i = 0; i < staveBoundary.staveDetection.staveList.size(); i++)
	{
		Staves temp = staveBoundary.staveDetection.staveList.at(i);
		for (int j = 0; j < temp.symbolPos.size(); j++)
		{
			int startH = temp.top - (double)0.5*(temp.bottom - temp.top);
			int endH = temp.bottom + (double)0.5*(temp.bottom - temp.top);
			Mat submat = m(Range(startH, endH), Range(temp.symbolPos.at(j).start - 2, temp.symbolPos.at(j).stop + 2));
			char c[10];
			sprintf(c, "cut\\%d.jpg", num++);
			imwrite(c, submat);
		}
	}
	//notedetection
	
	NoteHeadDetection noteheaddec(m,staveparam);
	vector<vector<NoteHead>> notelist;
	vector<PitchCalculation> noteList;
#ifdef _DEBUG
	Mat g = imread("summer.jpg");
	IplImage ori = IplImage(g);
#endif
	for (int i = 0; i < staveBoundary.staveDetection.staveList.size(); i++)
	{
		Staves temp = staveBoundary.staveDetection.staveList.at(i);
		for (int j = 0; j < temp.symbolPos.size(); j++)
		{
			int startH = temp.top-(double)0.5*( temp.bottom-temp.top);
			int endH = temp.bottom + (double)0.5*(temp.bottom - temp.top);
			notelist.push_back(noteheaddec.findNotes(g,temp.top, temp.bottom, temp.symbolPos.at(j).start, temp.symbolPos.at(j).stop));
				//Cannot calculate pitch here as we need more info about note segments...
			//add pitchcaculation
			PitchCalculation temp = PitchCalculation();
				//temp.setNote(yPos, 5, refPos);
				noteList.push_back(temp);
			
			
#ifndef _DEBUG

			for (int k = 0; k < notelist.at(i).size(); k++)
			{
				CvPoint p1 = CvPoint(notelist.at(i).at(k).x, temp.top);
				CvPoint p2 = CvPoint(notelist.at(i).at(k).y, temp.bottom);
				rectangle(img, p1, p2, Scalar(255, 0, 0),1,8);

				show(&img);
			}
#endif			
		}		
	}
	
	imwrite("img.jpg", g);

	return 0;
	//StaveDetection(YProjection yproj, StaveParameters staveParams);
	
}