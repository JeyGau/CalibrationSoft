#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <opencv2/opencv.hpp>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

public slots:
    void parseParametersFile(const QString &path);

signals:

private:
    struct CameraParameters {
        cv::Mat intrinsicParams;
        cv::Mat distortionCoeffs;
    } m_cameraParameters;
};

#endif // CAMERA_H
