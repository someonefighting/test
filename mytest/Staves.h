#ifndef Stave_HEAD 
#define Stave_HEAD 

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "YProjection.h"
#include"StavePeaks.h"
#include"L0_Segment.h"
using namespace std;
using namespace cv;


namespace omr_engine
{
	class Staves
	{
	public:
		StavePeaks *stave_line;
		vector<int> vBars;
		vector<L0_Segment> symbolPos;
		int top;
		int bottom;
		int staveNumber;
		int left;
		int right;
		int noteDistance;
		int start;	//where we start to look for notes VERTICALLY
		int end; //where we stop looking for notes VERTICALLY
		bool available;
		Staves();
		~Staves();
		Staves(int staveNumber);
		void addStaveline(int pos, StavePeaks staveLine);
		void printStavesInfo();
		StavePeaks getStave_line(int number);
		void setStave_line(int number, StavePeaks stavePeak);
		void addVBar(int xPos);
		void addSymbolPos(int start, int stop);
		void saveStaveInfo();

	};

}

#endif