import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 20

    property alias picture: photo.source

    function resolutionRatio() {
        // ratio between displayed resolution and source image resolution
        return Math.max(photo.sourceSize.width/photo.width, photo.sourceSize.height/photo.height);
    }

    signal estimatePose()


    Label {
        Layout.fillWidth: true
        Layout.preferredHeight: 40

        font.pixelSize: 20
        text: "Select all 4 corners of paper sheet from its bottom left to its bottom right corner, clockwise:"
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

            function drawLine(from, to) {
                var ctx = getContext("2d");
                ctx.strokeStyle = Qt.rgba(1, 0, 0, 1);
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.moveTo(from.x, from.y);
                ctx.lineTo(to.x, to.y);
                ctx.stroke();
            }

            onPaint: {
                if (coords2D.length < 2) {
                    return;
                }

                drawLine(coords2D[coords2D.length - 2], coords2D[coords2D.length - 1]);

                if (coords2D.length === 4) {
                    drawLine(coords2D[0], coords2D[coords2D.length - 1]);
                }
            }
        }

        MouseArea {
            id: myMouseArea
            anchors.fill: parent

            onClicked: {
                if (coords2D.length > 3) {
                    return;
                }

                coords2D.push({x: mouseX, y: mouseY})
                var i = coords2D.length - 1;
                selectedPointsText.text += '\n(' + coords2D[i].x*resolutionRatio() + ', ' +  coords2D[i].y*resolutionRatio()+')'
                + ' -> (' + coords3D[i].x + ', ' +  coords3D[i].y + ', ' +  coords3D[i].z+')';

                if (coords2D.length > 1) {
                    myCanvas.requestPaint();
                }

                if (coords2D.length === 4) {
                    paperSheet.receiveCoordinates({
                                                      "coords_2D": coords2D,
                                                      "resolution_ratio": resolutionRatio()
                                                   });
                    loadButton.enabled = true;
                }
            }
        }
    }

    Label {
        id: selectedPointsText

        Layout.fillWidth: true
        Layout.preferredHeight: 140

        font.pixelSize: 20

        text:  "Selected points are (image coordinates -> space coordinates):"
    }

    Button {
        id: loadButton
        Layout.alignment: Qt.AlignHCenter
        text: "Estimate pose"
        enabled: false
        onClicked: estimatePose()
    }
}
