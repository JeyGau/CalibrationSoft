#include "processor.h"

#include "tools.h"

Processor::Processor(Camera *camera, IdentifiedObject *object, QObject *parent)
    : QObject{parent}
    , m_camera{camera}
    , m_object{object}
    , m_transformation{}
{

}

bool Processor::estimatePose()
{
    bool success = calculatePnPTransformation();
    if (!success) {
        qCritical() << "PnP estimation failed";
        return false;
    }
    if (m_transformation.isEmpty()) {
        qCritical() << "empty transformation vectors";
        return false;
    }
    if (m_transformation.isNull()) {
        qCritical() << "nil transformation vectors";
        return false;
    }

    qInfo() << "Translation vector" << Tools::toString(m_transformation.translationVector);
    qInfo() << "Rotation vector" << Tools::toString(m_transformation.rotationVector);

    return true;
}

bool Processor::calculatePnPTransformation()
{
    m_transformation.rotationVector = cv::Mat::zeros(3, 1, CV_64F);
    m_transformation.translationVector = cv::Mat::zeros(3, 1, CV_64F);

    return cv::solvePnP(m_object->points3D(),
                 m_object->points2D(),
                 m_camera->intrinsicParams(),
                 m_camera->distortionCoeffs(),
                 m_transformation.rotationVector,
                 m_transformation.translationVector,
                 false,
                        cv::SOLVEPNP_IPPE);
}

const Processor::Transformation &Processor::transformation() const
{
    return m_transformation;
}

//cv::Mat Processor::getCameraPositionInWorld() const
//{
//    // Convert rotation vector to rotation matrix
//    cv::Mat R;
//    cv::Rodrigues(m_transformation.rotationVector, R);

//    // Calculate camera position in world coordinates
//    cv::Mat cameraPosition = -R.inv() * m_transformation.translationVector;

//    // Print camera position
//    qInfo() << "Camera Position in World Coordinates: " << Tools::toString(cameraPosition);

//    return cameraPosition;
//}
