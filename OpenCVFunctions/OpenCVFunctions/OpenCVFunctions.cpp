
#include "pch.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>

void blur(cv::Mat img, int h, int w) {
	cv::Mat blurred, kernel = cv::Mat::ones(h, w, CV_32F) / (h*w);
	cv::filter2D(img, blurred, -1, kernel);
	cv::namedWindow("Blur", CV_WINDOW_AUTOSIZE);
	cv::imshow("Blur", blurred);
}

void blurGaussian(cv::Mat img, int h, int w, double sigma) {
	if (h % 2 == 0 || h < 0 || w % 2 == 0 || w < 0) {
		std::cout << "H si W trebuie sa fie impare si pozitive!" << std::endl;
		return;
	}
	cv::Mat blurred, kernel = cv::getGaussianKernel(h, sigma) * cv::getGaussianKernel(w, sigma).t();
	cv::filter2D(img, blurred, -1, kernel);
	cv::namedWindow("Gaussian", CV_WINDOW_AUTOSIZE);
	cv::imshow("Gaussian", blurred);
}

void blurMedian(cv::Mat img, int k_dim) {
	if (k_dim % 2 == 0) {
		std::cout << "k_dim trebuie sa fie impar!" << std::endl;
		return;
	}
	cv::Mat blurred = img.clone();
	for (int x = 0; x < blurred.rows; x++) {
		for (int y = 0; y < blurred.cols; y++) {
			if (img.channels() == 3) {
				std::vector<double> kernelB, kernelG, kernelR;
				for (int i = x - (k_dim / 2); i < x + (k_dim / 2) + 1; i++) {
					for (int j = y - (k_dim / 2); j < y + (k_dim / 2) + 1; j++) {
						cv::Vec3b pixelOld3b;
						if (i < 0 && j < 0) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i + (k_dim / 2), j + (k_dim / 2)));
						}
						else if (i >= blurred.rows && j >= blurred.cols) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i - (k_dim / 2), j - (k_dim / 2)));
						}
						else if (i < 0 && j >= blurred.cols) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i + (k_dim / 2), j - (k_dim / 2)));
						}
						else if (i >= blurred.rows && j < 0) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i - (k_dim / 2), j + (k_dim / 2)));
						}
						else if (i < 0) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i + (k_dim / 2), j));
						}
						else if (j < 0) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i, j + (k_dim / 2)));
						}
						else if (i >= blurred.rows) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i - (k_dim / 2), j));
						}
						else if (j >= blurred.cols) {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i, j - (k_dim / 2)));
						}
						else {
							pixelOld3b = img.at<cv::Vec3b>(cv::Point(i, j));
						}
						kernelB.push_back(pixelOld3b[0]);
						kernelG.push_back(pixelOld3b[1]);
						kernelR.push_back(pixelOld3b[2]);
					}
				}
				std::sort(std::begin(kernelB), std::end(kernelB));
				std::sort(std::begin(kernelG), std::end(kernelG));
				std::sort(std::begin(kernelR), std::end(kernelR));
				int medB = kernelB[kernelB.size() / 2], medG = kernelG[kernelG.size() / 2], medR = kernelR[kernelR.size() / 2];
				for (int i = x - (k_dim / 2); i < x + (k_dim / 2) + 1; i++) {
					for (int j = y - (k_dim / 2); j < y + (k_dim / 2) + 1; j++) {
						if (i >= 0 && i < blurred.rows && j >= 0 && j < blurred.cols) {
							cv::Vec3b pixelNew = blurred.at<cv::Vec3b>(cv::Point(i, j));
							pixelNew[0] = medB;
							pixelNew[1] = medG;
							pixelNew[2] = medR;
							blurred.at<cv::Vec3b>(cv::Point(i, j)) = pixelNew;
						}
					}
				}
			}
			else {
				std::vector<double> kernelGray;
				for (int i = x - (k_dim / 2); i < x + (k_dim / 2) + 1; i++) {
					for (int j = y - (k_dim / 2); j < y + (k_dim / 2) + 1; j++) {
						uchar pixelGray;
						if (i < 0 && j < 0) {
							pixelGray = img.at<uchar>(cv::Point(i + (k_dim / 2), j + (k_dim / 2)));
						}
						else if (i >= blurred.rows && j >= blurred.cols) {
							pixelGray = img.at<uchar>(cv::Point(i - (k_dim / 2), j - (k_dim / 2)));
						}
						else if (i < 0 && j >= blurred.cols) {
							pixelGray = img.at<uchar>(cv::Point(i + (k_dim / 2), j - (k_dim / 2)));
						}
						else if (i >= blurred.rows && j < 0) {
							pixelGray = img.at<uchar>(cv::Point(i - (k_dim / 2), j + (k_dim / 2)));
						}
						else if (i < 0) {
							pixelGray = img.at<uchar>(cv::Point(i + (k_dim / 2), j));
						}
						else if (j < 0) {
							pixelGray = img.at<uchar>(cv::Point(i, j + (k_dim / 2)));
						}
						else if (i >= blurred.rows) {
							pixelGray = img.at<uchar>(cv::Point(i - (k_dim / 2), j));
						}
						else if (j >= blurred.cols) {
							pixelGray = img.at<uchar>(cv::Point(i, j - (k_dim / 2)));
						}
						else {
							pixelGray = img.at<uchar>(cv::Point(i, j));
						}
						kernelGray.push_back(pixelGray);
					}
				}
				std::sort(std::begin(kernelGray), std::end(kernelGray));
				int medGray = kernelGray[kernelGray.size() / 2];
				for (int i = x - (k_dim / 2); i < x + (k_dim / 2) + 1; i++) {
					for (int j = y - (k_dim / 2); j < y + (k_dim / 2) + 1; j++) {
						if (i >= 0 && i < blurred.rows && j >= 0 && j < blurred.cols) {
							uchar pxG = blurred.at<uchar>(cv::Point(i, j));
							pxG = medGray;
							blurred.at<uchar>(cv::Point(i, j)) = pxG;
						}
					}
				}
			}
		}
	}
	cv::namedWindow("Median", CV_WINDOW_AUTOSIZE);
	cv::imshow("Median", blurred);
}

