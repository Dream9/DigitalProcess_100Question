#include"Solution/031-040/frequency_domain_filter_high_pass.h"
#include"Solution/fourier_transform.h"

#include<opencv2/highgui.hpp>

namespace digital {
//brief:Ƶ�����ͨ�˲�������ʵ��λ��fourier.[h/cc]
void FrequencyDomainFilterHighPass::operator()() {
	static_assert(IHPF == detail::IHPF, "compatibility error");
	static_assert(BHPF == detail::BHPF, "compatibility error");
	static_assert(GHPF == detail::GHPF, "compatibility error");

	cv::Mat data = cv::imread(getPath(), cv::IMREAD_GRAYSCALE);
	if (data.empty()) {
		dealException(kFileError);
		return;
	}
	cv::Mat data_64f;
	cv::Mat fft_data;
	cv::Mat lp_fft_data;
	cv::Mat img;
	string title;

	data.convertTo(data_64f, CV_64F);
	data_64f = detail::centralize(data_64f);
	detail::dft(data_64f, fft_data, detail::DFT_COMPLEX_OUTPUT);

	//becare:����������������ֻ�в��ð�����˹ʱ���ڶ��������ŷ�������
	double threshold[] = { 5,2 };


	detail::frequencyDomainFilter(fft_data, lp_fft_data, op_, &threshold);

	if (op_ == IHPF) {
		title = "�����ͨ";
	}
	else if (op_ == BHPF) {
		title = "������˹��ͨ";
	}
	else if (op_ == GHPF) {
		title = "��˹��ͨ";
	}
	else {
		dealException(kFatal);
		return;
	}


	cv::Mat spectrum;
	detail::getAmplitudeSpectrum(lp_fft_data, spectrum);
	cv::Mat out = detail::grayscaleAmplitudeSpctrum(spectrum);

	if (needShowOriginal())
		show(&data, &out, "Amplitude Spectrum");
	else
		show(&out, "Amplitude Spectrum");

	detail::fft(lp_fft_data, data_64f, detail::DFT_INVERSE | detail::DFT_REAL_OUTPUT | detail::DFT_SCALE);
	data_64f = data_64f(cv::Rect(0, 0, data.cols, data.rows));//��ֹfft�ĸ�����
	data_64f = detail::centralize(data_64f);
	data_64f.convertTo(img, CV_8U);

	if (needShowOriginal())
		show(&data, &img, title);
	else
		show(&img, title);
}


}//!namespace digital