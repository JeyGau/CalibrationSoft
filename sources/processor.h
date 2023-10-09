#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>

#include <opencv2/opencv.hpp>

#include "camera.h"
#include "identifiedobject.h"

class Processor : public QObject
{
    Q_OBJECT

    struct Transformation
    {
        bool isNull() const
        {
            return cv::countNonZero(rotationVector) < 1 && cv::countNonZero(translationVector) < 1;
        }

        bool isEmpty() const
        {
            return rotationVector.empty() && translationVector.empty();
        }


        cv::Mat rotationVector;
        cv::Mat translationVector;
    };

public:
    explicit Processor(Camera *camera, IdentifiedObject *object, QObject *parent = nullptr);

    const Transformation &transformation() const;

public slots:
    bool estimatePose();

signals:
    void log(const QString &message);

private:
    bool calculatePnPTransformation();
//    cv::Mat getCameraPositionInWorld() const;

private:
    Camera *m_camera;
    IdentifiedObject *m_object;

    Transformation m_transformation;
};

#endif // PROCESSOR_H
