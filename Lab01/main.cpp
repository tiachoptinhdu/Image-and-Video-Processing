#include"ColorTransformer.h"
#include"Converter.h"
int main(int argc, char* argv[])
{

	if (strcmp(argv[1], "-rgb2gray") == 0)
		{
		//Lab01.exe -rgb2gray C:/Users/ASUS/Desktop/lena.jfif C:/Users/ASUS/Desktop/test.png
			Mat sourceImage = imread(argv[2], IMREAD_ANYCOLOR);
			if (sourceImage.data != NULL)
			{
				Mat destinationImage;
				Converter cvt;
				int check = cvt.Convert(sourceImage, destinationImage, 0);
				if (check == 0)
					cout << "Error" << endl;
				else
				{
					imwrite(argv[3], destinationImage);
				}
			}
			else
				cout << "Cant read image" << endl;
		}
	else if (strcmp(argv[1], "-rgb2hsv") == 0)
		{
		//Lab01.exe -rgb2hsv C:/Users/ASUS/Desktop/lena.jfif C:/Users/ASUS/Desktop/test.png
			Mat sourceImage = imread(argv[2], IMREAD_ANYCOLOR);
			if (sourceImage.data != NULL)
			{
				Mat destinationImage;
				Converter cvt;
				int check = cvt.Convert(sourceImage, destinationImage, 1);
				if (check == 0)
					cout << "Error" << endl;
				else
				{
					imwrite(argv[3], destinationImage);
				}
			}
			else
				cout << "Cant read image file" << endl;
		}
	else if (strcmp(argv[1], "-bright") == 0)
		{
		//Lab01.exe -bright 100 C:/Users/ASUS/Desktop/lena.jfif C:/Users/ASUS/Desktop/test.png 
		Mat sourceImage = imread(argv[3], IMREAD_ANYCOLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			int b = atoi(argv[2]);
			ColorTransformer colortransform;
			int check = colortransform.ChangeBrighness(sourceImage, destinationImage, b);
			if (check == 0)
				cout << "Error" << endl;
			else
			{
				imwrite(argv[4], destinationImage);
			}
		}
		}
	else if (strcmp(argv[1], "-constrast") == 0)
	{
		//Lab01.exe -constrast 3 C:/Users/ASUS/Desktop/lena.jfif C:/Users/ASUS/Desktop/test.png 
		Mat sourceImage = imread(argv[3], IMREAD_ANYCOLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			float c = stof(argv[2]);
			ColorTransformer colortransform;
			int check = colortransform.ChangeContrast(sourceImage, destinationImage, c);
			if (check == 0)
				cout << "Error" << endl;
			else
			{
				imwrite(argv[4], destinationImage);
			}
		}
		else
			cout << "Cannot Read image" << endl;
	}
	else if (strcmp(argv[1], "-drawhist") == 0) 
	{
		//Lab01.exe -drawhist C:/Users/ASUS/Desktop/lena.jfif C:/Users/ASUS/Desktop/test.png 
		Mat sourceImage = imread(argv[2], IMREAD_ANYCOLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage; 
			Mat hist;
			ColorTransformer colortransform;
			colortransform.CalcHistogram(sourceImage, hist);
			int check = colortransform.DrawHistogram(hist, destinationImage);
			if (check == 0)
				cout << "Error" << endl;
			else
			{
				imwrite(argv[3], destinationImage);
			}
		}
		else
			cout << "Cant read Image" << endl;
	}
	else if (strcmp(argv[1], "-equalhist") == 0)
	{
		//Lab01.exe -equalhist C:/Users/ASUS/Desktop/lena.jfif C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[2], IMREAD_ANYCOLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			ColorTransformer colortransform;
			int check = colortransform.HistogramEqualization(sourceImage, destinationImage);
			if (check == 0)
				cout << "Error" << endl;
			else
			{
				imwrite(argv[3], destinationImage);
			}
		}
	}

	return 0;
}


	
