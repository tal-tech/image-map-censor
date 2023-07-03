#include "det_body_ssd_resnet50.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace facethink;

int main(int argc, char *argv[]) {

	const std::string det_model = "/home/guoweiye/workspace/image-map-censor/src/ai_model/model/det_body_ssd_mobilenetv1_v2.0.1.uff";
	const std::string image_path = "/home/guoweiye/workspace/image-map-censor/src/ai_model/images/testing/JPEGImages/1.jpeg";
	const std::string config_file_path = "/home/guoweiye/workspace/image-map-censor/src/ai_model/model/config.ini";

	cv::Mat img = cv::imread(image_path); //test image
	if (img.data == 0) {
		std::cout << "read image failed:"<< std::endl;
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
	return 0;
}
