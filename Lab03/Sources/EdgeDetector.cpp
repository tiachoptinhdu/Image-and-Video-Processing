#include"EdgeDetector.h"
#include"Convolution.h"

int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method)
{
	if (sourceImage.data == NULL)
		return 0;
	//Sobel Edge detector
	if (method == 1)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		Mat fx = Mat(sourceImage.rows, sourceImage.cols, CV_32FC1);
		Mat fy = Mat(sourceImage.rows, sourceImage.cols, CV_32FC1);
		Convolution x;
		vector<float> Wx{ 0.25,0,-0.25,0.5,0,-0.5,0.25,0,-0.25 };
		vector<float> Wy{ -0.25,-0.5, -0.25, 0 , 0 , 0, 0.25,0.5,0.25 };
		x.SetKernel(Wx, 3, 3);
		x.DoConvolution(sourceImage, fx);
		x.SetKernel(Wy, 3, 3);
		x.DoConvolution(sourceImage, fy);
		Mat image = fx.mul(fx) + fy.mul(fy);
		// Độ lớn vector gradient chuyển về uchar
		for (int x = 0; x < destinationImage.rows; x++)
			for (int y = 0; y < destinationImage.cols; y++)
			{
				destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(sqrt(double(image.at<float>(x, y))));
			}

			/* Chuyển về binaryEdgeImage

			for(int x = 0; x < destinationImage.rows; x++)
				for (int y = 0; y < destinationImage.cols; y++)
				{
					if(sqrt(double(destinationImage.at<uchar>(x, y))>220))
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(sqrt(double(destinationImage.at<uchar>(x, y))));
				}
			*/
	}
	//1 Prewitt Edge detector
	if (method == 2)
	{
		Mat fx = Mat(sourceImage.rows, sourceImage.cols, CV_32FC1);
		Mat fy = Mat(sourceImage.rows, sourceImage.cols, CV_32FC1);
		Convolution x;
		vector<float>Wx{ 0.333333333,0, -0.333333333, 0.333333333, 0, -0.333333333, 0.333333333, 0, -0.333333333 };
		vector<float>Wy{ -0.333333333, -0.333333333, -0.333333333, 0,0,0,0.333333333, 0.333333333, 0.333333333 };
		x.SetKernel(Wx, 3, 3);
		x.DoConvolution(sourceImage, fx);
		x.SetKernel(Wy, 3, 3);
		x.DoConvolution(sourceImage, fy);
		Mat image = fx.mul(fx) + fy.mul(fy);
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		// Độ lớn vector gradient chuyển về uchar
		for (int x = 0; x < destinationImage.rows; x++)
			for (int y = 0; y < destinationImage.cols; y++)
			{
				destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(sqrt(double(image.at<float>(x, y))));
			}

	}
	// Laplace Edge detector
	if (method == 3)
	{
		Mat f;
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		Convolution x;
		vector<float> W{ 1,1,1,1,-8,1,1,1,1 };
		x.SetKernel(W, 3, 3);
		x.DoConvolution(sourceImage, f);
		for(int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
			{
				destinationImage.at<uchar>(i, j) = saturate_cast<uchar>(f.at<float>(i,j));
			}

	}
	return 1;
}
EdgeDetector::EdgeDetector(){}
EdgeDetector::~EdgeDetector(){}