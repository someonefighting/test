#include"YProjection.h"
using namespace omr_engine;

YProjection::YProjection(Mat img)
{
	buffImage = img;
	row = img.rows;
}
/**
* Cacluate the Y-Projection of the BufferedImage
* @param startH Desired start Y-Coordinate of the BufferedImage
* @param endH Desired end Y-Coordinate of the BufferedImage
* @param startW Desired start X-Coordinate of the BufferedImage
* @param endW Desired end X-Coordinate of the BufferedImage
*/

void YProjection::calcYProjection(int startH, int endH, int startW, int endW)
{
	int height = endH - startH ;
	row = height;
	yProjection = (int *)malloc(sizeof(int)*height);
	int sum = 0;
	for (int i = startH; i < endH; i += 1)
	{
		uchar* data = buffImage.ptr<uchar>(i);
		yProjection[i-startH] = 0;
		//sum = 0;
		for (int j = startW; j < endW; j += 1)
		{
			int color;
			color = data[j];
			if (color == 255) // white pixel
			{
				// Do nothing
			}
			else
			{
				yProjection[i - startH] += 1;
				//sum++;
			}
		}

	}
}

/**
* Returns the resulting Y-Projection of the BufferedImage
* @return yProjection
*/

int* YProjection::getYProjection()
{
	return yProjection;
}


/**
* Prints the Y-Projection of the BufferedImage
*
*/

void YProjection::printYProjection()
{
	printf("Y-Projection");
	for (int i = 0; i < row; i += 1)
	{
		printf("%d\n", yProjection[i]);
	}
	printf("END Y-Projection");
	Mat histogramImage(buffImage.rows, buffImage.cols, CV_8UC1);
	for (int i = 0; i < buffImage.rows; i++)
	{

		uchar *data = histogramImage.ptr<uchar>(i);
		for (int j = 0; j < buffImage.cols; j++)
		{
			//设置为黑色。  
			data[j] = 0;
		}
	}
	show(& histogramImage);
	for (int i = 0; i < buffImage.rows; i++)
	{
		uchar *data = histogramImage.ptr<uchar>(i);
		for (int j = 0; j<yProjection[i]; j++)
		{
			//value = 255;  //设置为白色  
			data[j] = 255;
		}

	}
	imwrite("yproj.jpg", histogramImage);
	show(&histogramImage);
}


/**
* Return the size of the yProjection array. In other words, <code> height = endH - startH </code>
* @return height
*/

int YProjection::getrow()
{
	return row;
}



YProjection::YProjection()
{
	yProjection = { 0 };
}


YProjection::~YProjection()
{
}
