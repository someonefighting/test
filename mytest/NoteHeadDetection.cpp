#include "NoteHeadDetection.h"
using namespace omr_engine;
NoteHeadDetection::NoteHeadDetection()
{

}


NoteHeadDetection::~NoteHeadDetection()
{
}
NoteHeadDetection::NoteHeadDetection(Mat buff, StaveParameters stavePa)
{
	buffImage = buff;
	staveParams = stavePa;
	yProj = YProjection(buff);
}
vector<NoteHead> NoteHeadDetection::findNotes(Mat img,int startH, int endH, int startW, int endW)
{
	height = endH - startH;
	width = endW - startW;

	xProjection = new int[width];
	xProjectionBeforeFilter = new int[width];
	yPos = new int[width];
	yPosBeforeFilter = new int[width];

	vector<NoteHead> coordList;


	//BEGIN --> better note detection, but not yet in use
	for (int i = startW; i < endW; i += 1)
	{
		Xprojection(startH, endH, i, startW);
	}
#ifndef _DEBUG
	cout << "old" << endl;
	for (int i = 0; i < width; i += 1)
		cout << xProjection[i] << endl;
#endif
	for (int i = 0; i < width; i += 1)
		xProjectionBeforeFilter[i] = xProjection[i];

	filterXproj();
#ifndef _DEBUG
	cout << "xproj" << endl;
	for (int i = 0; i < width; i += 1)
		cout << xProjection[i]<<endl;
#endif
	filterMeanXProjection();

	//END

	for (int i = startW; i < endW; i += 1)
	{
		
			YProjRLE(startH, endH, i, startW);
#ifndef _DEBUG
		cout << yPos[i - startW] << endl;
#endif
	}

	//printYPos(width, 0);

	for (int i = 0; i < width; i += 1)
		yPosBeforeFilter[i] = yPos[i];

	//printYPos(width, 0);

	
#ifndef _DEBUG
	cout << "new" << endl;
	for (int i = startW; i < endW; i += 1)
	{		
		cout << yPos[i - startW] << endl;
	}
#endif
	//filterYpos(startW, startH);
	//printYPos(width, 0);

#ifndef _DEBUG
	Mat img = imread("summer.jpg");
	IplImage ori = IplImage(img);
#endif
	for (int i = 0; i < width; i += 1)
	{
		bool start = true;
		int xTempPos = 0;
		for (int j = i; j < width; j += 1)
		{
			if (yPos[j] != 0)
			{
				if (start)
					xTempPos = j;
				start = false;
			}
			if (yPos[j] == 0)
			{
				if (!start)
				{
					NoteHead temp = NoteHead();
					temp.x = startW + xTempPos;
					temp.y = startH + findyPos(xTempPos);
					//System.out.println("Found note @ x: " + xTempPos + " y: " + temp.y + " ::: " + findyPos(xTempPos));
#ifdef _DEBUG
					CvPoint p1 = CvPoint(temp.x, temp.y);

					circle(img, p1, 1, Scalar(255, 0, 0), 2, 8);
					show(&img);
#endif
					//Locate position of stem (left or right)
					temp.stemInfo = locateStem(temp.x, startH, endH);

					//Add note to linked list

					coordList.push_back(temp);
					i = j;
					break;
				}
				start = true;
			}
		}
	}
	return coordList;
}

