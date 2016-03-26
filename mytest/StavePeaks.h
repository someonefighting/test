#ifndef StavePeaks_HEAD
#define StavePeaks_HEAD
#include<stdio.h>
#include"show.h"
namespace omr_engine
{
	class StavePeaks
	{
	public:
		int peakPos;
		int peakValue;
		int start;
		int end;
		bool available;
		StavePeaks(int pos, int value);
		void SetStart(int start);
		void SetEnd(int end);
		void setStartEnd(int start, int end);
		bool equals(StavePeaks x);

		StavePeaks();
		~StavePeaks();
	};

}

#endif