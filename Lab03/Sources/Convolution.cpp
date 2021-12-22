#include"Convolution.h"
vector<float> Convolution::GetKernel() {
	return this->_kernel;
}
void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	_kernel = kernel;
	_kernelWidth = kWidth;
	_kernelHeight = kHeight;
}
int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
		return 1;
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;
	if (sourceImage.channels() == 1)
	{
		
		if (_kernel.size() > 0)
		{
			destinationImage = Mat(rows, cols, CV_32FC1);
			for (int x = 0; x < destinationImage.rows; x++)
				for (int y = 0; y < destinationImage.cols; y++)
				{
					// biến check kiểm tra biên cạnh
					bool check = true;

					// biến trả về kết quả phép convolution
					float result = 0;

					//Tinh toan trong Convolution
					for (int i = 0; i < _kernelHeight; i++)
					{
						for (int j = 0; j < _kernelWidth; j++)
						{
							// Đưa về tâm kernel
							int ireal = i - _kernelHeight / 2;
							int jreal = j - _kernelWidth / 2;
							if (x - ireal < 0 || x - ireal >= rows || y - jreal >= cols || y - jreal < 0)
							{
								check = false;
								break;
								continue;
							}

							result += int(sourceImage.at<uchar>(x - ireal, y - jreal)) * _kernel[i * _kernelWidth + j];
							
						}
						if (check == false)
							break;
					}

					if (check == true)
					{
						destinationImage.at<float>(x, y) = result;
					}
					else
					{
						destinationImage.at<float>(x, y) = sourceImage.at<uchar>(x, y);
					}
				}
			
		}
		else
		{
			destinationImage = Mat(rows, cols, CV_8UC1);
			for (int x = 0; x < destinationImage.rows; x++)
				for (int y = 0; y < destinationImage.cols; y++)
				{
					// Biến trả về kết quả của phép convolution
					float result = 0;
					bool check = true;
					vector<int> neighbor;
					for (int i = 0; i < _kernelHeight; i++)
					{
						for (int j = 0; j < _kernelWidth; j++)
						{
							// Đưa tâm kernel về chính giữa
							int ireal = i - _kernelHeight / 2;
							int jreal = j - _kernelWidth / 2;

							if (x - ireal < 0 || x - ireal >= rows || y - jreal >= cols || y - jreal < 0)
							{
								continue;
							}
							neighbor.push_back(int(sourceImage.at<uchar>(x - ireal, y - jreal)));
						}
					}

					sort(neighbor.begin(), neighbor.end());
					result = neighbor[neighbor.size() / 2];
					destinationImage.at<uchar>(x, y) = saturate_cast<uchar> (result);
				}
		}
		return 1;
	}
	else
		return 0;
	
	



}
Convolution::Convolution() { _kernelWidth = 0; _kernelHeight = 0; }
Convolution::~Convolution() {}
