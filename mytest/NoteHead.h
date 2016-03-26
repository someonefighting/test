#ifndef NoteHead_HEAD 
#define NoteHead_HEAD 

#include <stdio.h>
#include <iostream>
#include "NoteStem.h"
using namespace std;

namespace omr_engine
{
	class NoteHead
	{
	public:
		int x;
		//public int xStart;
		//public int xEnd;
		int y;
		NoteStem stemInfo;	//indicates if stem is on left or right
		NoteHead();
		~NoteHead();
	};
}
#endif