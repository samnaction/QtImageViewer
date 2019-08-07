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
            width: parent.width
            Button{
                Layout.fillWidth: false
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                text: qsTr("Open Image")
                onClicked: {
                    fileOpenDialog.open()
                }
            }

            Button{
                Layout.fillWidth: false
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                text: qsTr("Close All")
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
            spacing: 10
            width : parent.width
            height: parent.height
            ListView
            {
                id:mainList
                highlight: Rectangle { width: parent.width; color: "dodgerblue" ; radius: 4; border.color: "yellowgreen" ; z:3; opacity:0.1  }
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
                    border.color: "yellowgreen"
                    color: "beige"

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
                            }
                            Text {
                                id: modifiedDate
                                text: modelData.modifiedDate
                            }
                            RoundButton{
                                text: qsTr("X")
                                onClicked: {Wrapper.deleteSlide(index)
                                }
                            }

                    }
                }
            }

            Rectangle
            {
                id:viewpaneRectId
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
