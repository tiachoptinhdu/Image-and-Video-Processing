#include"ColorTransformer.h"

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{ 
	if (b > 255 || b < -255 || sourceImage.data == NULL)
		return 0;
	if (sourceImage.channels() == 1)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, Scalar(0));
		for (int i = 0; i < destinationImage.rows; i++)
		{
			for (int j = 0; j < destinationImage.cols; j++)
			{
				for (int channels = 0; channels < destinationImage.channels(); channels++)
				{
					destinationImage.at<uchar>(i, j) = saturate_cast<uchar>(sourceImage.at<uchar>(i, j) + b); // Từng pixel tăng lên b đơn vị
				}
			}
		}
	}
	if (sourceImage.channels() == 3)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC3, Scalar(0));
		for (int i = 0; i < destinationImage.rows; i++)
		{
			for (int j = 0; j < destinationImage.cols; j++)
			{
				for (int channels = 0; channels < destinationImage.channels(); channels++)
				{
					destinationImage.at<Vec3b>(i, j)[channels] = saturate_cast<uchar>(sourceImage.at<Vec3b>(i, j)[channels] + b); // Từng pixel tăng lên b đơn vị
				}
			}
		}
	}

	return 1;
}
int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	if (c < 0 || sourceImage.data == NULL)
		return 0;

	if (sourceImage.channels() == 1)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, Scalar(0));
		for (int i = 0; i < destinationImage.rows; i++)
		{
			for (int j = 0; j < destinationImage.cols; j++)
			{
				for (int channels = 0; channels < destinationImage.channels(); channels++)
				{
					destinationImage.at<uchar>(i, j) = saturate_cast<uchar>(sourceImage.at<uchar>(i, j) * c); //*c cho từng pixel
				}
			}
		}
	}
	if (sourceImage.channels() == 3)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC3, Scalar(0));
		for (int i = 0; i < destinationImage.rows; i++)
		{
			for (int j = 0; j < destinationImage.cols; j++)
			{
				for (int channels = 0; channels < destinationImage.channels(); channels++)
				{
					destinationImage.at<Vec3b>(i, j)[channels] = saturate_cast<uchar>(sourceImage.at<Vec3b>(i, j)[channels] * c);
				}
			}
		}

	}

	return 1;
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
{
	if (sourceImage.data == NULL)
		return 0;
	int L = 256;
	histMatrix = Mat(sourceImage.channels(), L, CV_16UC1, Scalar(0)); // Ma trận:channels * L chẳng hạn 3 * 256 mỗi phần tử là số nguyên unsigned short
	if (sourceImage.channels() == 1)
	{
		for (int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
			{
				histMatrix.at<unsigned short>(sourceImage.at<uchar>(i, j))++;
			}
	}
	if (sourceImage.channels() == 3)
	{
		for (int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
			{
				for (int channels = 0; channels < sourceImage.channels(); channels++)
				{
					histMatrix.at<unsigned short>(channels, sourceImage.at<Vec3b>(i, j)[channels])++;
				}
			}
	}
	return 1;
}

int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage)
{
	int his_row = 400;
	int his_col = 256 * 3;
	histImage = Mat(his_row, his_col, CV_8UC3, Scalar(255, 255, 255)); // khởi tạo ảnh histogram (3 channels) màu trắng

	if (histMatrix.rows == 1)
	{
		const unsigned short* row_ptr = histMatrix.ptr<unsigned short>(0);

		float scale = (1.0 * histImage.rows) / *max_element(row_ptr, row_ptr + 256);  // hệ số tỉ lệ khi scale biểu đồ cột tới khoảng (0, his_row)
		int hist_after_scale[256] = { 0 };
		for (int i = 0; i < 256; i++)
		{
			hist_after_scale[i] = scale * row_ptr[i];
		}
		// Vẽ các cột histogram với từng channels
		for (int i = 0; i < 256; i++)
		{
			line(histImage, Point(i * 3, histImage.rows), Point(i * 3, histImage.rows - hist_after_scale[i]), Scalar(0,0,0), 3);
		}
	}
	if (histMatrix.rows == 3)
	{
		Scalar color[3];
		color[0] = Scalar(255, 0, 0); // Blue
		color[1] = Scalar(0, 255, 0); // Green
		color[2] = Scalar(0, 0, 255); // Red

		for (int channels = 0; channels < histMatrix.rows; channels++)
		{

			const unsigned short* row_ptr = histMatrix.ptr<unsigned short>(channels);

			float scale = (1.0 * histImage.rows) / *max_element(row_ptr, row_ptr + 256);  // hệ số tỉ lệ khi scale biểu đồ cột tới khoảng (0, his_row)
			int hist_after_scale[256] = { 0 };
			for (int i = 0; i < 256; i++)
			{
				hist_after_scale[i] = scale * row_ptr[i];
			}
			// Vẽ các cột histogram với từng channels
			for (int i = 0; i < 256; i++)
			{
				line(histImage, Point(channels * 256 + i, histImage.rows), Point(channels * 256 + i, histImage.rows - hist_after_scale[i]), color[channels], 1);
			}

		}
	}

	return 1;
}
int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{

	if (sourceImage.data == NULL)
		return 0;
	// TH1: Anh Gray Scale Ý tưởng: 
	// Histogram equalization 
	if (sourceImage.channels() == 1)
	{
		Mat histMatrix;
		CalcHistogram(sourceImage, histMatrix);
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, Scalar(0));
		int F[256] = { 0 }; // Ham CDF = ham phan phoi tich luy 
		F[0] = histMatrix.at<unsigned short>(0);
		for (int i = 1; i < histMatrix.cols; i++)
		{
			F[i] = F[i - 1] + histMatrix.at<unsigned short>(i);
		}
		int number_of_pixel = sourceImage.cols * sourceImage.rows; // Số pixel trong ảnh cũng chính là (tổng của F)
		// Chuyen ham tich luy F ve dang tan so G (0->1)
		float G[256] = { 0 };
		for (int i = 0; i < 256; i++)
			G[i] = (1.0 * F[i]) / number_of_pixel;
		// Anh xa H tu x->y  Bien pixel value x thanh y
		int H[256] = { 0 };
		for (int i = 0; i < 256; i++)
			H[i] = int(G[i] * 255);
		//Dung anh xa cho tung pixel cua anh
		for (int i = 0; i < destinationImage.rows; i++)
			for (int j = 0; j < destinationImage.cols; j++)
			{
				destinationImage.at<uchar>(i, j) = H[sourceImage.at<uchar>(i, j)];
			}
	}
	// Th2: Color Image
	// Ý tưởng: Cân bằng theo từng kênh R,G,B sẽ làm hỏng màu sắc của ảnh 
	// Đưa về dạng HSV sau đó cân bằng Value  giữ nguyên H,S (màu sắc)
	if (sourceImage.channels() == 3)
	{
		Mat histMatrix;
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);
		cvtColor(sourceImage, destinationImage, COLOR_RGB2HSV);
		CalcHistogram(sourceImage, histMatrix);
		int F[256] = { 0 };
		F[0] = histMatrix.at<unsigned short>(2, 0);
		for (int i = 1; i < histMatrix.cols; i++)
		{
			F[i] = F[i - 1] + histMatrix.at<unsigned short>(2, i);
		}

		int number_of_pixel = sourceImage.cols * sourceImage.rows; // Số pixel trong ảnh cũng chính là (tổng của F)
			// Chuyen ham tich luy F ve dang tan so G (0->1)
		float G[256] = { 0 };
		for (int i = 0; i < 256; i++)
			G[i] = (1.0 * F[i]) / number_of_pixel;
		// Anh xa H tu x->y  Bien pixel value x thanh y
		int H[256] = { 0 };
		for (int i = 0; i < 256; i++)
			H[i] = int(G[i] * 225);
		//Dung anh xa cho tung pixel cua anh
		for (int i = 0; i < destinationImage.rows; i++)
			for (int j = 0; j < destinationImage.cols; j++)
			{
				destinationImage.at<Vec3b>(i, j)[2] = H[sourceImage.at<Vec3b>(i, j)[2]];
			}
		cvtColor(destinationImage, destinationImage, COLOR_HSV2RGB);
	}
	return 1;
}
ColorTransformer::ColorTransformer()
{}
ColorTransformer::~ColorTransformer()
{}


