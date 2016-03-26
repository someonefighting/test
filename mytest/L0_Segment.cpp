#include "L0_Segment.h"
using namespace omr_engine;

L0_Segment::L0_Segment()
{
	//hasNote = false;
}


L0_Segment::~L0_Segment()
{
}/*
void L0_Segment::setParameters(Mat buff, Mat dup, StaveDetection staveDet, Staves s)
{
	buffImage = buff;
	dupImage = dup;
	staveDetection = StaveDetection(staveDet);
	stave = Staves( s);
}/*
void L0_Segment::processL0_Segment()
{
	//Locate all notehead in segment (if any are present)
	findNoteHeads();

	//if this segment has a note, we need to further segment it
	if (hasNote)
	{
		puts("***** L0_Segment *****");
		for (int i = 0; i<noteCoordList.size(); i += 1)
		{
			NoteHead noteHeadTemp = noteCoordList.at(i);

			//System.out.printf("Stem position: %d\n", noteHeadTemp.stemInfo.stemDirection);

			L1_Segment l1_temp = L1_Segment(noteHeadTemp.x - staveDetection.staveParams.n2, noteHeadTemp.x + staveDetection.staveParams.d2, noteHeadTemp.stemInfo);
			l1_temp.setParameters(buffImage, dupImage, staveDetection, stave);
			l1_temp.doL1_Segment();
			l1_segmentList.push_back(l1_temp);

			if (noteHeadTemp.stemInfo.stemDirection == 1)  //stem is on right
			{
				if (i < noteCoordList.size() - 1)
				{
					int xStart = noteHeadTemp.stemInfo.stemPosition;
					int xStop = noteCoordList.at(i + 1).x - 9;  //9 is an arbritrary #. Need to find a pattern

					if (xStop - xStart > 14) //we don't want to process very small sgements
					{
						L1_Segment l1_temp2(xStart, xStop, NoteStem());
						l1_temp2.setParameters(buffImage, dupImage, staveDetection, stave);
						l1_temp2.doL1_Segment();
						l1_segmentList.push_back(l1_temp2);
					}
				}
				else	//There are no more stems in L0_Segment
				{
					int xStart = noteHeadTemp.stemInfo.stemPosition;
					int xStop = stop;

					if (xStop - xStart > 14) //we don't want to process very small sgements
					{
						L1_Segment l1_temp2 = L1_Segment(xStart, xStop, NoteStem());
						l1_temp2.setParameters(buffImage, dupImage, staveDetection, stave);
						l1_temp2.doL1_Segment();
						l1_segmentList.push_back(l1_temp2);
					}
				}

			}

			else	//stem is on the left
			{
				//System.out.println("Stem is on the right... Still needs to be implemented");
				if (i < noteCoordList.size() - 1)
				{

				}
			}
		}
	}
	else
	{
		segmentNonNoteSymbols();
	}
}
void L0_Segment::findNoteHeads()
{
	//DrawingTools.drawBox(dupImage, stave.getStart(), stave.getEnd(), start, stop, Color.GREEN);

	//Locate all noteheads
	NoteHeadDetection noteHeadDetection = NoteHeadDetection(buffImage, staveDetection.staveParams);
	noteCoordList = noteHeadDetection.findNotes(stave.start, stave.end, start, stop);

	//Traverse linked list containing noteHeads and draw them
	vector<NoteHead>::iterator posIterator = noteCoordList.begin();
	while (posIterator!=noteCoordList.end())
	{
		NoteHead pos = *posIterator;
		addNote(pos.y + 8, stave.noteDistance, stave.getStave_line(4).end);

		// draw note
		//DrawingTools.drawNote(dupImage, pos.x, 20, pos.y, staveDetection.getStavelineParameters().getD2());
	}

}
void L0_Segment::segmentNonNoteSymbols()
{
	YProjection yProj =  YProjection(buffImage);
	yProj.calcYProjection(stave.start, stave.end, start, stop);

	int *yProjection = yProj.getYProjection();

	bool start = false;
	int count = 0;
	int startPos = 0;
	// System.out.println("Symbol #" + i);

	for (int j = 0; j < stave.end - stave.start; j += 1)
	{
		if (yProjection[j] != 0)
		{
			start = true;

			if (count == 0)
				startPos = j;

			count += 1;
		}
		else
		{
			if (start && count > staveDetection.staveParams.n2)
			{
				//DrawingTools.drawBox(dupImage, stave.getStart() + startPos, stave.getStart() + j, this.start, this.stop, Color.MAGENTA);
			}
			start = false;
			count = 0;
		}
	}
}
/*
void L0_Segment::addNote(int yPos, int noteDistance, int refPos)
{
	//Cannot calculate pitch here as we need more info about note segments...
	PitchCalculation temp = PitchCalculation();
	temp.setNote(yPos, noteDistance, refPos);
	noteList.push_back(temp);
	hasNote = true;
}
void L0_Segment::calculateNoteDuration()
{
	int notesProcessed = 0;
	for (int i = 0; i<l1_segmentList.size(); i += 1)
	{
		L1_Segment l1_temp = l1_segmentList.at(i);
		if (l1_temp.getStemInfo().stemDirection == 1 && i < l1_segmentList.size() - 1) //Stem to the right
		{
			//Therefore we need next L1_Segment to determine duration of note
			l1_temp = l1_segmentList.at(i + 1);
			if (l1_temp.getWidth() > 4)
			{
				vector<L2_Segment> l2_temp = l1_temp.getL2_Segment();
				for (int j = 0; j<l2_temp.size(); j += 1)
				{
					if (l2_temp.at(j).getSymbolName()=="quaver_line" || l2_temp.at(j).getSymbolName()==("quaver"))
					{
						if (l2_temp.at(j).getAccuracy() > 0.2)
						{
							noteList.at(notesProcessed).setDuration(2);
							break;
						}
					}
					else if (l2_temp.at(j).getSymbolName()==("semiquaver_line") || l2_temp.at(j).getSymbolName()==("semiquaver"))
					{
						if (l2_temp.at(j).getAccuracy() > 0.2)
						{
							noteList.at(notesProcessed).setDuration(1);
							break;
						}
					}
				}
				notesProcessed += 1;
			}
		}
		else if (l1_temp.getStemInfo().stemDirection == 1 && l1_temp.getWidth() > 4)
		{
			l1_temp = l1_segmentList.at(i);
			vector<L2_Segment> l2_temp = l1_temp.getL2_Segment();
			for (int j = 0; j<l2_temp.size(); j += 1)
			{
				if (l2_temp.at(j).getSymbolName()==("quaver_line") || l2_temp.at(j).getSymbolName()==("quaver"))
				{
					if (l2_temp.at(j).getAccuracy() > 0.2)
					{
						noteList.at(notesProcessed).setDuration(2);
						break;
					}
				}
				else if (l2_temp.at(j).getSymbolName()==("semiquaver_line") || l2_temp.at(j).getSymbolName()==("semiquaver"))
				{
					if (l2_temp.at(j).getAccuracy() > 0.2)
					{
						noteList.at(notesProcessed).setDuration(1);
						break;
					}
				}
			}
			notesProcessed += 1;
		}

	}
}
vector<PitchCalculation>  L0_Segment::getNotes()
{
	return noteList;
}*/