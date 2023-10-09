#include <QTest>

#include <opencv2/opencv.hpp>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "camera.h"
#include "identifiedobject.h"

class TestCalibration: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void parseTestSheetPoints();
    void parseTestCameraParameters();
    void parseTestDistortionParameters();
    void testCalibration();

private:
    QJsonObject m_testCaseJson;

    Camera m_camera;
    IdentifiedObject m_paperSheet;
};

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
    QJsonArray imagePointsArray = m_testCaseJson[QLatin1String("image_points")].toArray();
    QVERIFY(!imagePointsArray.isEmpty());

    QVERIFY(m_paperSheet.parseCoordinates(imagePointsArray));
}

void TestCalibration::parseTestCameraParameters()
{
    QJsonObject intrisicParameters = m_testCaseJson[QLatin1String("intrinsic")].toObject();
    QVERIFY(!intrisicParameters.isEmpty());

    QVERIFY(m_camera.parseIntrinsicParameters(intrisicParameters));
}

void TestCalibration::parseTestDistortionParameters()
{
    QJsonObject distortionParameters = m_testCaseJson[QLatin1String("distortion")].toObject();
    QVERIFY(!distortionParameters.isEmpty());

    QVERIFY(m_camera.parseDistortionCoeffs(distortionParameters));
}

void TestCalibration::testCalibration()
{
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64F);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64F);

    bool success = cv::solvePnP(m_paperSheet.points3D(), m_paperSheet.points2D(), m_camera.intrinsicParams(), m_camera.distortionCoeffs(), rvec, tvec, false, cv::SOLVEPNP_IPPE);

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
