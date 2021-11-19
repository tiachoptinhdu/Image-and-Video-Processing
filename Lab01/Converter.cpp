#include"Converter.h"
int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
		return 0;

	
	if (sourceImage.channels() == 3)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1); // grayScale Image
		for (int i = 0; i < destinationImage.rows; i++)
			for (int j = 0; j < destinationImage.cols; j++)
			{
				uchar gray_scale = saturate_cast < uchar>(0.144 * sourceImage.at<Vec3b>(i, j)[0] + 0.587 * sourceImage.at<Vec3b>(i, j)[1] + 0.299 * sourceImage.at<Vec3b>(i, j)[2]);
				destinationImage.at<uchar>(i, j) = gray_scale;
			}
		return 1;
	}
	if (sourceImage.channels() == 1)
	{
		destinationImage = sourceImage.clone();
		return 1;
	}
	return 0;
	
}
int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.channels() == 1)
		return 0;

	destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);
	for (int i = 0; i < sourceImage.rows; i++)
		for (int j = 0; j < sourceImage.cols; j++)
		{
			float R = int(sourceImage.at<Vec3b>(i, j)[0]);
			float G = int(sourceImage.at<Vec3b>(i, j)[1]);
			float B = int(sourceImage.at<Vec3b>(i, j)[2]);
			B /= 255; G /= 255; R /= 255;
			float maximum = max(R, max(G, B));
			float minimum = min(R, min(G, B));
			float V, H, S;
			V = maximum;
			if (V != 0)
				S = (maximum - minimum) / maximum;
			else S = 0;
			if (V == R)
			{
				H = 60 * (G - B) / (maximum - minimum);
			}
			if (V == G)
			{
				H = 120 + 60 * (B - R) / (maximum - minimum);
			}
			if (V == B)
			{
				H = 240 + 60 * (R - G) / (maximum - minimum);
			}
			V = 255 * V; S = 255 * S; H = H / 2;
			destinationImage.at<Vec3b>(i, j)[0] = uchar(round(H));
			destinationImage.at<Vec3b>(i, j)[1] = uchar(round(S));
			destinationImage.at<Vec3b>(i, j)[2] = uchar(round(V));
		}
}
int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	if (sourceImage.data == NULL || (type != 0 && type != 1))
		return 0;
	int check = 0;
	if (type == 0)
	{
		 check = RGB2GrayScale(sourceImage, destinationImage);
	}
	else if (type == 1)
	{
		check = RGB2HSV(sourceImage, destinationImage);
	}
	return check;

}
Converter::Converter() {};
Converter::~Converter() {};