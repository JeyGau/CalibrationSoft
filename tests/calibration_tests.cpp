#include <QTest>
#include <opencv2/opencv.hpp>

class TestCalibration: public QObject
{
    Q_OBJECT
private slots:
    void loadTestImage() const;
    void testCalibration();
};

void TestCalibration::loadTestImage() const
{

}

void TestCalibration::testCalibration()
{
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    int flags = 0;
    cv::Size imageSize(640, 480); // Remplace par la taille réelle de tes images

    double error = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);

    QVERIFY(error < 1);
}

QTEST_MAIN(TestCalibration)
#include "calibration_tests.moc"
