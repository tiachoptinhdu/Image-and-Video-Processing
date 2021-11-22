#include "GeometricTransformer.h"
#define PI 3.14159265358979323846


//Interpolate
PixelInterpolate::PixelInterpolate() {}
PixelInterpolate::~PixelInterpolate() {}
uchar* BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels)
{
	// Reference Formula: Slide XLAS Bai4 Ly Quoc Ngoc
	// Làm tròn dưới giá trị tx, ty 
	int l = int(tx);
	int k = int(ty);
	float a = tx - l; float b = ty - k;
	uchar result[3];
	for (int channel = 0; channel < nChannels; channel++)
	{
		// Lấy giá trị f(l,k), f(l+1,k), f(l,k+1), f(l+1,k+1) từ ảnh pSrc
		int f_lk = pSrc[l * srcWidthStep + k * nChannels + channel];
		int f_l1k = pSrc[(l + 1) * srcWidthStep + k * nChannels + channel];
		int f_lk1 = pSrc[l * srcWidthStep + (k + 1) * nChannels + channel];
		int f_l1k1 = pSrc[(l + 1) * srcWidthStep + (k + 1) * nChannels + channel];
		result[channel] = saturate_cast<uchar>((1 - a) * (1 - b) * f_lk + a * (1 - b) * f_l1k + b * (1 - a) * f_lk1 + a * b * f_l1k1);
	}
	return result;
}
BilinearInterpolate::BilinearInterpolate(){}
BilinearInterpolate::~BilinearInterpolate() {}
uchar* NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels)
{
	int l = round(tx);
	int k = round(ty);
	uchar result[3];
	for (int channel = 0; channel < nChannels; channel++)
	{
		result[channel] = pSrc[l * srcWidthStep + k * nChannels + channel];
	}
	return result;
}
NearestNeighborInterpolate::NearestNeighborInterpolate() {}
NearestNeighborInterpolate::~NearestNeighborInterpolate() {}

//AffineTransformer Các hàm affine ngược của biến đổi ảnh 
void AffineTransform::Translate(float dx, float dy)
{
	float data[] = { 1,0,-dx,0,1,-dy,0,0,1 };
	Mat affine = Mat(3, 3, CV_32F, data);
	_matrixTransform =  _matrixTransform * affine;
}
void AffineTransform::Rotate(float angle)
{
	// Hàm Rotate một góc angle ngược chiều kim đồng hồ 
	float cosin = cos(angle * PI / 180.0); // tinh cos
	float sinh = sin(angle * PI / 180.0);// tinh sin

	float data[] = { cosin,sinh,0,-sinh,cosin,0,0,0,1 };
	Mat affine = Mat(3, 3, CV_32F, data);
	_matrixTransform = _matrixTransform * affine;
}
void AffineTransform::Scale(float sx, float sy)
{	
	float data[] = { 1.0/sx,0,0,0,1.0/sy,0,0,0,1 };
	Mat affine = Mat(3, 3, CV_32F, data);
	_matrixTransform =  _matrixTransform * affine;
}
void AffineTransform::TransformPoint(float& x, float& y)
{
	// Tính x, y của P'x    P'x =  matrix * Px 
	float data[] = { x,y, 1 }; // Coordinate Matrix [x;y;1]
	Mat result = _matrixTransform * Mat(3, 1, CV_32F, data);
	// Lấy các tọa độ x, y sau khi biến đổi affine
	x = result.at<float>(0,0);
	y = result.at<float>(1,0);
}
AffineTransform::AffineTransform()
{
	// Khởi tạo Indentity affine matrix
	_matrixTransform = Mat(3, 3, CV_32F, Scalar(0));
	float data[] = { 1,0,0,0,1,0,0,0,1 }; // Indentity matrix
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			_matrixTransform.at<float>(i, j) = data[3 * i + j];

}
AffineTransform::~AffineTransform(){}

