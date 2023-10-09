#ifndef IDENTIFIEDOBJECT_H
#define IDENTIFIEDOBJECT_H

#include <QObject>

#include <QJsonObject>

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief The IdentifiedObject class represents an object for which 3D coordinates and their 2D
 * projection coordinates are known. It aims at being used for camera calibration/pose estimating.
 *
 * PS: This class aims at being more generic but is currently only made for an A4 paper sheet
 */
class IdentifiedObject : public QObject
{
    Q_OBJECT
public:
    explicit IdentifiedObject(QObject *parent = nullptr);

public slots:
    void receiveCoordinates(QJsonObject coords);

private:
    std::vector<cv::Point2f> m_points2D;
    std::vector<cv::Point3f> m_points3D;
};

#endif // IDENTIFIEDOBJECT_H
