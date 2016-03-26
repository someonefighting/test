#include "StaveBoundaries.h"

using namespace omr_engine;
StaveBoundaries::StaveBoundaries()
{

}


StaveBoundaries::~StaveBoundaries()
{

}
StaveBoundaries::StaveBoundaries(Mat Image, StaveDetection staveDetec)
{
	buffImage = Image;
	staveDetection = staveDetec;
}
void StaveBoundaries::findBoundaries()
{
	vector<Staves>::iterator it = staveDetection.staveList.begin();
	while (true)
	{
		if (it == staveDetection.staveList.end())
			break;
		int start = doLeftBoundaries(*it);
		int end = doRightBoundaries(*it);
		(*it).start = start;
		(*it).end = end;
		it++;
	}
}
int StaveBoundaries::doLeftBoundaries(Staves stave)
{
	for (int i = 0; i < buffImage.cols; i += 1)
	{
		bool res = isStaveOnly(stave, i, 1);
		if (!res)
		{
			return i;
		}
	}
	return 0;
}
int StaveBoundaries::doRightBoundaries(Staves stave)
{
	for (int i = buffImage.cols - 1; i >= 0; i -= 1)
	{
		bool res = isStaveOnly(stave, i, 1);
		if (!res)
		{
			for (int j = i - 1; j >= 0; j -= 4)
			{
				res = isStaveOnly(stave, j, 4);
				if (res)
					return j;
			}
		}
	}
	return buffImage.cols - 1;
}

bool StaveBoundaries::isStaveOnly(Staves stave, int xStart, int step)
{
	bool found = false;

	int bCount = 0;
	int start = stave.top;
	int stop = stave.bottom;

	int diff = stop - start;
	start -= diff * 0.5;
	stop += diff * 0.5;

	if (start < 0)
		start = 0;
	if (stop >= buffImage.rows)
		stop = buffImage.rows - 1;

	int j = 0;
	for (j = 0; j < step; j += 1)//计算step范围内的像素和
	{
		if (xStart + j >= buffImage.cols)
			break;
		for (int i = start; i < stop; i += 1)
		{
			uchar *data = buffImage.ptr<uchar>(i);
			int pix = data[xStart+j];
			if (pix != 255)
				bCount += 1;
		}
	}

	bCount /= j ;
	int N1 = staveDetection.staveParams.n1;
	int N2 = staveDetection.staveParams.n2;
	double nAvg = (N1 + N2) / 2;
	nAvg *= 0.65;
	if (bCount <= nAvg * 5  /* # of stavelines */)
		found = true;
	return found;
}
void StaveBoundaries::findMeasures()
{
	vector<Staves>::iterator it = staveDetection.getStaveInfo();

	while (true)
	{
		if (it == staveDetection.staveList.end())
			break;

		Staves stave_up = *it;
		Staves stave_down = *(it+1);
		int end = stave_down.bottom;
		int start = stave_up.top;
		int height = end-start;
		bool found = false;
		int count = 0;
		int remember = 0;
#ifdef _DEBUG
		Mat img = imread("summer.jpg");
		IplImage ori = IplImage(img);
#endif
		for (int i = 0; i < buffImage.cols; i += 1)
		{
			
			int bCount = 0;
			for (int j = start; j < end; j += 1)
			{
				uchar *data = buffImage.ptr<uchar>(j);
				int bPix = data[i];
				if (bPix != 255)
					bCount += 1;
			}
			if (bCount > height *0.8)//* MEASURE_PIXELS)
			{
#ifndef _DEBUG

				CvPoint p = CvPoint(i, start);
				
				Staves s;
				
						CvPoint p1 = CvPoint(i,start);//s.stave_line->at(j).peakPos);
						cvCircle(&ori, p1, 1, CvScalar(255, 0, 0), 3, 8);
						cout << i << "	" << bCount << endl;
				show(&ori);
				//cvSaveImage("measure.jpg", &ori);
#endif
				if (!found)
					remember = i;
				found = true;
				count += 1;
			}
			else
			{
				if (count > 0)
				{
					(*it).addVBar(remember);
					(*(it + 1)).addVBar(remember);
				}
					
				found = false;
				count = 0;
			}
			
		}
		it+=2;
	}
}
void StaveBoundaries::findGroupsOfNotes()
{
	vector<Staves>::iterator it = staveDetection.getStaveInfo();
	int count = 0;
#ifdef _DEBUG
	Mat img = imread("summer.jpg");
	IplImage ori = IplImage(img);
#endif
	//traverse all staves
	while (true)
	{
		if (it == staveDetection.staveList.end())
			break;
		Staves stave = *it;

		count += 1;

		int SPACING_BEFORE = 1;
		int SPACING_AFTER = 1;
	
		for (int k = 0; k < (*it).vBars.size()-1; k++)
		{
			for (int i = (*it).vBars.at(k)+1; i <(*it).vBars.at(k+1); i += SPACING_BEFORE)
			{

				//Do we have a an empty stave spanning accross a distance of SPACING_BEFORE pixels?
				bool res = isStaveOnly(stave, i, SPACING_BEFORE);
#ifndef _DEBUG
				if (res)
				{
					CvPoint p1 = CvPoint(i, (*it).top);
					CvPoint p2 = CvPoint(i, (*it).bottom);
					cvLine(&ori, p1, p2, CvScalar(255, 0, 0), 1, 8);
				}
#endif
				int start = i;
				int end = i;
				if (!res)
				{
					for (int j = i + 1; j < (*it).vBars.at(k + 1); j += /*SPACING_AFTER*/+1)
					{
						//Do we have a an empty stave spanning accross a distance of SPACING_AFTER pixels?
						res = isStaveOnly(stave, j, SPACING_AFTER);
						if (res)
						{
							end = j;
							break;
						}
					}
					i = end;
					//int dAvg = (staveDetection.getStavelineParameters().getD1() + staveDetection.getStavelineParameters().getD2()) / 2;
					if (end - start > 3)//end - start > dAvg)
					{

						(*it).addSymbolPos(start , end/* + 4*/);
#ifndef _DEBUG
						CvPoint p1 = CvPoint(start, (*it).top);
						CvPoint p2 = CvPoint(end, (*it).top);
						cvLine(&ori, p1, p2, CvScalar(255, 0, 0), 3, 8);
#endif
					}
				}

			}
		}

		
		it++;
	}
#ifndef _DEBUG
	show(&ori);
	Mat im = cvarrToMat(&ori);
	imwrite("node.jpg", im);
	//cvSaveImage("result.jpg", &ori);
#endif
}