#include "pch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

int main()
{
	cv::Mat image, gray, blur, binary;
	image = cv::imread("shapes.png");
	cv::namedWindow("Before", 0);
	cv::imshow("Before", image);
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);
	cv::threshold(blur, binary, 60, 255, cv::THRESH_BINARY);
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
 	for (int i = 0; i < contours.size(); i++) {
		double perimeter = cv::arcLength(contours.at(i), true);
		std::vector<cv::Point> approx;
		cv::approxPolyDP(contours.at(i), approx, 0.04 * perimeter, true);
		if (approx.size() == 3) {
			cv::drawContours(image, contours, i, cv::Scalar(255, 0, 0), 5);
		}
		else if (approx.size() == 4) {
			cv::Rect rect = cv::boundingRect(approx);
			cv::RotatedRect rotRect = cv::fitEllipse(contours.at(i));
			if ((rect.width / (float)rect.height) == 1) {
				cv::drawContours(image, contours, i, cv::Scalar(0, 255, 0), 5);
			}
			else {
				if (rotRect.angle == 0 || rotRect.angle == 90) {
					cv::drawContours(image, contours, i, cv::Scalar(0, 128, 0), 5);
				}
				else if (rotRect.angle < 90) {
					cv::drawContours(image, contours, i, cv::Scalar(0, 255, 255), 5);
				}
				else {
					cv::drawContours(image, contours, i, cv::Scalar(0, 0, 255), 5);
				}
			}
		}
		else if (approx.size() == 5) {
			cv::drawContours(image, contours, i, cv::Scalar(255, 0, 255), 5);
		}
		else {
			cv::drawContours(image, contours, i, cv::Scalar(0, 0, 255), 5);
		}
 	}
	cv::namedWindow("After", 0);
	cv::imshow("After", image);
 	cv::waitKey(0);
}