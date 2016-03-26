#include "L1_Segment.h"
using namespace omr_engine;

L1_Segment::L1_Segment()
{
	
}
L1_Segment::~L1_Segment()
{

}/*
L1_Segment::L1_Segment(int Start, int Stop, NoteStem stem)
{
	xStart = Start;
	xStop = Stop;
	stemInfo = stem;
}
void L1_Segment::setParameters(Mat buff, Mat dup, StaveDetection staveDetec, Staves s)
{
	buffImage = buff;
	dupImage = dup;
	staveDetection = staveDetec;
	stave = s;
}
void L1_Segment::doL1_Segment()
{
	
	//System.out.printf("L1_Segment: xStart=%d and xStop=%d\n", xStart, xStop);

	if (stemInfo.stemDirection != -1)
	{
		segmentVertically();
	}
	else
	{
		//DrawingTools.drawBox(dupImage, stave.getStart(), stave.getEnd(), xStart, xStop, Color.PINK);
		segmentVertically();
	}
}
void L1_Segment::segmentVertically()
{
	String stem ;
	if (stemInfo.stemDirection == 0)
		stem =  String("Left");
	else if (stemInfo.stemDirection == 1)
		stem = String("Right");
	else
		stem =  String("No note in this segment");

	//System.out.println("Stem position: " + stem);

	YProjection yProj = YProjection(buffImage);
	yProj.calcYProjection(stave.start, stave.end, xStart, xStop);

	//yProj.printYProjection();

	int *yProjection = yProj.getYProjection();

	// Threshold function to remove stems
	for (int j = 0; j < stave.end - stave.start; j += 1)
		if (yProjection[j] < staveDetection.staveParams.n2)
			yProjection[j] = 0;

	// Remove stavelines from projection
	for (int j = 0; j < 5; j += 1)
	{
		StavePeaks sPeak = stave.getStave_line(j);
		int stavelineStart = sPeak.start - 1;

		//System.out.println("stavelineStart: " + (stavelineStart-stave.getStart()));
		for (int n = stavelineStart - stave.start; n < (stavelineStart - stave.start) + staveDetection.staveParams.n2; n += 1)
		{
			//System.out.println("n=" + n);
			
				yProjection[n] = 0;
		}
	}

	/*System.out.println("Y-PROJECTION START");
	for (int i=0; i<stave.getEnd() - stave.getStart(); i+=1)
	System.out.println(yProjection[i]);
	System.out.println("Y-PROJECTION END");
	*/
/*
	bool start = false;
	int startPos = 0;

	int wcount = 0;
	for (int j = 0; j < stave.end - stave.start; j += 1)
	{
		// Case when we have a non-zero element in projection
		if (yProjection[j] > 0)
		{
			//System.out.println("*** Start at " + j);
			wcount = 0;
			if (!start)
				startPos = j;
			start = true;
		}

		else
		{

			wcount += 1;
			int heightCheck = (j - wcount) - startPos;
			//System.out.println("Height check = " + heightCheck + "  wcount= " + wcount);
			if ((wcount > staveDetection.staveParams.d1 || (j == stave.end - stave.start - 1)) && heightCheck > staveDetection.staveParams.n2)
			{
				if (start)
				{
					int yStart = stave.start + startPos - 6;
					int yEnd = 6 + stave.start + j - wcount - (stave.start + startPos);

					if (stemInfo.stemDirection == 1 || stemInfo.stemDirection == 0)
					{
						int xDiff = xStop - xStart;
						xStart -= xDiff*0.1;
					}
					//xStart -= 6;
					//xStop +=3;
					//xStart -=6;


					//System.out.printf("xStart=%d, xEnd=%d, yStart=%d, yEnd=%d\n", xStart, xStop, yStart, yEnd);

					//Draw L2_Segment
				//	DrawingTools.drawBox(dupImage, yStart, yStart + yEnd, xStart, xStop, Color.ORANGE);


					//Test neural network
					//double data[] = ANNPrepare.prepareImage(buffImage(Range(xStart + 4, xStop - xStart + 3),Range(  yStart, yEnd + 6)));
					//GUI.getNeuralNetwork().testNet(neuralData);
					double neuralData[1][128];
					//neuralData[0] = data;
					//SymbolConfidence result = GUI.getANNInterrogator().interogateNN(neuralData);
					String name;
					//Uncomment when NN is taken care of
					//L2_Segment l2_temp =L2_Segment(yStart, result.getName(), result.getRMSE());
					double accuracy;
				//	L2_Segment l2_temp = L2_Segment(yStart, name, result.getRMSE());
					L2_Segment l2_temp ;
				//	l2_temp.printInfo();
				//	l2_Segments.push_back(l2_temp);

					//System.out.printf("Symbol: %s  Percentage: %f\n", res.symbolName, res.percentage);

					start = false;
					wcount = 0;
				}
			}
		}
	}
}
NoteStem L1_Segment::getStemInfo()
{
	return stemInfo;
}
vector<L2_Segment> L1_Segment::getL2_Segment()
{
	return l2_Segments;
}
int L1_Segment::getWidth()
{
	return xStop - xStart;
}*/