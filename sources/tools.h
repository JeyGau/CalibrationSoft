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
        std::ostringstream os;
        for (int i = 0; i < matrix.rows; ++i)
        {
            cv::Mat row = matrix.row(i);
            os << row << std::endl;
        }
        std::string asStr = os.str();

        return QString::fromStdString(asStr);
    }
};


#endif // TOOLS_H
