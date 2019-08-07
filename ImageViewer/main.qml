import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

ApplicationWindow {
    id:rootId
    visible: true
    visibility: "Maximized"
    width: 640
    height: 480
    title: qsTr("C-Bits")
    color: "#FF24292E"

    property string  imagepath: ""

    FileDialog
    {
        id: fileOpenDialog
        title: "Select an image file"
        folder: shortcuts.documents
        selectMultiple: true
        nameFilters: ["Image files (*.png *.jpeg *.jpg *.tif *.tiff *.svs)"]
        onAccepted: {
            Wrapper.setSlides(fileOpenDialog.fileUrls)
        }
    }

    Column{
        id: slidetryId
        anchors.fill: parent
        spacing: 10
        RowLayout
        {
            id:buttonLayoutId
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.leftMargin: 10
            width: parent.width
            Button
            {
                id:openImageId
                text: qsTr("Open Image")
                contentItem: Text
                {
                    text: openImageId.text
                    font: openImageId.font
                    color: openImageId.hovered ? "green" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle
                {
                    implicitWidth: 100
                    implicitHeight: 40
                    border.color: "black"
                    color: openImageId.hovered ?   "#FF24292E" : "#FF2F363D"
                    border.width: 1
                    radius: 2
                }
                onClicked: {
                    fileOpenDialog.open()
                }
                anchors.verticalCenter: parent.verticalCenter
            }

            Button
            {
                id:closeAllId
                text: qsTr("Close All")
                contentItem: Text
                {
                    text: closeAllId.text
                    font: closeAllId.font
                    color: closeAllId.hovered ? "red" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle
                {
                    implicitWidth: 100
                    implicitHeight: 40
                    border.color: "black"
                    color: closeAllId.hovered ? "#FF24292E" : "#FF2F363D"
                    border.width: 1
                    radius: 2
                }
                onClicked: {
                    Wrapper.clearSlides()
                    imagepath=""
                }
                anchors.verticalCenter: parent.verticalCenter
            }

            Layout.fillWidth: true
            Text {
                id: brightness
                text: qsTr("brightness ")
                font.pointSize: 13;
                color: "white"
                anchors.right: brightnessID.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Slider {
                id: brightnessID
                from:-1.0
                to: 1.0
                anchors.top: parent.top
                value: 0.0
                anchors.right: contrast.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: contrast
                text: qsTr("contrast ")
                font.pointSize: 13;
                color: "white"
                anchors.right: contrastID.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Slider {
                id: contrastID
                from:-1.0
                to: 1.0
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 30
                value: 0.0
                anchors.verticalCenter: parent.verticalCenter
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout
        {
            anchors.left: parent.left
            anchors.top: buttonLayoutId.bottom
            anchors.topMargin: 10
            anchors.leftMargin: 10
            spacing: 10
            width : parent.width
            height: parent.height
            ListView
            {
                id:mainList
                highlight: Rectangle { width: parent.width; color: "#FF444D56" ; radius: 4; border.color: "black" ; z:3; opacity:0.4  }
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                focus: true
                width: 460
                spacing:10
                clip: true
                model: Wrapper.slideList
                onCurrentIndexChanged: {
                    imagepath = "image://colors/" + model[mainList.currentIndex].filePath
                }

                delegate: Rectangle{

                    height: 90
                    width: parent.width
                    border.color: "black"
                    color: "#FF24292E"

                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            mainList.currentIndex = index
                        }
                    }

                    RowLayout{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        Rectangle{
                            id: rectID
                            height:70
                            width:140
                            Image
                            {
                                id: thumbnailImage
                                width: rectID.width
                                height:rectID.height
                                asynchronous: true
                                source: "image://colors/" + modelData.filePath
                                fillMode: Image.PreserveAspectFit
                            }
                        }

                        Text {
                            id: slideName
                            width:2
                            text: modelData.slideName
                            color: "white"
                            wrapMode: Text.Wrap

                        }

                        Text {
                            id: modifiedDate
                            text: modelData.modifiedDate
                            color: "white"
                        }
                        RoundButton{
                            id: slideCloseId
                            text: qsTr("x")
                            font.family: "Arial"
                            font.weight: Font.Bold
                            contentItem: Text {
                                text: slideCloseId.text
                                font: slideCloseId.font
                                color: slideCloseId.hovered ? "red" : "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                            onClicked: {
                                Wrapper.deleteSlide(index)
                            }
                        }

                    }
                }
            }

            Rectangle
            {
                id:viewpaneRectId
                color: "#FF24292E"
                Layout.fillHeight: true
                Layout.fillWidth: true

                width: viewPaneId.width * (1 + 0.10 * viewPaneId.height / viewPaneId.width)
                height: viewPaneId.height * 1.10
                scale: defaultSize / Math.max(viewPaneId.sourceSize.width, viewPaneId.sourceSize.height)
                Behavior on scale { NumberAnimation { duration: 200 } }
                Behavior on x { NumberAnimation { duration: 200 } }
                Behavior on y { NumberAnimation { duration: 200 } }
                smooth: true
                antialiasing: true
                Component.onCompleted: {
                    x = Math.random() * root.width - width / 2
                    y = Math.random() * root.height - height / 2
                    rotation = Math.random() * 13 - 6
                }
                Image
                {
                    id: viewPaneId
                    anchors.centerIn: parent

                    fillMode: Image.PreserveAspectFit
                    source: imagepath
                    focus: true
                    BrightnessContrast {
                        anchors.fill: viewPaneId
                        source: viewPaneId
                        contrast: contrastID.value
                        brightness: brightnessID.value
                    }

                }
                MouseArea {
                    id: dragArea
                    hoverEnabled: true
                    anchors.fill: parent
                    drag.target: viewpaneRectId
                    scrollGestureEnabled: false  // 2-finger-flick gesture should pass through to the Flickable
                    onPressed: {
                        viewpaneRectId.z = ++root.highestZ;
                        parent.setFrameColor();
                    }
                    onEntered: parent.setFrameColor();
                    onWheel: {
                        if (wheel.modifiers & Qt.ControlModifier) {
                            viewpaneRectId.rotation += wheel.angleDelta.y / 120 * 5;
                            if (Math.abs(viewpaneRectId.rotation) < 4)
                                viewpaneRectId.rotation = 0;
                        } else {
                            viewpaneRectId.rotation += wheel.angleDelta.x / 120;
                            if (Math.abs(viewpaneRectId.rotation) < 0.6)
                                viewpaneRectId.rotation = 0;
                            var scaleBefore = viewpaneRectId.scale;
                            viewpaneRectId.scale += viewpaneRectId.scale * wheel.angleDelta.y / 120 / 10;
                        }
                    }
                }
            }

        }
    }
}
