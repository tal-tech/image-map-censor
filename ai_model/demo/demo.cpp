#include "det_body_ssd_resnet50.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace facethink;

int main(int argc, char *argv[]) {

	if (argc < 4) {
		std::cerr << "Usage: " << argv[0]
			<< " det_model"
			<< " image path"
			<< " config file path"
			<< std::endl;
		return 1;
	}

	const std::string det_model = argv[1];
	const std::string image_path = argv[2];
	const std::string config_file_path = argv[3];

	cv::Mat img = cv::imread(image_path); //test image
	if (img.data == 0) {
		std::cout << "read image failed:" << argv[2] << std::endl;
		return -1;
	}
	BodyDetectionRESNET50 *body_detector = BodyDetectionRESNET50::create(
		det_model,
		config_file_path);

	auto time_start = std::chrono::steady_clock::now();
	// body detection
	std::vector<cv::Rect> rectangles;
	std::vector<float> confidences;
	body_detector->detection(img, rectangles, confidences);
	auto time_end = std::chrono::steady_clock::now();
	double cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
	std::cout << cost_time << " ms," << std::endl;

	// draw result bboxes on image
	for (size_t i = 0; i < rectangles.size(); i++) {
		std::cout << "confidence: " << confidences[i] << std::endl;
		std::cout << "rectangle: " << rectangles[i] << std::endl;
		cv::rectangle(img, rectangles[i], cv::Scalar(0, 0, 255), 2);
	}
	cv::imwrite("out.jpg", img);

	delete body_detector;
	body_detector = nullptr;
	// std::getchar();
	return 0;
}