//GeometricTransformer
int GeometricTransformer::Transform(const Mat& beforeImage, Mat& afterImage, AffineTransform* transformer, PixelInterpolate* interpolator)
{
	uchar* pSrc = beforeImage.data;
	int srcWidthStep = beforeImage.step[0] ;
	int srcChannels = beforeImage.channels();

	for (int i = 0; i < afterImage.rows; i++)
		for (int j = 0; j < afterImage.cols; j++)
		{
			float x = i; float y = j;
			// Spatial Transformation
			transformer->TransformPoint(x, y);
			
			// interpolation intensity
			if (x >= beforeImage.rows - 1 || y >= beforeImage.cols - 1 || x < 0 || y < 0)
			{
				afterImage.at<Vec3b>(i, j)[0] = 255;
				afterImage.at<Vec3b>(i, j)[1] = 255;
				afterImage.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				uchar* k;
				k = interpolator->Interpolate(x, y, pSrc, srcWidthStep, srcChannels);
				afterImage.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(int(k[0]));
				afterImage.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(int(k[1]));
				afterImage.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(int(k[2]));
			}
		}
	return 1;
}
int GeometricTransformer::RotateKeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (srcImage.data == NULL)
		return 0;
	int W = srcImage.cols; // W của ảnh gốc
	int H = srcImage.rows;// H của ảnh gốc
	float cosin = cos(angle * PI / 180.0);
	float sinh = sin(angle * PI / 180.0);
	// Tính toán bounding box cho ảnh mới xoay khi xoay góc angle
	// Công thức tham khảo: https:/ /stackoverflow.com/questions/53632184/rotate-image-in-bounds
	// W' = W |cos Θ| + H |sin Θ|, H' = W |sin Θ| + H |cos Θ|.
	int W_dst = (int)(W * abs(cosin) + H * abs(sinh));
	int H_dst = (int)(W * abs(sinh) + H * abs(cosin));
	dstImage = Mat(H_dst, W_dst, CV_8UC3);


	// Xây dựng phép affine transformer:
	AffineTransform* transformer = new AffineTransform();
	// Đưa ảnh srcImage về gốc tọa độ 
	transformer->Translate(-(H / 2), -(W / 2));
	// Xoay ảnh sourceImage 
	transformer->Rotate(angle);
	// Đưa ảnh src bằng phép translate về góc phần tư thể hiện ảnh 
	transformer->Translate(H_dst / 2, W_dst / 2);

	// Biến đổi ảnh
	GeometricTransformer g;
	g.Transform(srcImage, dstImage, transformer, interpolator);
	
	return 1;
}
int GeometricTransformer::RotateUnkeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (srcImage.data == NULL)
		return 0;
	dstImage = Mat(srcImage.rows, srcImage.cols, CV_8UC3);
	AffineTransform* transformer = new AffineTransform();
	// Dịch chuyển ảnh về tâm xoay
	transformer->Translate(-srcImage.rows / 2, -srcImage.cols / 2);
	// Xoay goc angle
	transformer->Rotate(angle);
	// Dịch chuyển ảnh về vị trí cũ
	transformer->Translate(srcImage.rows / 2, srcImage.cols / 2);
	GeometricTransformer g;
	g.Transform(srcImage, dstImage, transformer, interpolator);
	return 1;
}
int GeometricTransformer::Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator)
{
	if (srcImage.data == NULL)
		return 0;
	dstImage = Mat(sx * srcImage.rows, sy * srcImage.cols, CV_8UC3);
	AffineTransform* transformer = new AffineTransform();
	transformer->Scale(sx, sy);
	GeometricTransformer g;
	g.Transform(srcImage, dstImage, transformer, interpolator);
	return 1;
}
int GeometricTransformer::Resize(const Mat& srcImage, Mat& dstImage, int newWidth, int newHeight, PixelInterpolate* interpolator)
{
	/*https:// docs.opencv.org/2.4/modules/imgproc/doc/geometric_transformations.html#resize */
	float sx = (newHeight * 1.0) / srcImage.rows;
	float sy = (newWidth * 1.0) / srcImage.cols;

	dstImage = Mat(newHeight, newWidth, CV_8UC3);
	AffineTransform* transformer = new AffineTransform();
	transformer->Scale(sx, sy);
	GeometricTransformer g;
	g.Transform(srcImage, dstImage, transformer, interpolator);

	return 1;
}
int GeometricTransformer::Flip(const Mat& srcImage, Mat& dstImage, bool direction, PixelInterpolate* interpolator)
{
	if (direction == false)
	{
		dstImage = Mat(srcImage.rows, srcImage.cols, CV_8UC3);
		PixelInterpolate* interpolator = new NearestNeighborInterpolate();

		AffineTransform* transformer = new AffineTransform();
		transformer->Scale(1, -1); // Đối xứng qua trục Oy (trục dọc) vecto affine sẽ cho biến đổi tương ứng (x,y)->(x,-y)
		transformer->Translate(0, srcImage.cols - 1); // Dịch chuyển ảnh sau khi đối xứng về tâm 


		GeometricTransformer g;
		g.Transform(srcImage, dstImage, transformer, interpolator);
	}
	else
	{
		dstImage = Mat(srcImage.rows, srcImage.cols, CV_8UC3);
		AffineTransform* transformer = new AffineTransform();

		transformer->Scale(-1, 1); // Đối xứng qua trục Oy (trục ngang của ảnh) 
		transformer->Translate(srcImage.rows-1, 0); // Dịch chuyển về tâm


		GeometricTransformer g;
		g.Transform(srcImage, dstImage, transformer, interpolator);
	}
	return 1;
}
GeometricTransformer::GeometricTransformer(){}
GeometricTransformer::~GeometricTransformer(){}

