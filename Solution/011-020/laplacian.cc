#include"Solution/011-020/laplacian.h"
#include"Solution/base.h"
#include"Solution/grayscale_transfrom.h"

#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

namespace digital {

//brief:拉普拉斯算子并没有对图像做平滑处理，其对细节/噪声的响应较大
//      LoG会在拉普拉斯算子基础上加上高斯平滑
void Laplacian::operator()() {
	cv::Mat data = cv::imread(getPath());
	if (data.empty()) {
		dealException(kFileError);
		return;
	}
	cv::cvtColor(data, data, cv::COLOR_RGB2GRAY);
	cv::GaussianBlur(data, data, cv::Size(5,5), 0);//依然进行了平滑处理
	cv::Mat img;

#ifdef USE_OPENCVLIB

	cv::Laplacian(data, img, CV_16S);

#else

	laplacian_(&data, &img, CV_16S);

#endif

	cv::convertScaleAbs(img, img);//, 1.0, 0.0);

	//颜色反转
	detail::colorInversion(img);

	if (needShowOriginal())
		show(&data, &img);
	else
		show(&img);
}

//brief:拉普拉斯算子是不可分离的
void Laplacian::laplacian_(void* src, void* dst, int ddepth, int bordertype) {
	cv::Mat& data = *static_cast<cv::Mat*>(src);
	cv::Mat& img = *static_cast<cv::Mat*>(dst);

	cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
		0, 1, 0,
		1, -4, 1,
		0, 1, 0);

	detail::convolution2D(data, img, ddepth, kernel, cv::Point(-1, -1), bordertype);
}

}