void sobel(cv::Mat img) {
	cv::Mat image;
	if (img.channels() == 3) {
		cv::cvtColor(img, image, cv::COLOR_BGR2GRAY);
	}
	else {
		image = img.clone();
	}
	cv::Mat sbl = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
	for (int y = 1; y < image.rows - 1; y++) {
		for (int x = 1; x < image.cols - 1; x++) {
			int gX = image.at<uchar>(y - 1, x - 1) + 2 * image.at<uchar>(y, x - 1) + image.at<uchar>(y + 1, x - 1) - image.at<uchar>(y - 1, x + 1) - 2 * image.at<uchar>(y, x + 1) - image.at<uchar>(y + 1, x + 1);
			int gY = image.at<uchar>(y - 1, x - 1) + 2 * image.at<uchar>(y - 1, x) + image.at<uchar>(y - 1, x + 1) - image.at<uchar>(y + 1, x - 1) - 2 * image.at<uchar>(y + 1, x) - image.at<uchar>(y + 1, x + 1);
			int sum = sqrt(gX * gX + gY * gY);
			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;
			sbl.at<uchar>(cv::Point(x, y)) = sum;
		}
	}
	cv::namedWindow("Sobel", CV_WINDOW_AUTOSIZE);
	cv::imshow("Sobel", sbl);
}

void binaryThreshold(cv::Mat img, int val) {
	cv::Mat threshold;
	if (img.channels() == 3) {
		cv::cvtColor(img, threshold, cv::COLOR_BGR2GRAY);
	}
	else {
		threshold = img.clone();
	}
	for (int x = 0; x < threshold.rows; x++) {
		for (int y = 0; y < threshold.cols; y++) {
			threshold.at<uchar>(cv::Point(x, y)) = threshold.at<uchar>(cv::Point(x, y)) > val ? 255 : 0;
		}
	}
	cv::namedWindow("Binary Threshold", CV_WINDOW_AUTOSIZE);
	cv::imshow("Binary Threshold", threshold);
}

void calculHist(cv::Mat img) {
	std::vector<cv::Mat> bgr_channels;
	split(img, bgr_channels);
	int channels_size = static_cast<int>(bgr_channels.size());
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	if (channels_size == 3) {
		cv::Mat b_hist, g_hist, r_hist;
		cv::calcHist(&bgr_channels[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_channels[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_channels[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::Mat histImageB(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
		cv::Mat histImageG(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
		cv::Mat histImageR(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
		normalize(b_hist, b_hist, 0, histImageB.rows, cv::NORM_MINMAX, -1, cv::Mat());
		normalize(g_hist, g_hist, 0, histImageG.rows, cv::NORM_MINMAX, -1, cv::Mat());
		normalize(r_hist, r_hist, 0, histImageR.rows, cv::NORM_MINMAX, -1, cv::Mat());
		for (int i = 1; i < histSize; i++)
		{
			line(histImageB, cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
				cv::Scalar(255, 0, 0), 2, 8, 0);
			line(histImageG, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
				cv::Scalar(0, 255, 0), 2, 8, 0);
			line(histImageR, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				cv::Scalar(0, 0, 255), 2, 8, 0);
		}
		cv::namedWindow("HistB", CV_WINDOW_AUTOSIZE);
		cv::imshow("HistB", histImageB);
		cv::namedWindow("HistG", CV_WINDOW_AUTOSIZE);
		cv::imshow("HistG", histImageG);
		cv::namedWindow("HistR", CV_WINDOW_AUTOSIZE);
		cv::imshow("HistR", histImageR);
	}
	else {
		cv::Mat gray_hist;
		cv::calcHist(&bgr_channels[0], 1, 0, cv::Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::Mat histImageGray(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
		normalize(gray_hist, gray_hist, 0, histImageGray.rows, cv::NORM_MINMAX, -1, cv::Mat());
		for (int i = 1; i < histSize; i++) {
			line(histImageGray, cv::Point(bin_w*(i - 1), hist_h - cvRound(gray_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(gray_hist.at<float>(i))),
				cv::Scalar(128, 128, 128), 2, 8, 0);
		}
		cv::namedWindow("HistGray", CV_WINDOW_AUTOSIZE);
		cv::imshow("HistGray", histImageGray);
	}
}

int main()
{
	cv::Mat img = cv::imread("lena.png");
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
	cv::namedWindow("Image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Image", img);
	blur(img, 11, 11);
	blurGaussian(img, 9, 9, 5);
	blurMedian(img, 3); //ineficient
	sobel(img);
	binaryThreshold(img, 128);
	calculHist(img);
	cv::waitKey(0);
}