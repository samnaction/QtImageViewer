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
            nameFilters: [
                "Image files (*.png *.jpeg *.jpg *.tif *.tiff)",
            ]
            onAccepted: {
//                for(var i=0; i <= fileOpenDialog.fileUrls.lenght; i++)
//                {
//                    filenames.push(fileOpenDialog.fileUrls[i])
//                }
                Wrapper.setSlides(fileOpenDialog.fileUrls)
            }
     }

    ColumnLayout{
        id: slidetryId
        height: 550
        RowLayout
        {
            spacing: 20
            Button{
                text: qsTr("Open Image")
                onClicked: {
                    fileOpenDialog.open()


                }
            }

            Button{
                text: qsTr("Save Image")
            }

            Button{
                text: qsTr("Close All")
                onClicked: {Wrapper.clearSlides()
                }
            }
        }

        Rectangle
        {
            width : 400
            height: parent.height
            color: "greenyellow"
            ListView
            {
                id:mainList
                width: parent.width - 10
                height: parent.height
                anchors.fill: parent
                clip: true

                model: Wrapper.slideList

                delegate: Rectangle{
                    height: 90
                    radius: 10
                    color : "gray"// Can also do modelData.favoriteColor directly but adding model makes it clear where the data is coming from. More readable
                    border.color: "cyan"
                    width: parent.width

                    RowLayout{
                        anchors.fill: parent
                        anchors.margins: 20

                        Rectangle{
                            id: rectID
                            height:80
                            width:80
                            Image
                            {
                                id: thumbnailImage
                                source: modelData.slideName
                                width: rectID.width
                                height:rectID.height
                                fillMode: Image.PreserveAspectFit
                            }
                        }

                        Text {
                            id: slideName
                            text: modelData.slideName
                            Layout.fillWidth: true
                        }
                        Text {
                            id: modifiedDate
                            text: modelData.modifiedDate
                            Layout.fillWidth: true
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
