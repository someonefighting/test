#ifndef PitchCalculation_HEAD
#define PitchCalculation_HEAD
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;
using namespace cv;
namespace omr_engine
{
	class PitchCalculation
	{
	public:
		static const int KEY_TREBLE = 0;
		static const int KEY_BASS = 1;
		static const int KEY_ALTO = 2;

		int note;
		int origNote;
		int duration;
		void setNote(int yPos, int noteDistance, int refPos);
		void setDuration(int duration);
		int calcNotePitch(int origNote);
		int getNote();
		void printNote();
		int getDuration();
		PitchCalculation();
		~PitchCalculation();
	};

}

#endif
