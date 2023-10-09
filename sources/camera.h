#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <opencv2/opencv.hpp>

/**
 * @brief The Camera class parses a json file into a parameters matrices:
 * - intrinsic parameters matrix
 * - distortion coeffs matrix
 */
class Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(QObject *parent = nullptr);

    const cv::Mat &distortionCoeffs() const;
    const cv::Mat &intrinsicParams() const;

    bool parseIntrinsicParameters(const QJsonObject &obj);
    bool parseDistortionCoeffs(const QJsonObject &obj);

    const cv::Mat &positionInWorld() const;
    void setPositionInWorld(const cv::Mat &newPositionInWorld);

public slots:
    bool parseParametersFile(QString path);

private:
    cv::Mat m_intrinsicParams;
    cv::Mat m_distortionCoeffs;

    cv::Mat m_positionInWorld;
};

#endif // CAMERA_H
