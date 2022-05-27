#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool compareContourAreas(vector<Point> contour1, vector<Point> contour2) {
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i > j);
}
//struct str {
//    bool operator() (Point2f a, Point2f b) {
//        if (a.y != b.y)
//            return a.y < b.y;
//        return a.x <= b.x;
//    }
//} comp;
int main() {
    Mat image = imread("identityCard1.jpg", 1);
    Mat original;
    Mat BoxCard;
    Mat Expansion;
    float width = image.size().width;
    float height = image.size().height;
    image.copyTo(BoxCard);
    image.copyTo(original);

    //기본 이미지 출력
    imshow("Original", original);
    waitKey();
    destroyAllWindows();

    Mat gray;
    Mat blur;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0);

    //gray이미지와 blur이미지 출력
    imshow("grayImage", gray);
    imshow("blurImage", blur);
    waitKey();
    destroyAllWindows();


    Mat edge;
    Canny(blur, edge, 100, 200);
    //Edge 출력
    imshow("CannyEdge", edge);
    waitKey();
    destroyAllWindows();

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    int mode = RETR_CCOMP;
    int module = CHAIN_APPROX_TC89_KCOS;


    findContours(edge, contours, hierarchy, mode, module);

    sort(contours.begin(), contours.end(), compareContourAreas);
    drawContours(image, contours, -1, Scalar(255, 0, 0), 1);

    //경계선 출력
    imshow("ContourImage", image);
    waitKey();
    destroyAllWindows();

    vector<Point> contours_approx;

    for (int i = 0; i < 5; i++) {
        double length = arcLength(contours[i], true);
        approxPolyDP(contours[i], contours_approx, 0.1 * length, true);

        if (contours_approx.size() == 4) {
            break;
        }
        contours_approx.clear();
    }
    Point2f src_p[4], dst_p[4];
    int j = 0;
    for (Point i : contours_approx) {
        src_p[j] = i;
        j++;
    }
    //sort(src_p, src_p + 4, comp);

    for (int i = 0; i < 4; i++)
    {
        cout << src_p[i] << endl;
    }
    Point2f temp;
    temp = src_p[2];
    src_p[2] = src_p[3];
    src_p[3] = temp;

    dst_p[0] = Point2f(0.0f, 0.0f);
    dst_p[1] = Point2f(0.0f, height);
    dst_p[2] = Point2f(width, 0.0f);
    dst_p[3] = Point2f(width, height);

    Mat mapMatrix = getPerspectiveTransform(src_p, dst_p);
    warpPerspective(original, Expansion, mapMatrix, Size(1100,950));
    //가장큰 사각형 테두리 출력
    drawContours(BoxCard, vector<vector<Point>>(1, contours_approx), -1, Scalar(0, 0, 255), 1);
    imshow("BoxCard", BoxCard);
    imshow("Expansion", Expansion);
    waitKey();
    destroyAllWindows();

    return 0;
}