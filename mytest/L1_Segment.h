#ifndef l1_Segment_HEAD
#define l1_Segment_HEAD
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
using namespace cv;

using namespace std;

namespace omr_engine
{
	class L1_Segment
	{
	public:
		int xStart; // Start and stop coordinates of L1_Segment
		int xStop;
		NoteStem stemInfo;
		/*Mat buffImage;
		Mat dupImage;
		StaveDetection staveDetection;
		Staves stave;

		// Linked list of L2_Segments
		//vector<L2_Segment> l2_Segments;
		L1_Segment();
		~L1_Segment();
		L1_Segment(int xStart, int xStop, NoteStem stemInfo);
		void setParameters(Mat buffImage, Mat dupImage, StaveDetection staveDetection, Staves stave);
		void doL1_Segment();
		void segmentVertically();
		NoteStem getStemInfo();
		vector<L2_Segment> getL2_Segment();
		int getWidth();
		*/
		L1_Segment();
		~L1_Segment();
	};
}
#endif