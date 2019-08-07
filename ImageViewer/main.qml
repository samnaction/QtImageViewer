import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    title: qsTr("C-Bits")

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
                onClicked: {Wrapper.clearSlides()
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
        RowLayout
        {
            width : parent.width
            height: parent.height
            ListView
            {
                id:mainList
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                width: 200
                model: Wrapper.slideList
                delegate: Rectangle{
                    height: 90
                    radius: 10
                    width: parent.width
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            Wrapper.setSelected(index)
                        }
                    }

                    ColumnLayout{
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
                        RowLayout{

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
            }
            Image
            {
                id: viewPaneId
                source : "image://colors/" + Wrapper.selectedSlide
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                fillMode: Image.PreserveAspectFit
                cache: false
            }
        }
    }
}
