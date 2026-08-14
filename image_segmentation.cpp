#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    /*
        이미지의 영역을 분할하여 더 쉽게 식별하기 위함
        Thresholding: 임계값을 활용하여 분할
        Otsu's meshod: threshold 값을 자동으로 골라주는 방법, 이미지의 히스토그램을 보고 물체가 가장
        잘 나뉘는 threshold를 자동으로 찾는 것이다.
        Local Threshold: 각 영역별로 사진을 잘라서 thresholding을 수행하는 것.
    */

    Mat image, thresh;
    int thresh_T, low_cnt, high_cnt, low_sum, high_sum, i, j, th;

    thresh_T = 200;
    th = 10;
    low_cnt = high_cnt = low_sum = high_sum = 0;

    image = imread("assets/lena.png", 0);
    cout << "threshold value: " << thresh_T << endl;

    while (1) {
        for (j = 0; j < image.rows; j++) {
            for (i = 0; i < image.cols; i++) {
                if (image.at<uchar>(j, i) < thresh_T) {
                    low_sum += image.at<uchar>(j, i);
                    low_cnt++;
                } else {
                    high_sum += image.at<uchar>(j, i);
                    high_cnt++;
                }
            }
        }

        if (abs(thresh_T - (low_sum / low_cnt + high_sum / high_cnt) / 2.0f) < th) {
            break;
        } else {
            thresh_T = (low_sum / low_cnt + high_sum / high_cnt) / 2.0f;
            cout << "threshold value: " << thresh_T << endl;
            low_cnt = high_cnt = low_sum = high_sum = 0;
        }
    }

    threshold(image, thresh, thresh_T, 255, THRESH_BINARY);

    imshow("thresholding", thresh);
    waitKey(0);
    return 0;
}