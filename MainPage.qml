import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15

Item {
    property string selectedJson: jsonFileLabel.text
    property string selectedJpg: jpgFileLabel.text

    signal run()

    GridLayout {
        anchors.centerIn: parent
        columns: 2

        Label {
            text: "Select camera parameters JSON and paper sheet JPG Files"
            font.pixelSize: 20
            Layout.columnSpan: 2
            horizontalAlignment: Text.AlignHCenter
            color: "white"
        }

        Button {
            text: "Browse JSON"
            onClicked: jsonFileDialog.open()
        }

        Label {
            id: jsonFileLabel
            text: "qrc:/assets/camera_parameters.json"
            color: "white"
        }

        Button {
            text: "Browse JPG"
            onClicked: jpgFileDialog.open()
        }

        Label {
            id: jpgFileLabel
            text: "qrc:/assets/calibration.jpg"
            color: "white"
        }

        Button {
            id: loadButton
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            text: "Run"
            onClicked: run()
        }
    }

    FileDialog {
        id: jsonFileDialog
        title: "Please choose a JSON file"
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            // open jpg file selector and parse selected file
            console.log("JSON file selected: " + selectedFile);
            jsonFileLabel.text = selectedFile;
            close();
        }
    }

    FileDialog {
        id: jpgFileDialog
        title: "Please choose a JPG file"
        nameFilters: ["JPG files (*.jpg)"]
        onAccepted: {
            // display selected file
            console.log("JPG file selected: " + selectedFile);
            jpgFileLabel.text = selectedFile;
            close();
        }
    }
}

