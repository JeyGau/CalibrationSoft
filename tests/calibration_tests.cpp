#include <QTest>

#include <opencv2/opencv.hpp>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class TestCalibration: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
//    void loadTestImage() const;
//    void testExifExtractor();
    void testCalibration();

private:
    QJsonObject m_cameraParameters;
};

//void TestCalibration::loadTestImage() const
//{

//}

//void TestCalibration::testExifExtractor()
//{
//    QFile file(":/assets/calibration.jpg");
//    QVERIFY(file.open(QIODevice::ReadOnly));  // Ensure the file is opened

//    QByteArray data = file.readAll();
//    easyexif::EXIFInfo result;
//    int code = result.parseFrom(reinterpret_cast<unsigned char*>(data.data()), data.size());
//    QCOMPARE(code, 0);

//    // Print and check specific EXIF data
//    qDebug() << "Camera make:" << result.Make.c_str();
//    qDebug() << "Camera model:" << result.Model.c_str();

//    QVERIFY(!result.Make.empty());
//    QVERIFY(!result.Model.empty());
//}

void TestCalibration::initTestCase()
{
    QFile file(":/assets/camera_parameters.json");
    QVERIFY(file.open(QIODevice::ReadOnly));

    QByteArray data = file.readAll();
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(data, &error);
    QCOMPARE(error.error, QJsonParseError::NoError);

    m_cameraParameters = doc.object();
    QVERIFY(!m_cameraParameters.isEmpty());

    qDebug().noquote() << doc.toJson();
}

void TestCalibration::testCalibration()
{
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    int flags = 0;
    cv::Size imageSize(640, 480);

    double error = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);

    QVERIFY(error < 1);
}


QTEST_APPLESS_MAIN(TestCalibration)
#include "calibration_tests.moc"
