import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

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
                model: Wrapper.slideList
                onCurrentIndexChanged: {
                    imagepath = "image://colors/" + model[mainList.currentIndex].filePath
                }

                delegate: Rectangle{

                    height: 65
                    radius: 4
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
                        anchors.fill: parent
                        Rectangle{
                            id: rectID
                            height:60
                            width:60
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
                            text: modelData.slideName
                            color: "white"
                        }

                        Text {
                            id: modifiedDate
                            text: modelData.modifiedDate
                            color: "white"
                        }
                        RoundButton{
                            contentItem: Text {
                                text: qsTr("x")
                                color: "white"
                            }
                            onClicked: {Wrapper.deleteSlide(index)
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
                Image
                {
                    id: viewPaneId
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    source: imagepath
                }
            }
        }
    }
}
