#ifndef IDENTIFIEDOBJECT_H
#define IDENTIFIEDOBJECT_H

#include <QObject>

#include <QJsonObject>

#include <opencv2/opencv.hpp>
#include <vector>

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
