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

    /**
     * @brief parseIntrinsicParameters parses intrinsic parameters from json
     * @param obj
     * @return
     */
    bool parseIntrinsicParameters(const QJsonObject &obj);
    /**
     * @brief parseDistortionCoeffs parses distortion parameters from json
     * @param obj
     * @return
     */
    bool parseDistortionCoeffs(const QJsonObject &obj);

    const cv::Mat &positionInWorld() const;
    void setPositionInWorld(const cv::Mat &newPositionInWorld);

public slots:
    /**
     * @brief parseParametersFile parses camera parameters from a json file
     * @param path
     * @return
     */
    bool parseParametersFile(QString path);

private:
    cv::Mat m_intrinsicParams;
    cv::Mat m_distortionCoeffs;

    cv::Mat m_positionInWorld;
};

#endif // CAMERA_H
