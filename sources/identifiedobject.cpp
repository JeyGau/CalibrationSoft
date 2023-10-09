#include "identifiedobject.h"

#include <QJsonArray>

#include "tools.h"

IdentifiedObject::IdentifiedObject(QObject *parent)
    : QObject{parent}
    , m_points2D()
    , m_points3D({cv::Point3f{0, 0, 0}, {cv::Point3f{0, 297, 0}}, {cv::Point3f{210, 297, 0}}, {cv::Point3f{210, 0, 0}}})
{

}

bool IdentifiedObject::receiveCoordinates(QJsonObject coords)
{
    if (!coords.contains(QLatin1String("coords_2D"))) {
        qCritical() << "2D coordinates missing";
        return false;
    }

    const auto coordsArray = coords["coords_2D"].toArray();
    parseCoordinates(coordsArray);

    return true;
}

bool IdentifiedObject::parseCoordinates(const QJsonArray &obj)
{
    for(const auto &var : obj) {
        bool ok1, ok2 = false;
        auto pointMap = var.toObject();
        m_points2D.emplace_back(pointMap["x"].toVariant().toFloat(&ok1), pointMap["y"].toVariant().toFloat(&ok2));
        qInfo() << "Received coordinate: " << var;
        if (!ok1 || !ok2) {
            return false;
        }
    }

    return true;
}


const std::vector<cv::Point3f> &IdentifiedObject::points3D() const
{
    return m_points3D;
}

const std::vector<cv::Point2f> &IdentifiedObject::points2D() const
{
    return m_points2D;
}
