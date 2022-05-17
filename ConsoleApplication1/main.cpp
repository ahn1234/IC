#include "Geometry.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
	CGeometry cg; //CGeometry 객체 생성

	Mat InputImage1 = imread("briefcase.jpg"); // 인풋 이미지 받기
	Mat OutputImage1 = cg.GS_affineRotate(InputImage1, -90, 1);  // 앵글 45도 돌리기
	imshow("OutputImage1", OutputImage1); //아웃풋이미지 출력

	waitKey(0); //프로그램이 종료되는 것을 방지함.
}