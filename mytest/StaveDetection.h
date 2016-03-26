#ifndef StaveDetection_HEAD
#define StaveDetection_HEAD
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include<vector>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "NoteHead.h"
#include "StavePeaks.h"
#include"StaveParameters.h"
#include"YProjection.h"
#include "Staves.h"
using namespace std;
using namespace cv;


/**
* The <code> StaveDetection </code> class is responsible for detecting all the staves present in a BufferedImage.
* Since the stave detection algorithm needs relies on knowing the stave parameters (n1, n2, d1 and d1) along with
* the Y-Projection of the image, they need to be passed in the constructor.
* <p>
* The <code> StaveDetection </code> class is used as follows:
* <p>
* <code>
* StaveDetection sDetection = new StaveDetection(yProj, sParams); <br>
* sDetection.setParameters(0.80, 0.80); //This is optional as the StaveThreshold and PeakThreshold are both defaulted to 0.75 <br>
* sDetection.locateStaves(); <br>
* sDetection.calcNoteDistance(); <br>
* </code>
*/
namespace omr_engine
{
	class StaveDetection
	{
	public:
		StaveDetection();
		~StaveDetection();
		 double STAVE_THRESHOLD;
		 double PEAK_THRESHOLD;
		 int width;
		 int height;
		 int *bPixels;
		 vector<StavePeaks> localMaxList;
		 StaveParameters staveParams;
		 vector<Staves> staveList;
		 int stavesfound;
		
		 StaveDetection(YProjection yproj, StaveParameters staveParams);
		 void locateStaves();
		 void calcNoteDistance();
		 //iterator begin();
		 vector<Staves>::iterator  getStaveInfo();
		 void setParameters(double STAVE_THRESHOLD, double PEAK_THRESHOLD);
		 void findLocalMaximums();
		 vector<StavePeaks>::iterator StaveDetection::getToSamePosition(vector<StavePeaks>::iterator it1, StavePeaks tester);
		 StavePeaks findNextStave(int min, int max, int val, vector<StavePeaks>::iterator iter);

	};
}

#endif