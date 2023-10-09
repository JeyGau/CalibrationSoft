#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <opencv2/opencv.hpp>

/**
 * @brief The Camera class parses a json file into a struct of parameters matrices:
 * - intrinsic parameters matrix
 * - distortion coeffs matrix
 */
class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

public slots:
    void parseParametersFile(QString path);

private:
    bool parseIntrinsicParameters(const QJsonObject &obj);
    bool parseDistortionCoeffs(const QJsonObject &obj);

signals:

private:
    struct CameraParameters {
        cv::Mat intrinsicParams;
        cv::Mat distortionCoeffs;
    } m_cameraParameters;
};

#endif // CAMERA_H
