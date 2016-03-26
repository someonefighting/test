#include "XProjection.h"
using namespace omr_engine;

XProjection::XProjection()
{
}


XProjection::~XProjection()
{
}

XProjection::XProjection(Mat img)
{
	buffImage = img;
}

/**
* Cacluate the X-Projection of the BufferedImage
* @param startH Desired start Y-Coordinate of the BufferedImage
* @param endH Desired end Y-Coordinate of the BufferedImage
* @param startW Desired start X-Coordinate of the BufferedImage
* @param endW Desired end X-Coordinate of the BufferedImage
*/

void XProjection::calcXProjection(int startH, int endH, int startW, int endW)
{
	int newsize =abs(endW - startW) + 1;
	//System.out.println("Size: " + size);
	size = newsize;
	xProjection = new int[size];

	for (int i = startW; i < endW; i += 1)
	{
		uchar *data = buffImage.ptr < uchar >(i) ;
		for (int j = startH; j < endH; j += 1)
		{
			int color = 0;
			
				color = data[j];
			
			if (color != 255) //if black pixel
			{
				xProjection[i - startW] += 1;
			}
		}
	}
}

/**
* Returns the resulting X-Projection of the BufferedImage
* @return xProjection
*/

int* XProjection::getXProjection()
{
	return xProjection;
}


/**
* Prints the X-Projection of the BufferedImage
*
*/
void XProjection:: printXProjection()
{
	printf("X Projection");
	for (int i = 0; i<size; i += 1)
	{
		printf("%d\n",xProjection[i]);
	}
	puts("END X Projection");
}