int NoteHeadDetection::locateStartNote()
{
	return 0;
}
NoteStem NoteHeadDetection::locateStem(int x, int startH, int endH)
{
	int xStart = x - 14;
	int xEnd = staveParams.d1 * 2 + xStart;

	XProjection xProj = XProjection(buffImage);
	xProj.calcXProjection(startH, endH, xStart, xEnd);//col pixels sum

	int *xProjArray = xProj.getXProjection();
	int max = xProjArray[0];
	int pos = 0;
	for (int i = 1; i<xEnd - xStart; i += 1)
	{
		if (xProjArray[i] > max)
		{
			max = xProjArray[i];
			pos = i;
		}
	}

	NoteStem stem = NoteStem();
	stem.stemPosition = pos + xStart;

	//System.out.println("xStart: " + xStart + " xEnd: " + xEnd + " Max: " + max + " Pos: " + pos);

	//stem is to the right
	if (pos + xStart > x)
	{
		stem.stemDirection = 1;
		return stem;
	}
	//else stem is to the left
	else
	{
		stem.stemDirection = 0;
		return stem;
	}
}
void NoteHeadDetection::Xprojection(int startH, int endH, int i, int startW)
{
	int N2 = staveParams.n2;
	int D1 = staveParams.d1;
	int D2 = staveParams.d2;

	yProj.calcYProjection(startH, endH, i, i + 1);
	yProjection = yProj.getYProjection();

	bool start = false;
	int counter = 0;
	int max = 0;
	int tempPos = 0;
#ifdef _DEBUG
	Mat img = imread("summer.jpg");
	IplImage ori = IplImage(img);
#endif
	for (int j = 0; j < height; j += 1)
	{
		if (yProjection[j] == 1)
		{
			if (!start)
			{
				tempPos = j;
			}
			start = true;
			counter += 1;
		}
		else
		{
			if (start)
			{
				if (counter > max)
				{
					max = counter;
				}
			}
			counter = 0;
			start = false;
		}
	}
	
	if (max >= D1 && max <=  N2 + D1) // && max < 4*D2 ) // + 2 * N2) // && max < 2 * N2 + D1)//Ñ°ÕÒ·ûÍ·
	{
#ifndef _DEBUG
		CvPoint p1 = CvPoint(i, startH);
		circle(img, p1, 1, Scalar(255, 0, 0), 2, 8);
		show(&img);
#endif
		xProjection[i - startW] = max;
		//cout << xProjection[i - startW] << endl;
		return;
	}
	xProjection[i - startW] = 0;
	//cout << xProjection[i - startW] << endl;
	return;
}
void NoteHeadDetection::YProjRLE(int startH, int endH, int i, int startW)
{
	int D1 = staveParams.d1;
	int D2 = staveParams.d2;
	int N2 = staveParams.n2;
	int N1 = staveParams.n1;
	yProj.calcYProjection(startH, endH, i, i + 1);
	yProjection = yProj.getYProjection();

	bool start = false;
	int counter = 0;
	int max = 0;
	int tempPos = 0;
	int tempMaxPos = 0;

	// filter yProjection. Get rid of values greater than D2
	filteryProj();

	for (int j = 0; j < height; j += 1)
	{
		if (yProjection[j] == 1)
		{
			if (!start)
			{
				tempPos = j;
			}
			start = true;
			counter += 1;
		}
		else
		{
			if (start)
			{
				if (counter > max)
				{
					max = counter;
					tempMaxPos = tempPos;

				}
			}
			counter = 0;
			start = false;
		}
	}

	if (max > 2 * N1)// && max < D2 + 2 * N2) // && max < 2 * N2 + D1)
	{
		// xProjection[i - startW] = max;
		if (max >= D1 && max < D2 + 2 * N2) // This will get rid of two
			// notes directly above one
			// another
			yPos[i - startW] = tempMaxPos;
		else
			yPos[i - startW] = 0;
		return;
	}
	// xProjection[i - startW] = 0;
	yPos[i - startW] = 0;
	return;
}
void NoteHeadDetection::filteryProj()
{
	bool start = false;
	int N2 = staveParams.n2;
	int D2 = staveParams.d2;
	int counter = 0;
	int tempPos = 0;
	for (int j = 0; j < height; j += 1)
	{
		if (yProjection[j] == 1)
		{
			if (!start)
			{
				tempPos = j;
			}
			start = true;
			counter += 1;
		}
		else
		{
			if (start)
			{
				if (counter > D2 + 2 * N2)
				{
					for (int i = tempPos; i < j; i += 1)
						yProjection[i] = 0;
				}
			}
			counter = 0;
			start = false;
		}
	}
}
int NoteHeadDetection::findyPos(int tempPos)
{
	int N2 = staveParams.n2;
	if (tempPos + N2 >= width)
		return yPos[tempPos];
	if (yPos[tempPos + N2] != 0)
		return yPos[tempPos + N2];
	for (int i = tempPos; i >= tempPos - 2 * N2; i -= 1)
	{
		if (i < 0)
			break;
		if (yPos[i] != 0)
			if (yPos[i + N2] != 0)
				return yPos[i + N2];
	}
	for (int i = tempPos; i < tempPos + 2 * N2; i += 1)
	{
		if (i >= width || i + N2 >= width)
			break;
		if (yPos[i] != 0)
			if (yPos[i + N2] != 0)
				return yPos[i + N2]; // make sure we get a peak value
	}

	for (int i = tempPos; i >= tempPos - 2 * N2; i -= 1)
		if (yPos[i] != 0)
			return yPos[i];
	for (int i = tempPos; i < tempPos + 2 * N2; i += 1)
		if (yPos[i] != 0)
			return yPos[i];
	return 0;
}
void NoteHeadDetection::filterXproj()
{
	int N2 = staveParams.n2;
	int D2 = staveParams.d2;
	int D1 = staveParams.d1;

	// first find all local maximas. Then find all peaks in array
	for (int i = 1; i < width - 1; i += 1)
	{
		int forward = 0;
		int backward = 0;
		if (xProjection[i - 1] < xProjection[i] && xProjection[i] > xProjection[i + 1])
		{
			int val = (int)(xProjection[i] * 0.5);

			// search backwards
			for (int j = i - 1; j >= 0; j -= 1)
			{
				if (xProjection[j] < val)
				{
					backward = j;
					break;
				}
			}

			// search forwards
			for (int j = i + 1; j <= width - 1; j += 1)
			{
				if (xProjection[j] < val)
				{
					forward = j-1;
					break;
				}
			}

			int distance = forward - backward;
			if (distance < D1) // Flatten
				// the graph
				// if
				// distance
				// < 3/2 N2
			{
				// System.out.println("Flatten...");
				for (int j = i; j >= backward; j -= 1)
					xProjection[j] = 0;
				for (int j = i + 1; j <= forward; j += 1)
					xProjection[j] = 0;
			}
		}

	}
}
void NoteHeadDetection::filterMeanXProjection()
{
	static const double UPPER_BOUND = 1.2;
	static const double LOWER_BOUND = 0.8;
	int start = 0;
	int stop = 0;
	//System.out.println("Width: " + width);
	for (int i = 0; i < width-1; i += 1)
	{
		if (xProjection[i] != 0)
		{
			start = i;
			for (int j = i + 1; j < width; j += 1)
			{
				if (xProjection[j] == 0)
				{
					stop = j;
					break;
				}
			}
			if (stop < start)
				stop = width - 1;
			// find average values
			//System.out.println("Start: " + start + "  stop: " + stop);
			float average = 0;
			for (int j = start; j < stop; j += 1)
				average += xProjection[j];
			average = average / (float)(stop - start);
			int upperB = (int)round(average * UPPER_BOUND);
			int lowerB = (int)round(average * LOWER_BOUND);
			//System.out.println("average: " + average);
			//System.out.println("Upper: " + upperB + "  lower: " + lowerB);
			for (int j = start; j < stop; j += 1)
				if (xProjection[j] <= lowerB || xProjection[j] >= upperB)
					xProjection[j] = 0;

			i = stop - 1;
		}
	}
#ifndef _DEBUG
	cout << "mean" << endl;
	for (int i = 0; i < width; i += 1)
		cout << xProjection[i] << endl;
#endif
	for (int i = 0; i < width-1; i += 1)
	{
		if (xProjection[i] != 0)
		{
			start = i;
			for (int j = i + 1; j < width; j += 1)
			{
				if (xProjection[j] == 0)
				{
					stop = j;
					break;
				}
			}
			if (stop < start)
				break;
			if ((stop - start) < staveParams.d1*0.3)
			{
				for (int j = start; j < stop; j += 1)
					xProjection[j] = 0;
			}
			i = stop - 1;
		}
	}
#ifndef _DEBUG
	cout << "final" << endl;
	for (int i = 0; i < width; i += 1)
		cout << xProjection[i] << endl;
#endif
}
void NoteHeadDetection::filterYpos(int startW, int startH)
{
	int N2 = staveParams.n2;
	int D2 = staveParams.d2;
	int D1 = staveParams.d1;
	// first find all local maximas. Then find all peaks in array
	for (int i = 1; i < width - 1; i += 1)
	{
		int forward = 0;
		int backward = 0;
		if (yPos[i - 1] < yPos[i] && yPos[i] >= yPos[i + 1])
		{
			int val = (int)(yPos[i] * 0.5);

			// search backwards
			for (int j = i - 1; j >= 0; j -= 1)
			{
				if (yPos[j] < val)
				{
					backward = j;
					break;
				}
			}

			// search forwards
			for (int j = i + 1; j < width; j += 1)
			{
				if (yPos[j] < val)
				{
					forward = j - 1;
					break;
				}
			}

			int distance = forward - backward;
			if (/*(3 * N2) / 2 > distance ||*/ distance <D1 )// D2 / 2) // Flatten
				// the graph
				// if
				// distance
				// < 3/2 N2
			{
				// System.out.println("Flatten...");
				for (int j = i; j >= backward; j -= 1)
					yPos[j] = 0;
				for (int j = i + 1; j < forward; j += 1)
					yPos[j] = 0;
			}
		}

	}
}

int* NoteHeadDetection::getYPos()
{
	return yPos;
}
int* NoteHeadDetection::getYposBeforeFilter()
{
	return yPosBeforeFilter;
}
int* NoteHeadDetection::getXProjection()
{
	return xProjection;
}
int* NoteHeadDetection::getXProjectionBeforeFilter()
{
	return xProjectionBeforeFilter;
}
void NoteHeadDetection::printYPos(int width, int startH)
{
	printf("START YPOS -- Height = ", height);
	for (int i = 0; i < width; i += 1)
	{
		if (yPos[i] > 0)
			printf("%d", height - yPos[i]);
		else
			printf("0");
	}
	printf("END YPOS");
}
void NoteHeadDetection::printXProj(int width)
{
	puts("START XProj");
	for (int i = 0; i < width; i += 1)
		printf("%d\n", xProjection[i]);
	puts("END XProj");
}