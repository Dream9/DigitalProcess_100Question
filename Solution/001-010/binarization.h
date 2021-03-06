#ifndef _SOLUTION_BINARIZATION_H_
#define _SOLUTION_BINARIZATION_H_

#include"Solution/solution.h"

#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

namespace digital {

//brief：二值化
class Binarization : public Solution {
public:
	Binarization(const string& path, bool flag, const string& name = "Binarization")
		:Solution(name, path, flag)
	{
		getDescriptionHandler().assign("以128作为阈值对彩色图像(8bit)二值化");
	}

	~Binarization() override{}

	void operator()() override{
		cv::Mat data = cv::imread(getPath(), 1);
		if (data.empty()) {
			dealException(kFileError);
			return;
		}

		int rows = data.rows;
		int cols = data.cols;

		cv::Mat img = cv::Mat::zeros(rows, cols, CV_8UC1);

#ifdef USE_OPENCVLIB
		//使用库函数
		cv::cvtColor(data, img, cv::COLOR_RGB2GRAY);
		cv::threshold(img, img, 128, 255, CV_THRESH_BINARY);
#else
		auto iter = img.data;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				auto ptr = data.ptr<cv::uint8_t>(i, j);
				*iter = static_cast<int>(0.11 * ptr[0] + 0.59 * ptr[1] + 0.3 * ptr[2]) > 128 ? 255 : 0;
				++iter;
			}
		}
#endif

		if (needShowOriginal())
			show(&data, &img);
		else
			show(&img);
	}
};

}//!namespace digital



#endif