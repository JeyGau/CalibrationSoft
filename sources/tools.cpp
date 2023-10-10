#include "tools.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

QDebug operator<<(QDebug debug, const cv::Mat &mat)
{
    QDebugStateSaver saver(debug);

    debug.nospace() << "Mat [rows: " << mat.rows
                    << ", cols: " << mat.cols
                    << "] ";

    debug.nospace() << Tools::toString(mat);

    return debug;
}

QDebug operator<<(QDebug debug, const QJsonObject &obj)
{
    QDebugStateSaver saver(debug);
    debug.noquote() << QJsonDocument(obj).toJson();
    return debug;
}

QDebug operator<<(QDebug debug, const QJsonValue &val)
{
    QDebugStateSaver saver(debug);
    switch (val.type()) {
    case QJsonValue::Null:
        break;
    case QJsonValue::Bool:
        debug.noquote() << val.toBool();
        break;
    case QJsonValue::Double:
        debug.noquote() << val.toDouble();
        break;
    case QJsonValue::String:
        debug.noquote() << val.toString();
        break;
    case QJsonValue::Array:
        debug.noquote() << QJsonDocument(val.toArray()).toJson();
        break;
    case QJsonValue::Object:
        debug.noquote() << QJsonDocument(val.toObject()).toJson();
        break;
    case QJsonValue::Undefined:
    default:
        break;
    }

    return debug;
}
