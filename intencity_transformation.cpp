/*
    Image negatives: 색상 반전
    Log transformation: s = clog(1+r): log0은 정의가 되어있지 않기 때문에 1을 더해준다.
    => 낮은 밝기의 영역을 더 크게 확장시켜 쉽게 구분할 수 있게 하기 위함, 어두운 영역의 디테일을
   향상, 밝은 영역의 디테일은 감소

   Power-Law(Gamma) s = cr^r: 감마 값이 낮으면 어두운 영역의 디테일을 개선하는 효과
   감마 값이 높으면 밝은 영역의 디테일을 개선하는 효과
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// int main() {
//     Mat image = imread("assets/lena.png", 0);
//     Mat f_img, log_img;

//     double c = 1.5f;

//     image.convertTo(f_img, CV_32F);
//     f_img = abs(f_img) + 1;
//     log(f_img, f_img);
//     normalize(f_img, f_img, 0, 255, NORM_MINMAX);
//     // 로그 변환을 시켜준 값의 범위를 0~255 값으로 정규화

//     convertScaleAbs(f_img, log_img, c); // f_img * c = log_img
//     imshow("input image", image);
//     imshow("Log transformation", log_img);

//     waitKey(0);
//     return 0;
// }

int main() {
    Mat image = imread("assets/lena.png", 0);
    Mat gamma_img;
    MatIterator_<uchar> it, end;
    float gamma = 0.5;
    unsigned char pix[256];

    for (int i = 0; i < 256; i++) {
        pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
        // s = cr^r의 값이 너무 크기 때문에 255.0으로 나누어서 정규화르 시켜준 이후 255로 다시
        // 곱해준다.
    }
    gamma_img = image.clone();

    for (it = gamma_img.begin<uchar>(), end = gamma_img.end<uchar>(); it != end; it++) {
        *it = pix[(*it)];
    }

    imshow("Input image", image);
    imshow("Gamma transformation", gamma_img);
    waitKey(0);
    return 0;
}