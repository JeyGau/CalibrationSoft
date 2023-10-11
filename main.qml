import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Window

Window {
    width: 1280
    height: 960
    visible: true
    title: qsTr("Calibration")

    color: "black"

    property var coords2D: [] // coordinates points in source image of 2560*1920
    property var coords2D_displayed: [] // coordinates points in currently displayed image
    property var coords3D: [ // coordinates points in 3D space
        {
            "x": 0,
            "y": 0,
            "z": 0
        },
        {
            "x": 0,
            "y": 297,
            "z": 0
        },
        {
            "x": 210,
            "y": 297,
            "z": 0
        },
        {
            "x": 210,
            "y": 0,
            "z": 0
        }
    ]

    function toMainMenu() {
        mainPage.visible = true;
        imagePage.visible = false;
        processingPage.visible = false;
    }

    MainPage {
        id: mainPage
        anchors.fill: parent

        onRun: {
            camera.parseParametersFile(selectedJson);
            imagePage.picture = selectedJpg;

            // TO DO: use state machine and loader instead of changing visibility
            mainPage.visible = false;
            imagePage.visible = true;
            processingPage.visible = false;
        }
    }

    ImagePage {
        id: imagePage

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        visible: false

        onBack: toMainMenu()

        onEstimatePose: {
            mainPage.visible = false;
            imagePage.visible = false;
            processingPage.visible = true;

            processor.calculateCameraPositionInWorld();
        }
    }

    ProcessingPage {
        id: processingPage

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        visible: false

        onBack: toMainMenu()
    }
}
