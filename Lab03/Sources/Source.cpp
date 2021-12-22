#include"Blur.h"
#include"EdgeDetector.h"
#include"Convolution.h"
#include<iostream>

using namespace std;
int main(int argc, char* argv[])
{
	// Average  Filter
	if (strcmp(argv[1], "-avg") == 0)
	{
		Mat sourceImage = imread(argv[4], IMREAD_GRAYSCALE);
		//Lab03 -avg 3 3 C:/opencv/lena.png C:/opencv/test.png
		Blur x; Mat destinationImage;

		int check = x.BlurImage(sourceImage, destinationImage, stoi(argv[2]), stoi(argv[3]), 0);
		if (check == 0)
			cout << "Cannot read Image" << endl;
		else 
			imwrite(argv[5], destinationImage);
	}
	// Median Filter
	if (strcmp(argv[1], "-med") == 0)
	{
		//Lab03 -med 3 3 C:/opencv/lena.png C:/opencv/test.png
		Mat sourceImage = imread(argv[4], IMREAD_GRAYSCALE);
		Blur x; Mat destinationImage;
		int check = x.BlurImage(sourceImage, destinationImage, stoi(argv[2]), stoi(argv[3]), 1);
		if (check == 0)
			cout << "Cannot read Image" << endl;
		else
			imwrite(argv[5], destinationImage);
	}
	//Gaussian Filter
	if (strcmp(argv[1], "-gau") == 0)
	{
		//Lab03 -gau 3 3 C:/opencv/lena.png C:/opencv/test.png
		Mat sourceImage = imread(argv[4], IMREAD_GRAYSCALE);
		Blur x; Mat destinationImage;
		int check = x.BlurImage(sourceImage, destinationImage, stoi(argv[2]), stoi(argv[3]), 2);
		if (check == 0)
			cout << "Cannot read Image" << endl;
		else
			imwrite(argv[5], destinationImage);
	}
	// Sobel Edge Detect
	if (strcmp(argv[1], "-sobel") == 0)
	{
		//Lab03 -sobel C:/opencv/lena.png C:/opencv/test.png
		Mat sourceImage = imread(argv[2], IMREAD_GRAYSCALE);
		EdgeDetector x; Mat destinationImage;
		int check =x.DetectEdge(sourceImage, destinationImage, 1);
		if (check == 0)
			cout << "Cannot read Image" << endl;
		else
			imwrite(argv[3], destinationImage);
	}
	//Prewitt Edge Detect
	if (strcmp(argv[1], "-prew") == 0)
	{
		//Lab03 -prew C:/opencv/lena.png C:/opencv/test.png
		Mat sourceImage = imread(argv[2], IMREAD_GRAYSCALE);
		EdgeDetector x; Mat destinationImage;
		int check = x.DetectEdge(sourceImage, destinationImage, 2);
		if (check == 0)
			cout << "Cannot read Image" << endl;
		else
			imwrite(argv[3], destinationImage);
	}
	//
	if (strcmp(argv[1], "-lap") == 0)
	{
		//Lab03 -lap C:/opencv/lena.png C:/opencv/test.png
		Mat sourceImage = imread(argv[2], IMREAD_GRAYSCALE);
		EdgeDetector x; Mat destinationImage;
		int check = x.DetectEdge(sourceImage, destinationImage, 3);
		if (check == 0)
			cout << "Cannot read Image" << endl;
		else
			imwrite(argv[3], destinationImage);
	}


	return 0;

















	/*Mean and Median Filter */
	/*
	Mat sourceImage = imread("medi.png", IMREAD_GRAYSCALE);
	Mat mean_destinationImage;
	Mat median_destinationImage;
	Mat gauss_destinationImage;
	Blur x;
	x.BlurImage(sourceImage, mean_destinationImage, 10, 10, 0);
	x.BlurImage(sourceImage, median_destinationImage, 10, 10, 1);
	x.BlurImage(sourceImage, gauss_destinationImage, 10, 10, 2);
	
	imshow("mean", mean_destinationImage);
	imshow("median", median_destinationImage);
	imshow("gauss", gauss_destinationImage);
	imshow("before", sourceImage);
	waitKey(0);
	*/
	/*Sobel  prewitt and laplace Edge detectector*/
	/*
	Mat sourceImage = imread("lena.png",IMREAD_GRAYSCALE);
	Mat sobel_destinationImage, prewitt_destinationImage, laplace_destinationImage;
	EdgeDetector x;

	x.DetectEdge(sourceImage, sobel_destinationImage, 0);
	x.DetectEdge(sourceImage, prewitt_destinationImage, 1);
	x.DetectEdge(sourceImage, laplace_destinationImage, 2);
	
	imshow("sobel", sobel_destinationImage);
	imshow("prewit", prewitt_destinationImage);
	imshow("laplace", laplace_destinationImage);
	waitKey(0);
	*/

	
	

	





}