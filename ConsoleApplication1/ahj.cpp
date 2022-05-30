
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace cv;
using namespace std;

int main() {

	
	//내용//
	Mat img1 = imread("test.jpg");
	Mat img1_gray;
	Mat img1_otsu;

	resize(img1, img1, Size(600, 600));

	Mat img1_test = img1.clone(); //근사 안하고 바로 contour 적용
	cvtColor(img1, img1_gray, COLOR_BGR2GRAY);


	threshold(img1_gray, img1_otsu, 125, 255, THRESH_OTSU);

	//contours : 각 도형의 윤곽선을 표현하는 포인트 요소를 벡터로 표현
	//hierarchy : 4개 정수를 그 요소로 하는 벡터
	//conPoly : 윤곽선을 구성하는 데이터를 다각형 형태로 담기 위한 변수
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> conPoly(contours.size());

	vector<Point2f> approx;


	findContours(img1_otsu, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //threshold적용한 이미지에서 contours진행


	drawContours(img1_test, contours, -1, Scalar(0, 255, 0), 1); //근사 안하고 바로 적용(비교 예시를 위해)


	//입실론값  주어서 근사 작업을 진행, 수치를 바꾸면서 근사화 범위를 바꿀 수 있음
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.05, true);

		//어느정도 사이즈 크기가 있는 물체에서 진행
		if (fabs(contourArea(Mat(approx))) > 100)
		{
			int size = approx.size();

			//if (size % 2 == 0) {
			line(img1, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);//밑 가로

			for (int k = 0; k < size - 1; k++)
			{
				line(img1, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
			}
			//}
		}
	}

	//4 꼭지점의 좌표에다가 원을 표시
	for (int i = 0; i < approx.size(); i++) {
		circle(img1, approx[i], 20, Scalar(0, 255, 0), 5);
	}

	//x좌표의 크기가 최대 최소값, y좌표의 크기가 최대,최소값 찾기
	int max_x;
	int min_x;
	int min_y;
	int max_y;

	max_x = approx[0].x;
	min_x = approx[0].x;
	max_y = approx[0].y;
	min_y = approx[0].y;


	//각 좌표의 x or y좌표 크기 최대,최소값 찾는 과정
	for (int i = 0; i < approx.size(); i++) {
		if (approx[i].x < min_x)
			min_x = approx[i].x;
		if (approx[i].x > max_x)
			max_x = approx[i].x;
	}

	for (int i = 0; i < approx.size(); i++) {
		if (approx[i].y < min_y)
			min_y = approx[i].y;
		if (approx[i].y > max_y)
			max_y = approx[i].y;
	}


	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	vector<int> v4;
	vector<int> top_left;
	vector<int> top_right;
	vector<int> bottom_left;
	vector<int> bottom_right;



	//v1에 y 크기가 제일 작은 좌표를 대입
	for (int i = 0; i < approx.size(); i++) {
		if (approx[i].y == min_y) {
			v1.push_back(approx[i].x);
			v1.push_back(approx[i].y);
		}
	}
	//v2에 y 크기가 제일 큰 좌표를 대입
	for (int i = 0; i < approx.size(); i++) {
		if (approx[i].y == max_y) {
			v2.push_back(approx[i].x);
			v2.push_back(approx[i].y);
		}
	}


	//v3에 x크기가 제일 작은 좌표를 대입
	for (int i = 0; i < approx.size(); i++) {
		if (approx[i].x == min_x) {
			v3.push_back(approx[i].x);
			v3.push_back(approx[i].y);
		}
	}
	//v4에 x크기가 제일 큰 좌표를 대입
	for (int i = 0; i < approx.size(); i++) {
		if (approx[i].x == max_x) {
			v4.push_back(approx[i].x);
			v4.push_back(approx[i].y);
		}
	}




	//두가지 case에 대해 적용을 하면 됨(왼쪽으로 기울어진 경우, 오른쪽으로 기울어진 경우)
	if (v1[0] > v2[0]) {// 명함이 '\(역슬래쉬)'방향으로 기울어진 경우
		top_right.push_back(v1[0]);
		top_right.push_back(v1[1]);
		bottom_left.push_back(v2[0]);
		bottom_left.push_back(v2[1]);
		top_left.push_back(v3[0]);
		top_left.push_back(v3[1]);
		bottom_right.push_back(v4[0]);
		bottom_right.push_back(v4[1]);
	}
	else // 명함이 '/'방향으로 기울어진 경우
	{
		top_left.push_back(v1[0]);
		top_left.push_back(v1[1]);
		bottom_right.push_back(v2[0]);
		bottom_right.push_back(v2[1]);
		bottom_left.push_back(v3[0]);
		bottom_left.push_back(v3[1]);
		top_right.push_back(v4[0]);
		top_right.push_back(v4[1]);
	}



	cout << "top_left[0] : " << top_left[0] << endl;
	cout << "top_left[1] : " << top_left[1] << endl;

	cout << "top_right[0] : " << top_right[0] << endl;
	cout << "top_right[1] : " << top_right[1] << endl;

	cout << "bottom_left[0] : " << bottom_left[0] << endl;
	cout << "bottom_left[1] : " << bottom_left[1] << endl;

	cout << "bottom_right[0] : " << bottom_right[0] << endl;
	cout << "bottom_right[1] : " << bottom_right[1] << endl;


	int w1, w2, h1, h2, minwidth, minheight;

	w1 = abs(bottom_right[0] - bottom_left[0]);
	w2 = abs(top_right[0] - top_left[0]);
	h1 = abs(top_right[1] - bottom_right[1]);
	h2 = abs(top_left[1] - bottom_left[1]);

	minwidth = min(w1, w2);
	minheight = min(h1, h2);


	Point2f src_p[4], dst_p[4];

	src_p[0] = Point2f(top_left[0], top_left[1]);
	src_p[1] = Point2f(top_right[0], top_right[1]);
	src_p[2] = Point2f(bottom_right[0], bottom_right[1]);
	src_p[3] = Point2f(bottom_left[0], bottom_left[1]);


	dst_p[0] = Point2f(0, 0);
	dst_p[1] = Point2f(minwidth, 0);
	dst_p[2] = Point2f(minwidth, minheight);
	dst_p[3] = Point2f(0, minheight);

	Mat perspect_mat = getPerspectiveTransform(src_p, dst_p);

	cout << "행렬 값 : " << perspect_mat << endl;


	Mat dst;

	warpPerspective(img1, dst, perspect_mat, Size(minwidth, minheight));

	imshow("img1_gray", img1_gray);
	imshow("img1_thread", img1_otsu);
	imshow("contour", img1_test);
	imshow("approx", img1);
	imshow("dst", dst);
	waitKey(0);

	return 0;
}

