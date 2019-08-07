import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    Universal.theme: Universal.Dark
    Universal.accent: Universal.Violet
    width: 640
    height: 480
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
            height: 90

            spacing: 30
            Button{
                Layout.fillWidth: false
                text: qsTr("Open Image")
                onClicked: {
                    fileOpenDialog.open()
                }
            }

            Button{
                Layout.fillWidth: false
                text: qsTr("Close All")
                onClicked: {Wrapper.clearSlides()
                }
            }
        }

        Rectangle
        {
            width : parent.width
            height: parent.height
            ListView
            {
                id:mainList
                anchors.fill: parent
                clip: true
                spacing: 5
                model: Wrapper.slideList

                delegate: Rectangle{
                    height: 90
                    radius: 10
                    border.color: "black"
                    width: parent.width

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
                        Button{
                            text: qsTr("Close Image")
                            onClicked: {Wrapper.deleteSlide(index)
                            }
                        }
                    }
                }
            }
        }
    }
}
