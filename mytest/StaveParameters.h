#ifndef StaveParameters_HEAD
#define StaveParameters_HEAD
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include"show.h"
//#include "NoteHead.h"
using namespace std;
using namespace cv;

/** The <code> StaveParameters </code> is a class responsible for calculating two important paramters
* in a BufferedImage. Those two parameters are the thickness of a stave line and the distance between
* two stave lines. Given those two parameters, a lower and upper threshold value is determined.
* <p>
* The <code> StaveParameters </code> class is used as follows:
* <p>
* <code>
* StaveParameters sParams = new StaveParameters(buffImage); <br>
* sParams.calcParameters();
* </code>
* <p>
* Calling the calcParameters() method will calculate values for the following private fields:
* <p>
*<code>int n1</code> - The lower threshold value for the thickness of a stave line <br>
*<code>int n2</code> - The upper threshold value for the thickness of a stave line <br>
*<code>int d1</code> - The lower threshold value for the distance between two stave lines <br>
*<code>int d2</code> - The upper threshold value for the distance between two stave lines <br>
*/
namespace omr_engine
{
	class StaveParameters
	{
	public:
		StaveParameters();
		~StaveParameters();
		Mat buffImage;
		int len;
		int wPixels[100], bPixels[100];
		int height, width;
		
		static const int wThreshold = 2;
		static const int bThreshold = 0;

		int n1 = 0;
		int n2 = 0;
		int d1 = 0;
		int d2 = 0;

		StaveParameters(Mat buffImage);
		void calcParameters();
		void printHistogram();
		void calcRLE(int startW, int endW);
		int calcParams();
		int interpMin(int pos, int third, int pixels[]);
		int interpMax(int pos, int third, int pixels[]);

	};
}
#endif
