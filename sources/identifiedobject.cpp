#include "identifiedobject.h"

#include <QJsonArray>

#include "tools.h"

IdentifiedObject::IdentifiedObject(QObject *parent)
    : QObject{parent}
    , m_points2D()
    , m_points3D({cv::Point3f{0, 0, 0}, {cv::Point3f{0, 297, 0}}, {cv::Point3f{210, 297, 0}}, {cv::Point3f{210, 0, 0}}})
{

}

void IdentifiedObject::receiveCoordinates(QJsonObject coords)
{
    float ratio = coords["resolution_ratio"].toVariant().toFloat();
    const auto coordsArray = coords["coords_2D"].toArray();
    for(const auto &var : coordsArray) {
        auto pointMap = var.toObject();
        m_points2D.emplace_back(pointMap["x"].toVariant().toFloat() * ratio, pointMap["y"].toVariant().toFloat() * ratio);
        qInfo() << "Received coordinate: " << var;
    }
}
