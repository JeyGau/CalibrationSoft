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

    signal back()
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

        // TO DO: handle jpg file error here

        Canvas {
            id: myCanvas
            anchors.fill: parent

            function clear() {
                var ctx = getContext("2d");
                ctx.reset();
                requestPaint();
            }

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
                if (coords2D_displayed.length < 2) {
                    return;
                }

                drawLine(coords2D_displayed[coords2D_displayed.length - 2], coords2D_displayed[coords2D_displayed.length - 1]);

                if (coords2D_displayed.length === 4) {
                    drawLine(coords2D_displayed[0], coords2D_displayed[coords2D_displayed.length - 1]);
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

                // Calculate the scaling factor
                var imageScale = Math.min(photo.width / photo.sourceSize.width, photo.height / photo.sourceSize.height);

                // Calculate the offset due to the aspect ratio preservation
                var offsetX = (photo.width - photo.sourceSize.width * imageScale) / 2;
                var offsetY = (photo.height - photo.sourceSize.height * imageScale) / 2;

                // Calculate the mouse's position relative to the image
                var imageX = (mouseX - offsetX) / imageScale;
                var imageY = (mouseY - offsetY) / imageScale;

                coords2D_displayed.push({x: mouseX, y: mouseY})
                coords2D.push({x: imageX, y: imageY})

                var i = coords2D.length - 1;
                selectedPointsText.text += '\n(' + coords2D[i].x + ', ' +  coords2D[i].y+')'
                        + ' -> (' + coords3D[i].x + ', ' +  coords3D[i].y + ', ' +  coords3D[i].z+')';

                if (coords2D.length > 1) {
                    myCanvas.requestPaint();
                }

                if (coords2D.length === 4) {
                    paperSheet.receiveCoordinates({
                                                      "coords_2D": coords2D,
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

    RowLayout {
        Layout.fillHeight: true
        Layout.preferredWidth: 400
        Layout.alignment: Qt.AlignHCenter

        spacing: 100

        Button {
            id: previousButton
            Layout.alignment: Qt.AlignVCenter
            text: "Back"
            enabled: true
            onClicked: {
                coords2D_displayed = [];
                coords2D = [];
                selectedPointsText.text = "Selected points are (image coordinates -> space coordinates):";
                myCanvas.clear();
                loadButton.enabled = false;
                back();
            }
        }

        Button {
            id: loadButton
            Layout.alignment: Qt.AlignVCenter
            text: "Estimate pose"
            enabled: false
            onClicked: estimatePose()
        }
    }
}
