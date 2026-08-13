/*
    Shallow copy: 메모리 주소가 복사된다. 원본이 수정되면 shallow도 수정됨
    Deep copy: use clone(), 원본이 수정되어도 수정안됨.
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// int main() {
//     Mat m1 = (Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

//     Mat m_shallow = m1;
//     Mat m_deep = m1.clone();

//     cout << "m1 = " << m1 << endl;
//     cout << "m_shallow = " << m_shallow << endl;
//     cout << "m_deep " << m_deep << endl;

//     m1.at<double>(0, 0) = 100;

//     cout << "m1 = " << m1 << endl;
//     cout << "m_shallow = " << m_shallow << endl;
//     cout << "m_deep " << m_deep << endl;
// }

/*
    how to access at pixel
    image.at<data_type>(want_row, want_col)
    pointer
*/

int main() {
    Mat image, image_gray;
    int value, value_B, value_G, value_R, channels;

    image = imread("assets/lena.png");
    image_gray = imread("assets/lena.png", 0);

    channels = image.channels();

    switch (channels) {
    case 1:
        value = image.at<uchar>(50, 100);
        cout << "value" << value;
        break;

    case 3:
        value_B = image.at<Vec3b>(50, 100)[0];
        value_G = image.at<Vec3b>(50, 100)[1];
        value_R = image.at<Vec3b>(50, 100)[2];
        cout << "value at (100,50): " << value_B << " " << value_G << " " << value_R << endl;
        break;
    }

    uchar *p;
    p = image.ptr<uchar>(50);
    value_B = p[100 * channels + 0];
    value_G = p[100 * channels + 1];
    value_R = p[100 * channels + 2];
    cout << "value at (100,50): " << value_B << " " << value_G << " " << value_R << endl;

    return 0;
}