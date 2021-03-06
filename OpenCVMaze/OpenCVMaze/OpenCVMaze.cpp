// OpenCVMaze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

int main()
{
	for (int i = 1; i <= 4; i++) {
		cv::Mat image;
		switch (i)
		{
		case 1:
			image = cv::imread("maze1.png");
			break;
		case 2:
			image = cv::imread("maze2.png");
			break;
		case 3:
			image = cv::imread("maze3.png");
			break;
		case 4:
			image = cv::imread("maze4.png");
			break;
		default:
			return -1;
		}

		//afisarea imaginii initiale
		cv::namedWindow("Image", 0);
		cv::imshow("Image", image);

		//convertirea imaginii intr-o imagine binara
		cv::Mat gray, binary;
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
		cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY_INV);

		//detectarea contururilor in imagine
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
		cv::Mat path = cv::Mat::zeros(image.size(), CV_8UC1);
		cv::drawContours(path, contours, 0, CV_RGB(255, 255, 255), CV_FILLED);

		//calcularea latimii intrarii labirintului
		int colNr = 0, nrPx = 0;
		while (binary.at<uchar>(1, colNr) == 255) {
			colNr++;
		}
		while (binary.at<uchar>(1, colNr) == 0) {
			colNr++;
			nrPx++;
		}

		//dilatarea imaginii
		cv::Mat kernel = cv::Mat::ones(nrPx + 2, nrPx + 2, CV_8UC1);
		cv::dilate(path, path, kernel);

		//erodarea imaginii
		cv::Mat pathErode;
		cv::erode(path, pathErode, kernel);

		//extragerea imaginii erodate din imaginea dilatata
		cv::absdiff(path, pathErode, path);

		//colorarea caii de rezolvare a labirintului cu rosu
		std::vector<cv::Mat> channels;
		cv::split(image, channels);
		channels[0] &= ~path;
		channels[1] &= ~path;
		channels[2] |= path;

		//afisarea solutiei
		cv::Mat solution;
		cv::merge(channels, solution);
		cv::namedWindow("Solution", 0);
		cv::imshow("Solution", solution);
		cv::waitKey(0);
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
