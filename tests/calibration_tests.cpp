#include <QTest>

#include <opencv2/opencv.hpp>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "camera.h"
#include "identifiedobject.h"
#include "processor.h"

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
    Processor p{&m_camera, &m_paperSheet};
    QVERIFY(p.estimatePose());

    // Convert rotation vector to rotation matrix
    cv::Mat R;
    cv::Rodrigues(p.transformation().rotationVector, R);

    // Calculate camera position in world coordinates
    cv::Mat cameraPosition = -R.inv() * p.transformation().translationVector;
    // Print camera position
    std::cout << "Camera Position in World Coordinates: " << cameraPosition << std::endl;
    cv::Mat expectedPosition = (cv::Mat_<double>(3,1) <<
                                    968.8691370130923,
                                    135.6954993047486,
                                    677.3111399332236);
    for (int i = 0; i < 3; ++i)
    {
        // compare with a tolerance
        QCOMPARE(qFuzzyCompare(cameraPosition.at<double>(i, 0), expectedPosition.at<double>(i, 0)), true);
    }
}


QTEST_APPLESS_MAIN(TestCalibration)
#include "calibration_tests.moc"
