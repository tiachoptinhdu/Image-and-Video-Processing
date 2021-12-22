#include"Blur.h"
#include"Convolution.h"
#include <math.h>
#include <numeric>

# define M_PI          3.14159265358979323846

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	if (sourceImage.data == NULL)
		return 0;

	// Mean filter
	if (method == 0)
	{
		Convolution x;
		vector<float>kernel;
		float val = 1.0 / (kWidth * kHeight); // value
		for (int i = 0; i < kWidth * kHeight; i++)
		{
			kernel.push_back(val);
		}
		x.SetKernel(kernel, kWidth, kHeight);
		Mat img;
		x.DoConvolution(sourceImage, img);
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		for(int i = 0; i < destinationImage.rows; i++)
			for (int j = 0; j < destinationImage.cols; j++)
			{
				destinationImage.at<uchar>(i, j) = saturate_cast<uchar>(img.at<float>(i, j));
			}

	}

	// Median filter
	if (method == 1)
	{
		Convolution x; 
		vector<float> kernel; // kernel có size = 0 là thực hiện phép toán meadian
		x.SetKernel(kernel, kWidth, kHeight);
		x.DoConvolution(sourceImage, destinationImage);
	}
	//Gaussian Filter
	if (method == 2)
	{
		Convolution x; 
		vector<float>kernel;
		// Tinh ma tran kernel gauss
		for(int i = 0; i < kWidth; i++)
			for (int j = 0; j < kHeight; j++)
			{
				// Đưa tâm kernel về chính giữa
				int ireal = i - kWidth / 2;
				int jreal = j - kHeight / 2;
				float sigma = 1.5;
				// Công thức gauss;
				float value = (1.0 / (2 * M_PI * sigma * sigma)) * exp(-(ireal * ireal + jreal * jreal) / (2 * sigma * sigma));
				kernel.push_back(value);
			}
		// Tổng các hệ số của kernel gauss 
		float sum = 0;
		for (int i = 0; i < kernel.size(); i++)
			sum += kernel[i];
		// Đưa hệ số kernel gauss về tổng bằng 1
		for (int i = 0; i < kernel.size(); i++)
			kernel[i] = kernel[i]/sum;

		Mat img;
		x.SetKernel(kernel, kWidth, kHeight);
		x.DoConvolution(sourceImage, img);
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		for (int i = 0; i < destinationImage.rows; i++)
			for (int j = 0; j < destinationImage.cols; j++)
			{
				destinationImage.at<uchar>(i, j) = saturate_cast<uchar>(img.at<float>(i, j));
			}
	}
	return 1;
}
Blur::Blur() {}
Blur::~Blur() {}