import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import CalibrationSoft.Logger 1.0

ColumnLayout {
    property alias processingLogsArea: logArea

    signal back()

    Component.onCompleted: {
        logger.logMessage.connect(function(message, type) {
            var color;
            switch(type) {
            case Logger.Success:
                color = "green";
                break;
            case Logger.Info:
                color = "white";
                break;
            case Logger.Warning:
                color = "orange";
                break;
            case Logger.Error:
                color = "red";
                break;
            default:
                color = "white";
            }

            message = message.replace(/\n/g, "<br>"); // since we are using rich text format, we need to use html tag
            var coloredMessage = "<font color=\"" + color + "\">" + message + "</font>";
            processingLogsArea.append(coloredMessage);
        });
    }

    ScrollView {
        id: view
        Layout.fillHeight: true
        Layout.fillWidth: true

        TextArea {
            id: logArea
            font.pixelSize: 20
            textFormat: Text.RichText
        }
    }

    Button {
        id: previousButton
        Layout.alignment: Qt.AlignVCenter
        text: "Back"
        enabled: true
        onClicked: {
            back();
        }
    }
}
