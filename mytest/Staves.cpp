#include "Staves.h"

using namespace omr_engine;
Staves::Staves()
{
	available = false;
}


Staves::~Staves()
{
}
Staves::Staves(int Number)
{
	available = true;
	stave_line = new StavePeaks[5];
	staveNumber = Number;
}
void Staves::addStaveline(int pos, StavePeaks staveLine)
{
	stave_line[pos] = staveLine;
	if (pos == 0)
		top = staveLine.start - 5;
	if (pos == 4)
		bottom = staveLine.end + 5;
}
void Staves::printStavesInfo()
{
	printf("Stave # %d \n" , staveNumber);

	for (int i = 0; i < 5; i += 1)
	{
		printf("Staveline # %dStartPos:%d EndPos:%d\n" ,i , stave_line[i].start, stave_line[i].end);
	}
}
StavePeaks Staves::getStave_line(int number)
{
	return stave_line[number];
}
void Staves::setStave_line(int number, StavePeaks stavePeak)
{
	stave_line[number] = stavePeak;
}
void Staves::addVBar(int xPos)
{
	vBars.push_back(xPos);
}

void Staves::addSymbolPos(int start, int stop)
{
	L0_Segment temp;
	temp.start = start;
	temp.stop = stop;
	symbolPos.push_back(temp);
}
void Staves::saveStaveInfo()
{
	printf("%d\n",staveNumber);
	for (int i = 0; i < 5; i += 1)
	{
		printf("%d\n", stave_line[i].peakPos);
		printf("%d\n",stave_line[i].peakValue);
		printf("%d\n",stave_line[i].start);
		printf("%d\n",stave_line[i].end);
	}
}
