#ifndef TOOLS_H
#define TOOLS_H

#include <opencv2/opencv.hpp>

#include <QJsonObject>
#include <QString>

QDebug operator<<(QDebug debug, const cv::Mat &mat);
QDebug operator<<(QDebug debug, const QJsonObject &obj);
QDebug operator<<(QDebug debug, const QJsonValue &val);

class Tools
{

    /**
 * @brief toString transforms a matrix to a QString to ease debugging with Qt
 * @param matrix
 * @return matrix as a QString
 */
public:
    static QString toString(const cv::Mat &matrix)
    {
        cv::Mat oneRow = matrix.reshape(0,1);    // Treat as vector
        std::ostringstream os;
        os << oneRow;                             // Put to the stream
        std::string asStr = os.str();             // Get string
        asStr.pop_back();                         // Remove brackets
        asStr.erase(0,1);
        return QString::fromStdString(asStr);
    }

};


#endif // TOOLS_H
