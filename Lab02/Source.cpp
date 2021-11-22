#include"GeometricTransformer.h"

int main(int argc, char* argv[])
{
	if (strcmp(argv[1], "-zoom") == 0)
	{
		//Lab02.exe -zoom -nn 2 2 C:/Users/ASUS/Desktop/lena.png C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[5], IMREAD_COLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage; 
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[2], "-bl") == 0)
				interpolator = new BilinearInterpolate();
			if (strcmp(argv[2], "-nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate();
			}
			GeometricTransformer g;
			float sx = stof(argv[3]);
			float sy = stof(argv[4]);
			int check = g.Scale(sourceImage, destinationImage, sx, sy, interpolator);
			if (check == 0)
				cout << "Error" << endl;
			else
				imwrite(argv[6], destinationImage);
		}
	}
	else if (strcmp(argv[1], "-resize") == 0)
	{
		cout << "-resize" << endl;
		//Lab02.exe -resize -nn 250 200 C:/Users/ASUS/Desktop/lena.png C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[5], IMREAD_COLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[2], "-bl") == 0)
			{
				interpolator = new BilinearInterpolate();
				cout << "-bl" << endl;
			}
			if (strcmp(argv[2], "-nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate();
				cout << "-nn" << endl;
			}

			GeometricTransformer g;
			float nw = stof(argv[3]);
			float nh = stof(argv[4]);
			cout << nw << " " << nh << endl;
			int check = g.Resize(sourceImage, destinationImage, nw, nh, interpolator);
			if (check == 0)
				cout << "Error" << endl;
			else
				imwrite(argv[6], destinationImage);
		}
	}
	else if (strcmp(argv[1], "-rotK") == 0)
	{
		//Lab02.exe -rotK -nn 60 C:/Users/ASUS/Desktop/lena.png C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[4], IMREAD_COLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[2], "-bl") == 0)
				interpolator = new BilinearInterpolate();
			if (strcmp(argv[2], "-nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate();
			}
			GeometricTransformer g;
			float angle = stof(argv[3]);
			int check = g.RotateUnkeepImage(sourceImage, destinationImage, angle, interpolator);
			if (check == 0)
				cout << "Error" << endl;
			else
				imwrite(argv[5], destinationImage);
		}
	}
	else if (strcmp(argv[1], "-rotP") == 0)
	{
		//Lab02.exe -rotP -bl 30 C:/Users/ASUS/Desktop/lena.png C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[4], IMREAD_COLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[2], "-bl") == 0)
				interpolator = new BilinearInterpolate();
			if (strcmp(argv[2], "-nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate();
			}
			GeometricTransformer g;
			float angle = stof(argv[3]);
			int check = g.RotateKeepImage(sourceImage, destinationImage, angle, interpolator);
			if (check == 0)
				cout << "Error" << endl;
			else
				imwrite(argv[5], destinationImage);
		}
	}
	else if (strcmp(argv[1], "-flipV") == 0)
	{
		//Lab02.exe -flipV -bl C:/Users/ASUS/Desktop/lena.png C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[3], IMREAD_COLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[2], "-bl") == 0)
				interpolator = new BilinearInterpolate();
			if (strcmp(argv[2], "-nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate();
			}
			GeometricTransformer g;
			int check = g.Flip(sourceImage, destinationImage, 1, interpolator);
			if (check == 0)
				cout << "Error" << endl;
			else
				imwrite(argv[4], destinationImage);
		}
	}
	else if (strcmp(argv[1], "-flipH") == 0)
	{
		//Lab02.exe -flipH -nn C:/Users/ASUS/Desktop/lena.png C:/Users/ASUS/Desktop/test.png
		Mat sourceImage = imread(argv[3], IMREAD_COLOR);
		if (sourceImage.data != NULL)
		{
			Mat destinationImage;
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[2], "-bl") == 0)
				interpolator = new BilinearInterpolate();
			if (strcmp(argv[2], "-nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate();
			}
			GeometricTransformer g;
			int check = g.Flip(sourceImage, destinationImage, 0, interpolator);
			if (check == 0)
				cout << "Error" << endl;
			else
				imwrite(argv[4], destinationImage);
		}
	}
	
	return 0;
}