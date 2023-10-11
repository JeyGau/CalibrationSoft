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

    if (!calculatePnPTransformation()) {
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

    Logger::info() << "Translation vector:" << m_transformation.translationVector;
    Logger::info() << "Rotation vector:" << m_transformation.rotationVector << '\n';
    Logger::success() << "Pose estimation successful!";

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

IdentifiedObject *Processor::object() const
{
    return m_object;
}

Camera *Processor::camera() const
{
    return m_camera;
}

const Processor::Transformation &Processor::transformation() const
{
    return m_transformation;
}

bool Processor::calculateCameraPositionInWorld()
{
    Logger::info() << "Calculate camera position in world from image points, object points, camera and distortion parameters...";

    if (!estimatePose()) {
        Logger::error() << "Camera position in world calculation failed";
        return false;
    }

    // Convert rotation vector to rotation matrix
    cv::Mat R;
    cv::Rodrigues(m_transformation.rotationVector, R);

    // Calculate camera position in world coordinates
    cv::Mat positionInWorld = -R.inv() * m_transformation.translationVector;
    m_camera->setPositionInWorld(positionInWorld.clone());

    Logger::success() << "Camera position calculated successfully!";
    Logger::success() << "Camera position in world is (mm): \n" << Tools::toString(m_camera->positionInWorld());

    return true;
}
