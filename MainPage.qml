import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

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
        }

        Button {
            text: "Browse JSON"
            onClicked: jsonFileDialog.open()
        }

        Label {
            id: jsonFileLabel
            text: "qrc:/assets/camera_parameters.json"
        }

        Button {
            text: "Browse JPG"
            onClicked: jpgFileDialog.open()
        }

        Label {
            id: jpgFileLabel
            text: "qrc:/assets/calibration.jpg"
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

