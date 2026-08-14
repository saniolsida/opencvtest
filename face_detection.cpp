#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xobjdetect.hpp>
using namespace std;
using namespace cv;

int main() {
    /*
        Harr-like feature: 영상의 특정 영역의 픽셀의 값을 더함, 그 차이가 해당 영역의 feature 값
        픽셀 하나하나를 보는게 아닌, 인접한 밝은 영역과 어두운 영역의 밝기 차이를 특징으로 본다.
        Adaboost: 어떤 영역이 효과적인지 찾는 방식
        이것을 학습한 결과가 Haar Cascade Classifier
        Weak learner: 약한 분류기
        Integral image: 현재 픽셀값 포함 좌상단의 모든 값을 더하여 계산함.
    */
    CascadeClassifier face_classifier;
    Mat frame, grayframe;
    vector<Rect> faces;
    int i;

    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cout << "Cound not open camera" << endl;
        return -1;
    }

    if (!face_classifier.load("assets/haarcascade_frontalface_alt.xml")) {
        cout << "Cascade load failed" << endl;
        return -1;
    }
    while (true) {
        cap >> frame;

        cvtColor(frame, grayframe, COLOR_BGR2GRAY);
        face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(30, 30));

        for (i = 0; i < faces.size(); i++) {
            Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            Point tr(faces[i].x, faces[i].y);
            rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
        }
        imshow("Face Detection", frame);
        if (waitKey(33) == 27)
            break;
    }
    return 0;
}