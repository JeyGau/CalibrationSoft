#include "camera.h"

#include "tools.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Camera::Camera(QObject *parent)
    : QObject{parent}
    , m_distortionCoeffs{}
    , m_intrinsicParams{}
    , m_positionInWorld{}
{

}

bool Camera::parseParametersFile(QString path)
{
    qInfo() << "Parsing camera parameters from path:" << path;

    // QFile can't read file with qrc prefix, remove if set
    if (path.startsWith(QLatin1String("qrc"))) {
        path.remove(QLatin1String("qrc"));
    }

    // open file
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qInfo() << "Parsing failed with error: couldn't open file";
        return false;
    }

    // read file
    QByteArray data = file.readAll();
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qInfo() << "Parsing failed with error:";
        qCritical() << error.errorString();
        return false;
    }

    // parse file
    const QJsonObject cameraParametersObj = doc.object();
    parseIntrinsicParameters(cameraParametersObj[QLatin1String("intrinsic")].toObject());
    parseDistortionCoeffs(cameraParametersObj[QLatin1String("distortion")].toObject());

    qInfo() << "Camera parameters parsed successfully";
    return true;
}

const cv::Mat &Camera::positionInWorld() const
{
    return m_positionInWorld;
}

void Camera::setPositionInWorld(const cv::Mat &newPositionInWorld)
{
    m_positionInWorld = newPositionInWorld;
}

bool Camera::parseIntrinsicParameters(const QJsonObject &obj)
{
    bool ok = true; // TO DO: return error if true

    float fx = obj[QLatin1String("fx")].toVariant().toFloat(&ok);
    float fy = obj[QLatin1String("fy")].toVariant().toFloat(&ok);
    float cx = obj[QLatin1String("cx")].toVariant().toFloat(&ok);
    float cy = obj[QLatin1String("cy")].toVariant().toFloat(&ok);

    m_intrinsicParams = (cv::Mat_<float>(3,3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);

    qInfo() << "Intrinsic parameters:" << Tools::toString(m_intrinsicParams);

    return ok;
}

bool Camera::parseDistortionCoeffs(const QJsonObject &obj)
{
    bool ok = true; // TO DO: return error if true

    float k1 = obj[QLatin1String("k1")].toVariant().toFloat(&ok);
    float k2 = obj[QLatin1String("k2")].toVariant().toFloat(&ok);
    float k3 = obj[QLatin1String("k3")].toVariant().toFloat(&ok);
    float k4 = obj[QLatin1String("k4")].toVariant().toFloat(&ok);
    float k5 = obj[QLatin1String("k5")].toVariant().toFloat(&ok);
    float k6 = obj[QLatin1String("k6")].toVariant().toFloat(&ok);
    float p1 = obj[QLatin1String("p1")].toVariant().toFloat(&ok);
    float p2 = obj[QLatin1String("p2")].toVariant().toFloat(&ok);

    m_distortionCoeffs = (cv::Mat_<float>(8,1) << k1, k2, p1, p2, k3, k4, k5, k6);

    qInfo() << "Distortion parameters:" << Tools::toString(m_distortionCoeffs);

    return ok;
}

const cv::Mat &Camera::intrinsicParams() const
{
    return m_intrinsicParams;
}

const cv::Mat &Camera::distortionCoeffs() const
{
    return m_distortionCoeffs;
}
