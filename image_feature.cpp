#include <iostream>
#include <opencv2/geometry/2d.hpp>
#include <opencv2/geometry/3d.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() {

    /*
        Image feature: corner, edge, texture, CNN이 찾아낸 복잡한 패턴
        필요한 이유: 컴퓨터는 이미지를 판단하지 못함. 물체 안의 특징점을 골라서 추적
        Good feature: 적은 계산량, Corner, 회전이나 변화에 강해야함, 주변과 구별되어야 함

        ORB oFAST detector + rotated brief descriptor
        FAST: 특정 픽셀의 원 주위로 연속으로 9개 연속으로 밝기가 크다면 선택됨
        BRIEF: feature를 묘사하는 방법, descriptor로 표현
        NNDR: distance to best match / distance to second best match 작을 수록 좋다.
    */
    Mat query, image, descriptor1, descriptor2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoint1, keypoint2;
    vector<vector<DMatch>> matches;
    vector<DMatch> goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches, H;
    vector<Point2f> obj;
    vector<Point2f> scene;
    vector<Point2f> objP(4);
    vector<Point2f> sceneP(4);
    int i, k;
    float nndr, Ratio;

    query = imread("assets/query.png");
    image = imread("assets/input.png");

    if (query.empty() || image.empty())
        return -1;

    resize(image, image, Size(640, 480));
    orbF->detectAndCompute(query, noArray(), keypoint1, descriptor1);
    orbF->detectAndCompute(image, noArray(), keypoint2, descriptor2);

    k = 2;
    matcher.knnMatch(descriptor1, descriptor2, matches, k);

    nndr = 0.6f;
    for (i = 0; i < matches.size(); i++) {
        if (matches.at(i).size() == 2 &&
            matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
            goodMatches.push_back(matches[i][0]);
        }
    }

    drawMatches(query, keypoint1, image, keypoint2, goodMatches, imgMatches, Scalar::all(-1),
                Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    if (goodMatches.size() < 4) {
        cout << "Matching failed" << endl;
        return 0;
    }

    for (i = 0; i < goodMatches.size(); i++) {
        obj.push_back(keypoint1[goodMatches[i].queryIdx].pt);
        scene.push_back(keypoint2[goodMatches[i].trainIdx].pt);
    }

    H = findHomography(obj, scene, RANSAC);
    objP[0] = Point2f(0, 0);
    objP[1] = Point2f(query.cols, 0);
    objP[2] = Point2f(query.cols, query.rows);
    objP[3] = Point2f(0, query.rows);

    perspectiveTransform(objP, sceneP, H);

    for (i = 0; i < 4; i++)
        sceneP[i] += Point2f(query.cols, 0);
    for (i = 0; i < 4; i++)
        line(imgMatches, sceneP[i], sceneP[(i + 1) % 4], Scalar(255, 0, 0), 4);

    imshow("imgMatches", imgMatches);
    waitKey(0);

    return 0;
}