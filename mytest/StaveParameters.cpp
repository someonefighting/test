#include "StaveParameters.h"
using namespace omr_engine;

StaveParameters::StaveParameters()
{
	len = 100;
}


StaveParameters::~StaveParameters()
{
}
/**
*
* @param buffImage
*
* Initialises the StaveParameters class with buffImage
*/

StaveParameters::StaveParameters(Mat img)
{
	buffImage = img;
	len = 100;
	height = buffImage.rows;
	width = buffImage.cols;

	//calcRLE(0, width - 1);
	//calcParams();
}


/**
* This method will calculate the parameters N1, N2, D1 and D2
*
*/

void StaveParameters::calcParameters()
{
	calcRLE(0, width - 1);
	calcParams();
}


/**
* Returns the value n1 which is the lower threshold value for the thickness of a stave line
* @return n1
*/


/**
* Prints the two int[] arrays containing the RLE of the black pixels and white pixels respectively
*
*/

void StaveParameters::printHistogram()
{
	for (int i = 0; i < len; i += 1)
		printf("%d:%d,%d\n",i,bPixels[i] , wPixels[i]);
}


/*	PRIVATE METHODS */

void StaveParameters::calcRLE(int startW, int endW)
{
#ifdef _DEBUG
	Mat ori = imread("summer.jpg");
	IplImage img = IplImage(ori);
#endif
	for (int i = startW; i < endW; i += 1)
	{
#ifdef _DEBUG
		//cout << i <<  endl;
#endif
		int bcnt = 0, wcnt = 0;
		bool stop = false;

		for (int j = 0; j < height - 1; j += 1)
		{
			uchar *data = buffImage.ptr<uchar>(j);
			int color = data[i];

			if (color != 255 && !stop) // -1 == white pixel
			{
				

				bcnt += 1;
				uchar *data = buffImage.ptr<uchar>(j+1);
				int color = data[i];
				if (color == 255)
				{
					stop = true;
#ifdef _DEBUG
					//cout <<j << "	" ;
#endif
					//if (bcnt > bThreshold && bcnt < 100)
						if (bcnt > 0 && bcnt < 100)
						bPixels[bcnt] += 1;
#ifdef _DEBUG
					/*	if (bcnt == 1)
						{
							CvPoint p1 = CvPoint(i, j);
							CvPoint p2 = CvPoint(i, j - 1);
							cvLine(&img, p1, p2, CvScalar(255, 0, 0), 1, 8);
							show(&img);

						}*/
#endif
					bcnt = 0;
				}
			}
			else
			{
#ifdef _DEBUG
				//���ƺڵ���������
				
				CvPoint p = CvPoint(i, j);
				//cvCircle(&img, p, 1, CvScalar(255, 0, 0), 1, 8);
				//show(&img);
#endif
				wcnt += 1;
				uchar *data = buffImage.ptr<uchar>(j + 1);
				int color = data[i];
				if (color != 255)
				{
#ifdef _DEBUG
					//cout << j << "	";
#endif
					stop = false;
					//if (wcnt > wThreshold && wcnt < 100)
					if (wcnt > 2 && wcnt < 100)
						wPixels[wcnt] += 1;

					wcnt = 0;
				}
			}
		}
		cout << endl;
	}
//	show(&img);
	//imwrite("white.jpg", cvarrToMat(&img));
}

int StaveParameters::calcParams()
{
	int wMax = wPixels[0];
	int bMax = bPixels[0];
	int wPos = 0;
	int bPos = 0;
	int bThird;
	int wThird;

	// find max value in wPixels and bPixels
	for (int i = 0; i < len - 1; i += 1)
	{
		if (wPixels[i] > wMax)
		{
			wMax = wPixels[i];
			wPos = i;
		}
		if (bPixels[i] > bMax)
		{
			bMax = bPixels[i];
			bPos = i;
		}
	}
	// calculate 1/3 of black pixels and white pixels max height
	bThird = bMax / 3;
	wThird = wMax / 3;

	// interpolate to find xMin and xMax
	n1 = interpMin(bPos, bThird, bPixels);
	n2 = interpMax(bPos, bThird, bPixels) ;
	d1 = interpMin(wPos, wThird, wPixels);
	d2 = interpMax(wPos, wThird, wPixels);
	if (n1 <= 0)
		n1 = 1;
	return 1;
}

int StaveParameters::interpMin(int pos, int third, int pixels[])
{
	int temp = 0;
	for (int i = pos; i > 0; i--)
	{
		if (pixels[i] < third)
		{
			// need to interpolate
			temp = ((third - pixels[i + 1]) / (pixels[i] - pixels[i + 1])) + i + 1;
			break;
		}
	}
	return temp;
}

int StaveParameters::interpMax(int pos, int third, int pixels[])
{
	int temp = 0;
	for (int i = pos; i < len - 2; i++)
	{
		if (pixels[i] < third)
		{
			// need to interpolate
			temp = ((third - pixels[i]) / (pixels[i - 1] - pixels[i])) + i + 1;
			break;
		}
	}
	return temp;
}

/*	END PRIVATE METHODS */


/*
public int[] getWPixles()
{
return wPixels;
}

public int[] getBPixles()
{
return bPixels;
}
*/

