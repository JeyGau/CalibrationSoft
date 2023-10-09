#include "camera.h"
#include <QDebug>

Camera::Camera(QObject *parent)
    : QObject{parent}
    , m_cameraParameters{}
{

}

void Camera::parseParametersFile(const QString &path)
{
    qDebug() << path;
}
