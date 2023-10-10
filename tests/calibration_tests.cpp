// exteral lib
#include <opencv2/opencv.hpp>

// Qt
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

// project
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
    void testSolvePnp();
    void testCalibration();

private:
    bool compareMatrices(const cv::Mat &mat1, const cv::Mat &mat2) const;

private:
    QJsonObject m_testCaseJson;

    Camera m_camera;
    IdentifiedObject m_paperSheet;

    struct ResultExpectations {
        const std::vector<cv::Point2f> imagePoints = {
            {745, 643},
            {1674, 688},
            {1768, 1074},
            {687, 1093}
        };


        const std::vector<cv::Point3f> objectPoints = {
            {0, 0, 0},
            {0, 297, 0},
            {210, 297, 0},
            {210, 0, 0}
        };

        const cv::Mat intrinsicParameters = (cv::Mat_<float>(3,3) <<
                                 3751.937744140625, 0, 1280,
                                 0, 3821.9775390625, 960,
                                 0, 0, 1);

        const cv::Mat distCoeffs = (cv::Mat_<float>(8,1) <<
                               -1.2093325853347778, -1.041786551475525, -0.004819112829864025, -0.0027203019708395004,
                               64.01393127441406, -0.5568028688430786, -3.6638600826263428, 78.03067016601562);

        const cv::Mat cameraPosition = (cv::Mat_<float>(3,1) <<
                                        1002.118316524654, 86.68933302340521, 630.5536086368008);

    } m_expectations ;
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

    QVERIFY(compareMatrices(cv::Mat(m_expectations.imagePoints), cv::Mat(m_paperSheet.points2D())));
    QVERIFY(compareMatrices(cv::Mat(m_expectations.objectPoints), cv::Mat(m_paperSheet.points3D())));
}

void TestCalibration::parseTestCameraParameters()
{
    QJsonObject intrisicParameters = m_testCaseJson[QLatin1String("intrinsic")].toObject();
    QVERIFY(!intrisicParameters.isEmpty());

    QVERIFY(m_camera.parseIntrinsicParameters(intrisicParameters));

    QVERIFY(compareMatrices(m_expectations.intrinsicParameters, m_camera.intrinsicParams()));
}

void TestCalibration::parseTestDistortionParameters()
{
    QJsonObject distortionParameters = m_testCaseJson[QLatin1String("distortion")].toObject();
    QVERIFY(!distortionParameters.isEmpty());

    QVERIFY(m_camera.parseDistortionCoeffs(distortionParameters));

    QVERIFY(compareMatrices(m_expectations.distCoeffs, m_camera.distortionCoeffs()));
}

void TestCalibration::testSolvePnp()
{
    // expectations
    cv::Mat rvec, tvec;
    cv::solvePnP(m_expectations.objectPoints,
                 m_expectations.imagePoints,
                 m_expectations.intrinsicParameters,
                 m_expectations.distCoeffs,
                 rvec,
                 tvec,
                 false,
                 cv::SOLVEPNP_IPPE);

    Processor p{&m_camera, &m_paperSheet};
    QVERIFY(p.estimatePose());

    QVERIFY(compareMatrices(tvec, p.transformation().translationVector));
    QVERIFY(compareMatrices(rvec, p.transformation().rotationVector));
}

void TestCalibration::testCalibration()
{
    // expectations
    cv::Mat rvec, tvec;
    cv::solvePnP(m_expectations.objectPoints,
                 m_expectations.imagePoints,
                 m_expectations.intrinsicParameters,
                 m_expectations.distCoeffs,
                 rvec,
                 tvec,
                 false,
                 cv::SOLVEPNP_IPPE);

    cv::Mat rmat;
    cv::Rodrigues(rvec, rmat);

    // Compute the inverse of the translation vector
    cv::Mat expectedCameraPosition = -rmat.t() * tvec;

    Processor p{&m_camera, &m_paperSheet};
    QVERIFY(p.calculateCameraPositionInWorld());

    cv::Mat expectedCameraPos64F;
    cv::Mat cameraPos64F;

    // convert to cv32f so we can compare them both
    m_expectations.cameraPosition.convertTo(expectedCameraPos64F, CV_32F);
    m_camera.positionInWorld().convertTo(cameraPos64F, CV_32F);

    QVERIFY(compareMatrices(expectedCameraPos64F, cameraPos64F));
}

bool TestCalibration::compareMatrices(const cv::Mat &mat1, const cv::Mat &mat2) const
{
    if (mat1.size != mat2.size) {
        qCritical() << "Matrices size mismatch" << mat1.size << mat2.size;
        return false;
    }

    for (int i = 0; i<mat1.rows; ++i) {
        for (int j = 0; j<mat1.cols; ++j) {
            if (!qFuzzyCompare(mat1.at<float>(i, j), mat2.at<float>(i, j))) {
                qCritical() << "Matrices differ at" << i << j;
                qInfo() << mat1.at<float>(i, j) << mat2.at<float>(i, j);
                return false;
            }
        }
    }
    return true;
}


QTEST_APPLESS_MAIN(TestCalibration)
#include "calibration_tests.moc"
