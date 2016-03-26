#ifndef L0_Segment_HEAD
#define L0_Segment_HEAD
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "NoteHead.h"
#include"PitchCalculation.h"
//#include"Staves.h"
//#include"L1_Segment.h"
//#include"NoteHeadDetection.h"
using namespace cv;

using namespace std;


namespace omr_engine
{
	//StaveDetection staveDetection;
	class L0_Segment
	{
	public:
		int start;	//Start and stop coordinates of L0_Segment
		int stop;
		
		bool hasNote;
		//List of all noteheads present in L0_Segment
		//vector<NoteHead> noteCoordList;

	//	Mat buffImage;
	//	Mat dupImage;
		//StaveDetection staveDetection;
	//	Staves stave;
		//Consider moving this somewhere else
		vector<PitchCalculation> noteList;
		//List of all L1_Segments	
		//vector<L1_Segment> l1_segmentList;
		/*
		void setParameters(Mat buffImage, Mat dupImage, StaveDetection staveDetection, Staves stave);
		void processL0_Segment();
		void findNoteHeads();
		void segmentNonNoteSymbols();
		void addNote(int yPos, int noteDistance, int refPos);
		void calculateNoteDuration();
		vector<PitchCalculation> getNotes();
	*/
		L0_Segment();
		~L0_Segment();
	};
}
#endif
