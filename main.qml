import QtQuick 2.15
import QtQuick.Window 2.15

import QtQuick.Layouts 1.3

Window {
    width: 1280
    height: 960
    visible: true
    title: qsTr("Calibration")

    property var coords: []

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        spacing: 20

        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            text: "Select all 4 corners of paper sheet:"
        }

        Image {
            id: photo

            Layout.fillHeight: true
            Layout.fillWidth: true

            fillMode: Image.PreserveAspectFit

            source: "qrc:/assets/calibration.jpg"

            Canvas {
                id: myCanvas
                anchors.fill: parent

                onPaint: {
                    if (coords.length < 2) {
                        return;
                    }

                    var prevX = coords[coords.length - 2].x;
                    var prevY = coords[coords.length - 2].y;
                    var curX = coords[coords.length - 1].x;
                    var curY = coords[coords.length - 1].y;

                    var ctx = getContext("2d");
                    ctx.strokeStyle = Qt.rgba(1, 0, 0, 1);
                    ctx.lineWidth = 2;
                    ctx.beginPath();
                    ctx.moveTo(prevX, prevY);
                    ctx.lineTo(curX, curY);
                    ctx.stroke();

                    if (coords.length === 4) {
                        prevX = coords[0].x;
                        prevY = coords[0].y;
                        curX = coords[coords.length - 1].x;
                        curY = coords[coords.length - 1].y;
                        ctx.moveTo(prevX, prevY);
                        ctx.lineTo(curX, curY);
                        ctx.stroke();
                    }
                }
            }

            MouseArea {
                id: myMouseArea
                anchors.fill: parent

                onClicked: {
                    if (coords.length > 3) {
                        return;
                    }

                    coords.push({x: mouseX, y: mouseY})
                    selectedPointsText.text += '\n' + coords[coords.length - 1].x + ', ' +  coords[coords.length - 1].y;

                    if (coords.length > 1) {
                        myCanvas.requestPaint();
                    }
                }
            }
        }

        Text {
            id: selectedPointsText

            Layout.fillWidth: true
            Layout.preferredHeight: 100

            text:  "Selected points are:"
        }
    }
}
