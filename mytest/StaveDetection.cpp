#include "StaveDetection.h"

using namespace omr_engine;

StaveDetection::StaveDetection()
{
}


StaveDetection::~StaveDetection()
{
}
StaveDetection::StaveDetection(YProjection yproj, StaveParameters stavePara)
{
	bPixels = yproj.getYProjection();
	height = yproj.row;
	width = yproj.buffImage.cols;
	staveParams = stavePara;	
	stavesfound = 0;
	STAVE_THRESHOLD = 0.9;
	PEAK_THRESHOLD = 0.9;

}
/*vector<Staves>::iterator  StaveDetection::getStaveInfo()
{
	return staveList.begin();
}*/
/**
* This method will locate all the staves present in an image. <p>
*
*It does so by looking for five equidistant peaks in the Y-Projection. In order for it to detect a stave,
*the spacing between the peaks must be in the range d1 to d2 and the peaks must be in the range n1 to n2.
*/
void StaveDetection::locateStaves()
{
	findLocalMaximums();
	stavesfound = 0;
	// process each local maxima one at a time
	vector<StavePeaks>::iterator iter = localMaxList.begin();
	while (true)
	{
		if (iter == localMaxList.end())
		{
			break;
		}
		//StavePeaks temp = (*iter);
		// Percentage of local maximum used as threshold to search for other
		// neighbouring peaks
		int val = (int)((*iter).peakValue * STAVE_THRESHOLD);

		// search backwards bPixel array for values >= to threshold value
		int count = 1;
		for (int i = (*iter).peakPos - 1; i >= 0; i -= 1)
		{
			if (bPixels[i] < val || count >= staveParams.n2 / 2)
			{
				(*iter).SetStart((*iter).peakPos - count + 1);
				break;
			}
			count += 1;
		}

		// search forwards bPixel array for values >= to threshold value
		int count2 = 1;
		for (int i = (*iter).peakPos + 1; i < height; i += 1)
		{
			if (bPixels[i] < val || count2 >= staveParams.n2 / 2)
			{
				(*iter).SetEnd((*iter).peakPos + count2 - 1);
				break;
			}
			count2 += 1;
		}

		// calculate the width of potential staveline
		count += count2 - 1;
		if (count > staveParams.n2 || count < staveParams.n1)
		{
			// remove any items that are thicker or narrower than staveline
			// parameters N1 and N2
			iter = localMaxList.erase(iter);
			continue;
		}
		iter++;
	}
	/*
#ifdef _DEBUG
	Mat img = imread("summer.jpg");
	IplImage ori = IplImage(img);
	for (int i = 0; i < localMaxList.size(); i++)
	{
		CvPoint p1 = CvPoint(50,localMaxList.at(i).peakPos);
		CvPoint p2 = CvPoint(400, localMaxList.at(i).peakPos);
		cvLine(&ori, p1, p2, CvScalar(255, 0, 0), 1, 8);
	}
	show(&ori);
#endif*/
	// We now want to locate all the staves by processing each item in
	// linked list
	iter = localMaxList.begin();
	bool found = false;
	StavePeaks first ;
	StavePeaks nextPeak ;
	int min = 0;
	int max = 0;
	int linecount = 1;
	Staves temp = Staves(stavesfound);
	bool mark = true;
	while (true)
	{
		
		if (iter == localMaxList.end())
		{
			break;
		}
		
		if (!found)
		{
			// We haven't found a staveline, get next item in list to
			// process
		/*	if (mark != true)
				iter++;
			else
				mark = false;
		*/
			first = *iter;
			
			// min and max values to determine acceptable distance between
			// end of stave and beginning of next stave
			min = first.end + staveParams.d1;
			max = first.end + staveParams.d2;

			temp =Staves(stavesfound);
			temp.addStaveline(0, first);
		}
		else
		{
			// We could have a stave, find the next staveline
			min = nextPeak.end + staveParams.d1;
			max = nextPeak.end + staveParams.d2;
			first = nextPeak;
		}

		// Use a second iterator and get it to same position as first
		vector<StavePeaks> ::iterator iter2 = localMaxList.begin();
		iter2 = getToSamePosition(iter2, first);
	
		nextPeak = findNextStave(min, max, first.peakValue, iter2);
		if (nextPeak.available == false)
		{
			// Current item in list has been determined not to be part of a
			// stave
			// Reset found to false and linecount to 1
			found = false;
			iter++;
			linecount = 1;
		}
		else
		{
			// Current item could be part of a stave
			linecount += 1;
			if (linecount < 6)
				temp.addStaveline(linecount - 1, nextPeak);
			found = true;
			// When linecount == 5, we have found a stave
			if (linecount == 5)
			{
#ifdef _DEBUG
				cout << linecount << endl;
				for (int i = 0; i < 5; i++)
				{
					
					cout << temp.stave_line[i].peakPos <<"	"<< temp.stave_line[i].peakValue << "	";
				}
				cout << endl;
#endif
				stavesfound += 1;

				// advance iter to nextPeak
				iter = getToSamePosition(iter, nextPeak);
				found = false;
				iter++;
				linecount = 1;
				// add stave to linked list
				staveList.push_back(temp);
#ifndef _DEBUG
				Mat img = imread("summer.jpg");
				IplImage ori = IplImage(img);
				Staves s;
				
					for (int j = 0; j < 5; j++)
					{
						CvPoint p1 = CvPoint(50, temp.stave_line[j].peakPos);//s.stave_line->at(j).peakPos);
						CvPoint p2 = CvPoint(400, temp.stave_line[j].peakPos);
						cvLine(&ori, p1, p2, CvScalar(255, 0, 0), 1, 8);
					}

				
				show(&ori);
#endif
				//print location of bottom staveline
				//System.out.println("Bottom stave: " + temp.getStave(4).getEnd());
			}
			
		}
		
	}
#ifndef _DEBUG
	Mat img = imread("summer.jpg");
	IplImage ori = IplImage(img);
	Staves s;
	for (int i = 0; i < staveList.size(); i++)
	{
		s = staveList.at(i);
		for (int j = 0; j < 5; j++)
		{
			CvPoint p1 = CvPoint(50, s.stave_line[j].peakPos);//s.stave_line->at(j).peakPos);
			CvPoint p2 = CvPoint(400, s.stave_line[j].peakPos);
			cvLine(&ori, p1, p2, CvScalar(255, 0, 0), 1, 8);
		}
	
	}
	show(&ori);
	cvSaveImage("staveline.jpg", &ori);
#endif
}
void StaveDetection::calcNoteDistance()
{
	for (int i = 0; i<staveList.size(); i += 1)
	{
		int dist = 0;
		Staves temp = staveList.at(i);
		for (int j = 0; j<4; j += 1)
		{
			int avg = (temp.getStave_line(j).start + temp.getStave_line(j).end) / 2;
			int avg2 = (temp.getStave_line(j + 1).start + temp.getStave_line(j + 1).end) / 2;
			dist += avg2 - avg;
		}
		dist /= 4;
		staveList.at(i).noteDistance = dist;
	}
}
/*vector<Staves>::iterator StaveDetection::getStaveInfo()
{
	return staveList.begin();
}*/
void StaveDetection::setParameters(double STAVE, double PEAK)
{
	STAVE_THRESHOLD = STAVE;
	PEAK_THRESHOLD = PEAK;
}
void StaveDetection::findLocalMaximums()
{
	for (int i = 1; i < height - 1; i += 1)
	{
		int pixA = bPixels[i - 1];
		int pixB = bPixels[i + 1];
		int pix = bPixels[i];
		if (pix >= pixA && pix > pixB && pix > (double)2/3*width)
		{
			StavePeaks temp = StavePeaks(i, pix);//pos value
			localMaxList.push_back(temp);
		}
	}
}
vector<StavePeaks>::iterator StaveDetection::getToSamePosition(vector<StavePeaks>::iterator it1, StavePeaks tester)
{
	while (true)
	{
		if (it1==localMaxList.end())
			return it1;
		StavePeaks t = *it1;
		if (t.equals(tester))
			break;
		else
			it1++;
	}
	return it1;
}
StavePeaks StaveDetection::findNextStave(int min, int max, int val, vector<StavePeaks>::iterator iter)
{
	StavePeaks result;
	if (iter == localMaxList.end())
		return result;
	StavePeaks next =*iter;

	while (true)
	{
		

		// If starting position of next item in list > max, then we can't
		// consider that item to be part of stave
		if (next.start > max)
			return result;

		// If start is in between min and max, we can consider that item
		else if (next.start >= min && next.start <= max)
		{
			// consider this stave only if previous staveline maximum is >=
			// than 2/3 of current maximum
			// 2/3 seems to be a value that works for all test cases
			int val1 = (int)(next.peakValue * PEAK_THRESHOLD);
			if (val >= val1)
				return next;
		}

		if (iter == localMaxList.end())
			break;
		next =*iter++;
	}

	// Found nothing, return null
	return result;
}
vector<Staves>::iterator  StaveDetection::getStaveInfo()
{
	return staveList.begin();
}