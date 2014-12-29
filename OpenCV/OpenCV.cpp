#include <iostream>
#include <cstdint>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

static inline int squared(int x)
{
	return x*x;
}

static void fillMat(Mat& src, int rr)
{
	int cr = src.rows / 2;
	int cc = src.cols / 2;

	int r2 = rr*rr;

	for (int r = 0; r != src.rows; ++r)
	{
		for (int c = 0; c != src.cols; ++c)
		{
			auto& color = src.at<uint8_t>(r, c);
			color = 0u;
			if (squared(r - cr) + squared(c - cc) < r2)
			{
				color = 255u;
			}
		}
	}
}

int main(int argc, char** argv)
{
	Mat src = Mat::zeros(256, 256, CV_8UC1);
	fillMat(src, 70);

	/*
	// the first command-line parameter must be a filename of the binary
	// (black-n-white) image
	if (argc != 2 || !(src = imread(argv[1], 0)).data)
		return -1;
	*/

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC1);

	// src = src > 1;
	namedWindow("Source", 1);
	imshow("Source", src);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0;
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, idx, color, 0 /* CV_FILLED */, 8, hierarchy);
	}

	namedWindow("Components", 1);
	imshow("Components", dst);
	waitKey(0);

	src.deallocate();
	dst.deallocate();
}

/*
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
*/
