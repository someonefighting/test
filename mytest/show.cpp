#include"show.h"
void show(Mat *mimg)

{
	IplImage *img = &IplImage(*mimg);//
	cvNamedWindow("contour1", 0);
	cvShowImage("contour1", img);
#ifdef _DEBUG
	cvWaitKey(0); //等待按键
#else 
	cvWaitKey(100);
#endif

	//销毁窗口
	//cvDestroyWindow("contour1");
	//释放图像
	//cvReleaseImage(&img);

}
void show(IplImage *img)

{

	cvNamedWindow("contour1", 0);
	//cvResizeWindow("Control", 400, 400);
	cvShowImage("contour1", img);
#ifdef _DEBUG
	cvWaitKey(0); //等待按键
#else 
	cvWaitKey(100);
#endif
	//销毁窗口
	//cvDestroyWindow("contour1");
	//释放图像
	//cvReleaseImage(&img);

}