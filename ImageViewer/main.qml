import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    background: Rectangle {
    color: "white"
    }

//    header: ToolBar{
//        Flow{
//            anchors.fill: parent
//            ToolButton{
//                text: qsTr("Open")
//                icon.name: "document-open"
//                onClicked: fileOpenDialog.open()
//            }
//        }
//    }

    menuBar: MenuBar {
    Menu {
    title: qsTr("&File")
    MenuItem {
    text: qsTr("&Open...")
    icon.name: "document-open"
    onTriggered: fileOpenDialog.open()
    }
    }
    Menu {
    title: qsTr("&Help")
    MenuItem {
    text: qsTr("&About...")
    onTriggered: aboutDialog.open()
    }
    }
    }

    FileDialog {
    id: fileOpenDialog
    title: "Select an image file"
    folder: shortcuts.documents
    nameFilters: [
    "Image files (*.*)",
    ]
    onAccepted: {
    image.source = fileOpenDialog.fileUrl
    }
    }

    Dialog {
    id: aboutDialog
    title: qsTr("About")
    Label {
    anchors.fill: parent
    text: qsTr("QT Viewer")
    horizontalAlignment: Text.AlignHCenter
    }
    standardButtons: StandardButton.Ok
    }

    Image {
    id: image
    anchors.fill: parent
    fillMode: Image.PreserveAspectFit
    asynchronous: true
    }
}
