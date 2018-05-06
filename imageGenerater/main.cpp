#include <opencv2/opencv.hpp>

#define CV_LIB_PREFIX comment(lib, "opencv_"
#define CV_LIB_VERSION CVAUX_STR(CV_MAJOR_VERSION)\
        CVAUX_STR(CV_MINOR_VERSION)\
        CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
#define CV_LIB_SUFFIX CV_LIB_VERSION "d.lib")
#else
#define CV_LIB_SUFFIX CV_LIB_VERSION ".lib")
#endif
#define CV_LIBRARY(lib_name) CV_LIB_PREFIX CVAUX_STR(lib_name) CV_LIB_SUFFIX
#pragma CV_LIBRARY(core)
#pragma CV_LIBRARY(highgui)
#pragma CV_LIBRARY(imgcodecs)
#pragma CV_LIBRARY(imgproc)
#pragma CV_LIBRARY(photo)

using namespace cv;
using namespace std;

struct EmbedInfo
{
	Mat mask;
	Point center;
};

void embedImage(const cv::Mat src, cv::Mat& dest, const cv::Mat embeddedImage, const double angle, const double tx, const double ty)
{
	CV_Assert(!src.empty() && !embeddedImage.empty());

	src.copyTo(dest);

	if (angle != 0)
	{
		Point2d center(embeddedImage.cols / 2, embeddedImage.rows / 2);
		Mat rotateMat = cv::getRotationMatrix2D(center, angle, 1.0);
		rotateMat.at<double>(0, 2) += tx;
		rotateMat.at<double>(1, 2) += ty;
		warpAffine(embeddedImage, dest, rotateMat, dest.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
	}
	else
	{
		embeddedImage.copyTo(dest(Rect(tx, ty, embeddedImage.cols, embeddedImage.rows)));
	}
}

void cloningImage(const cv::Mat src, cv::Mat& dest, const cv::Mat embeddedImage, const double angle, const double tx, const double ty)
{
	CV_Assert(!src.empty() && !embeddedImage.empty());

	src.copyTo(dest);
	Point2d center(embeddedImage.cols / 2, embeddedImage.rows / 2);
	Mat rotateMat = cv::getRotationMatrix2D(center, angle, 1.0);
	rotateMat.at<double>(0, 2) += tx;
	rotateMat.at<double>(1, 2) += ty;
	warpAffine(embeddedImage, dest, rotateMat, dest.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);

}

void generateImage(const Mat src, Mat& dest, const Mat embeddedImage, bool isSeamless = false)
{
	CV_Assert(!src.empty() && !embeddedImage.empty());

	const int angle = 0;
	//const int angle = (rand() + 1.f) / (RAND_MAX + 2.f) * 360;
	const int tx = (rand() + 1.f) / (RAND_MAX + 2.f) * (src.cols - embeddedImage.cols * 2) + embeddedImage.cols;
	const int ty = (rand() + 1.f) / (RAND_MAX + 2.f) * (src.rows - embeddedImage.rows * 2) + embeddedImage.rows;

	//EmbedInfo ret;
	//ret.center = Point(tx + embeddedImage.cols, ty + embeddedImage.rows);
	//ret.mask = Mat::zeros(src.rows, src.cols, CV_8UC1);
	//Point pts[] = {
	//	Point(tx, ty),
	//	Point(tx, ty + embeddedImage.rows),
	//	Point(tx + embeddedImage.cols, ty + embeddedImage.rows),
	//	Point(tx + embeddedImage.cols, ty),
	//};
	//fillConvexPoly(ret.mask, pts, 4, Scalar(255));

	Mat clone;
	if (!isSeamless)
	{
		embedImage(src, dest, embeddedImage, angle, tx, ty);
	}
	else
	{
		const Mat src_mask = 255 * Mat::ones(embeddedImage.rows, embeddedImage.cols, embeddedImage.depth());
		const Point center(tx, ty);
		//seamlessClone(embeddedImage, src, src_mask, center, dest, NORMAL_CLONE);
		seamlessClone(embeddedImage, src, src_mask, center, dest, MIXED_CLONE);
	}
}

int main(int argc, char** argv)
{
	srand(time(nullptr));
	string sname = "beach";

	//string tname = "ocean_beach_kinzokutanchi";
	//string tname = "mokuzai_hakobu";
	//string tname = "kids_chuunibyou_girl";
	string tname = "airgun_women_syufu";

	Mat src = imread("img/" + sname + ".png");
	Mat embededImage = imread("template/" + tname + ".png", IMREAD_ANYCOLOR);

	resize(embededImage, embededImage, Size(64, embededImage.rows * 64 / embededImage.cols));

	Mat dest;
	while (true)
	{
		//generateImage(src, dest, embededImage, true);
		generateImage(src, dest, embededImage);
		imshow("create image", dest);
		imwrite(sname + ".ppm", dest);

		int key = waitKey();
		if (key == 'q')
		{
			break;
		}
	}
	imwrite(tname + ".ppm", embededImage);
}