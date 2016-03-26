#ifndef StaveBoundaries_HEAD
#define StaveBoundaries_HEAD

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "NoteHead.h"
#include "StavePeaks.h"
#include"StaveParameters.h"
#include"StaveDetection.h"
#include "Staves.h"
using namespace std;
using namespace cv;
namespace omr_engine
{
	class StaveBoundaries
	{
	public:
		 double MEASURE_PIXELS = 0.9;
		 Mat buffImage;
		 StaveDetection staveDetection;
		 StaveBoundaries();
		 ~StaveBoundaries();
		 StaveBoundaries(Mat buffImage, StaveDetection staveDetection);
		 void findBoundaries();
		 int doLeftBoundaries(Staves stave);
		 int doRightBoundaries(Staves stave);
		 bool isStaveOnly(Staves stave, int xStart, int step);
		 void findMeasures();
		 void findGroupsOfNotes();
	};
}
#endif
