#include "PitchCalculation.h"

using namespace omr_engine;


PitchCalculation::~PitchCalculation()
{
}
PitchCalculation::PitchCalculation()
{
	note = 0;
	duration = 3;
	//key = KEY_TREBLE;
	//timesig = 0;
}

void PitchCalculation::setNote(int yPos, int noteDistance, int refPos)
{
	origNote = (int)floor((refPos - yPos) / (noteDistance / 2.0));

	//System.out.printf("yPos=%d, refPos=%d, noteDistance=%d, note=%d\n", yPos, refPos, noteDistance, note);
	note = origNote;
	note = calcNotePitch(origNote);


}

void PitchCalculation::setDuration(int dur)
{
	duration = dur;
}

int PitchCalculation::getDuration()
{
	return duration;
}

int PitchCalculation::calcNotePitch(int origNote)
{
	int num = origNote / 7;

	int tempNote = origNote;
	if (tempNote < 0)
	{
		while (true)
		{
			if (tempNote > -7)
				break;
			tempNote += 7;
		}
	}
	else if (tempNote > 6)
	{
		while (true)
		{
			if (tempNote < 7)
				break;
			tempNote -= 7;
		}
	}
	if (tempNote >= 0 && tempNote <= 6)
	{
		switch (tempNote)
		{
		case 0:	return 0 + 7 * num;//0
		case 1:	return 1 + 7 * num;//1
		case 2:	return 3 + 7 * num;//3
		case 3:	return 5 + 7 * num;//5
		case 4:	return 7 + 7 * num;//0
		case 5:	return 8 + 7 * num;//1
		case 6:	return 10 + 7 * num;//3
		}
	}
	else if (tempNote < 0 && tempNote >= -6)
	{
		switch (tempNote)
		{
		case -1:	return -2 + 7 * num;//5
		case -2:	return -4 + 7 * num;//3
		case -3:	return -6 + 7 * num;//1
		case -4:	return -8 + 7 * num;//6
		case -5:	return -9 + 7 * num;//5
		case -6:	return -11 + 7 * num;//3
		}
	}
	return -1;
}

int PitchCalculation::getNote()
{
	return note;
}

void PitchCalculation::printNote()
{
	int tempNote = origNote;
	if (tempNote < 0)
	{
		while (true)
		{
			if (tempNote > 0)
				break;
			tempNote += 7;
		}
	}
	if (tempNote % 7 == 0)
		puts("E ");
	else if (tempNote % 7 == 1)//E F G A B C D 
		puts("F ");            //0 1 2 3 4 5 6
	else if (tempNote % 7 == 2)//3 4 5 6 0 1 2(MUSIC)
		puts("G ");
	else if (tempNote % 7 == 3)
		puts("A ");
	else if (tempNote % 7 == 4)
		puts("B ");
	else if (tempNote % 7 == 5)
		puts("C ");
	else if (tempNote % 7 == 6)
		puts("D ");

	printf("- Duration %d" ,duration);
}

