#include"show.h"
void show(Mat *mimg)

{
	IplImage *img = &IplImage(*mimg);//
	cvNamedWindow("contour1", 0);
	cvShowImage("contour1", img);
#ifdef _DEBUG
	cvWaitKey(0); //�ȴ�����
#else 
	cvWaitKey(100);
#endif

	//���ٴ���
	//cvDestroyWindow("contour1");
	//�ͷ�ͼ��
	//cvReleaseImage(&img);

}
void show(IplImage *img)

{

	cvNamedWindow("contour1", 0);
	//cvResizeWindow("Control", 400, 400);
	cvShowImage("contour1", img);
#ifdef _DEBUG
	cvWaitKey(0); //�ȴ�����
#else 
	cvWaitKey(100);
#endif
	//���ٴ���
	//cvDestroyWindow("contour1");
	//�ͷ�ͼ��
	//cvReleaseImage(&img);

}