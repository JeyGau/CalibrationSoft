#include "processor.h"

#include "logger.h"
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
    Logger::info() << "Estimating pose...";

    bool success = calculatePnPTransformation();
    if (!success) {
        Logger::error() << "PnP estimation failed";
        return false;
    }
    if (m_transformation.isEmpty()) {
        Logger::error() << "empty transformation vectors";
        return false;
    }
    if (m_transformation.isNull()) {
        Logger::error() << "nil transformation vectors";
        return false;
    }

    Logger::success() << "Pose estimation successful!";
    Logger::success() << "Translation vector:" << m_transformation.translationVector;
    Logger::success() << "Rotation vector:" << m_transformation.rotationVector << '\n';

    return true;
}

bool Processor::calculatePnPTransformation()
{
    Logger::info() << "Calculating PnP transformation using SolvePnP function from openCV library...";

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
