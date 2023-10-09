#include <QTest>

#include <opencv2/opencv.hpp>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class TestCalibration: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void parseTestSheetPoints();
    void parseTestCameraParameters();
    void parseTestDistortionParameters();
    //    void loadTestImage() const;
    void testCalibration();

private:
    QJsonObject m_testCaseJson;

    std::vector<cv::Point3f> m_sheetPointsInSpace; // 3D coordinates of paper sheet
    std::vector<cv::Point2f> m_sheetPointsInImage; // 2D coordinates of paper sheet in jpg
    cv::Mat m_cameraMatrix, m_distCoeffs;
};

//void TestCalibration::loadTestImage() const
//{

//}


void TestCalibration::initTestCase()
{
    QFile file(":/tests/test.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray data = file.readAll();
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(data, &error);
    QCOMPARE(error.error, QJsonParseError::NoError);

    m_testCaseJson = doc.object();
    QVERIFY(!m_testCaseJson.isEmpty());

    file.close();
}

void TestCalibration::parseTestSheetPoints()
{
    QJsonArray objectPointsArray = m_testCaseJson[QLatin1String("object_points")].toArray();
    QVERIFY(!objectPointsArray.isEmpty());

    bool ok = false;
    for (const auto point: objectPointsArray) {
        float x = point.toObject()[QLatin1String("x")].toVariant().toFloat(&ok);
        QVERIFY(ok);
        float y = point.toObject()[QLatin1String("y")].toVariant().toFloat(&ok);
        QVERIFY(ok);
        float z = point.toObject()[QLatin1String("z")].toVariant().toFloat(&ok);
        QVERIFY(ok);
        m_sheetPointsInSpace.push_back(cv::Point3f{x, y, z});
    }

    QJsonArray imagePointsArray = m_testCaseJson[QLatin1String("image_points")].toArray();
    QVERIFY(!imagePointsArray.isEmpty());

    for (const auto point: imagePointsArray) {
        float x = point.toObject()[QLatin1String("x")].toVariant().toFloat(&ok);
        QVERIFY(ok);
        float y = point.toObject()[QLatin1String("y")].toVariant().toFloat(&ok);
        QVERIFY(ok);
        m_sheetPointsInImage.push_back(cv::Point2f{x, y});
    }

}

void TestCalibration::parseTestCameraParameters()
{
    bool ok = false;

    QJsonObject intrisicParameters = m_testCaseJson[QLatin1String("intrinsic")].toObject();
    QVERIFY(!intrisicParameters.isEmpty());
    float fx = intrisicParameters[QLatin1String("fx")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float fy = intrisicParameters[QLatin1String("fy")].toVariant().toFloat(&ok);;
    QVERIFY(ok);
    float cx = intrisicParameters[QLatin1String("cx")].toVariant().toFloat(&ok);;
    QVERIFY(ok);
    float cy = intrisicParameters[QLatin1String("cy")].toVariant().toFloat(&ok);;
    QVERIFY(ok);

    m_cameraMatrix = (cv::Mat_<float>(3,3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
}

void TestCalibration::parseTestDistortionParameters()
{
    bool ok = false;

    QJsonObject distortionParameters = m_testCaseJson[QLatin1String("distortion")].toObject();
    QVERIFY(!distortionParameters.isEmpty());
    float k1 = distortionParameters[QLatin1String("k1")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float k2 = distortionParameters[QLatin1String("k2")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float k3 = distortionParameters[QLatin1String("k3")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float k4 = distortionParameters[QLatin1String("k4")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float k5 = distortionParameters[QLatin1String("k5")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float k6 = distortionParameters[QLatin1String("k6")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float p1 = distortionParameters[QLatin1String("p1")].toVariant().toFloat(&ok);
    QVERIFY(ok);
    float p2 = distortionParameters[QLatin1String("p2")].toVariant().toFloat(&ok);
    QVERIFY(ok);

    m_distCoeffs = (cv::Mat_<float>(8,1) << k1, k2, p1, p2, k3, k4, k5, k6);
}

void TestCalibration::testCalibration()
{
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);

    bool success = cv::solvePnP(m_sheetPointsInSpace, m_sheetPointsInImage, m_cameraMatrix, m_distCoeffs, rvec, tvec, false, cv::SOLVEPNP_IPPE);

    QVERIFY(success);

    std::cout << "Translation vector: " << tvec << std::endl;
    std::cout << "Rotation vector: " << rvec << std::endl;

    // Convert rotation vector to rotation matrix
    cv::Mat R;
    cv::Rodrigues(rvec, R);

    // Calculate camera position in world coordinates
    cv::Mat cameraPosition = -R.inv() * tvec;

    // Print camera position
    std::cout << "Camera Position in World Coordinates: " << cameraPosition << std::endl;
}


QTEST_APPLESS_MAIN(TestCalibration)
#include "calibration_tests.moc"
