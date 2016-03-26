#include "StavePeaks.h"
using namespace omr_engine;

StavePeaks::StavePeaks()
{
	available = false;
}


StavePeaks::~StavePeaks()
{
}
StavePeaks::StavePeaks(int pos, int value)
{
	available = true;
	peakPos = pos;
	peakValue = value;
}

void StavePeaks::SetStart(int s)
{
	start = s;
}

void StavePeaks::SetEnd(int e)
{
	end = e;
}

void StavePeaks::setStartEnd(int start, int end)
{
	start = start;
	end = end;
}

// method to test whether two objects are equal
bool StavePeaks::equals(StavePeaks x)
{
	if (x.peakPos == peakPos && x.peakValue == peakValue && x.start == start && x.end == end)
		return true;
	return false;
}