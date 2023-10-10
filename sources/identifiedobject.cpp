#include "identifiedobject.h"

// project
#include "logger.h"
#include "tools.h"

// Qt
#include <QJsonArray>
#include <QJsonDocument>

IdentifiedObject::IdentifiedObject(QObject *parent)
    : QObject{parent}
    , m_points2D()
    , m_points3D({cv::Point3f{0, 0, 0}, {cv::Point3f{0, 297, 0}}, {cv::Point3f{210, 297, 0}}, {cv::Point3f{210, 0, 0}}})
{

}

bool IdentifiedObject::receiveCoordinates(QJsonObject coords)
{
    Logger::info() << "Receiving coordinates from user clicks...";
    if (!coords.contains(QLatin1String("coords_2D"))) {
        Logger::error() << "2D coordinates missing from objet" << coords;
        return false;
    }

    const auto coordsArray = coords["coords_2D"].toArray();
    parseCoordinates(coordsArray);

    Logger::success() << "Coordinates received successfully!\n";

    return true;
}

bool IdentifiedObject::parseCoordinates(const QJsonArray &obj)
{
    for(const auto &var : obj) {
        bool ok1, ok2 = false;
        auto pointMap = var.toObject();
        cv::Point2f point2D{pointMap["x"].toVariant().toFloat(&ok1), pointMap["y"].toVariant().toFloat(&ok2)};
        m_points2D.emplace_back(point2D);
        if (!ok1 || !ok2) {
            Logger::error() << "Failed to parse coordinate: " << var;
            return false;
        }
        Logger::info() << cv::Mat(point2D);
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
