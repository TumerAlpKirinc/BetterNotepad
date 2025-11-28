import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    id: root
    width: 200
    height: 200

    color: "#2A2846"
    property string cardColor: "#FFFB7D"
    property string filePath: ""
    property string titleText: "Başlıksız Not"
    property string contentText: "İçerik yok..."

    signal cardClicked(string filePath)
    signal deleteClicked(string filePath)
    signal colorClicked(string filePath)
    signal rightClicked(string filePath)

    property bool isHovered: hoverArea.containsMouse || deleteMouseArea.containsMouse || colorMouseArea.containsMouse

    // ---------------------------
    // 🔥 DRAG & DROP EKLENTİLERİ
    // ---------------------------
    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2
    Drag.source: root

    DropArea {
        id: dropper
        anchors.fill: parent
        onDropped: {
            if (drop.source && drop.source !== root && root.GridView) {
                root.GridView.model.move(drop.source.GridView.index,
                                         root.GridView.index, 1)
            }
        }
    }
    // ---------------------------


    // --- Kartın Kendisi ---
    Rectangle {
        id: cardVisual
        anchors.fill: parent
        radius: 12
        color: parent.cardColor

        border.color: "#2A2846"
        border.width: 2

        // Hover efekti
        scale: root.isHovered ? 1.02 : 1.0
        Behavior on scale { NumberAnimation { duration: 150 } }

        // --- İçerik ---
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 10

            Text {
                text: root.titleText
                font.bold: true
                font.pixelSize: 16
                font.family: "Segoe UI"
                color: "#2A2846"
                Layout.fillWidth: true
                elide: Text.ElideRight
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#2A2846"
                opacity: 0.2
            }

            Text {
                text: root.contentText
                font.pixelSize: 12
                font.family: "Segoe UI"
                color: "#444444"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.fillHeight: true
                elide: Text.ElideRight
                verticalAlignment: Text.AlignTop
            }
        }
    }

    // --- Silme Butonu ---
    Rectangle {
        id: deleteBtnBackground
        width: 32
        height: 32
        radius: 16
        color: deleteMouseArea.containsMouse ? "#ff6b6b" : cardColor

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 10

        visible: root.isHovered
        opacity: visible ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: 200 } }
        z: 10

        Image {
            id: deleteImg
            anchors.centerIn: parent
            source: "qrc:/resource/icons/delete.svg"
            width: 16
            height: 16
            sourceSize.width: 32
            sourceSize.height: 32
        }

        MultiEffect {
            anchors.centerIn: parent
            width: 16
            height: 16
            source: deleteImg
            colorization: 1.0
            colorizationColor: deleteMouseArea.containsMouse ? "transparent" : "#ff6b6b"
        }

        MouseArea {
            id: deleteMouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true
            onClicked: root.deleteClicked(filePath)
        }
    }

    // --- Renk Butonu ---
    Rectangle {
        id: colorBtnBackground
        width: 32
        height: 32
        radius: 16
        color: colorMouseArea.containsMouse ? "#ff6b6b" : cardColor

        anchors.top: parent.top
        anchors.right: deleteBtnBackground.left
        anchors.topMargin: 10
        anchors.rightMargin: 10

        visible: root.isHovered
        opacity: visible ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: 200 } }

        z: 10

        Image {
            id: colorPickImg
            anchors.centerIn: parent
            source: "qrc:/resource/icons/color-pick.svg"
            width: 16
            height: 16
            sourceSize.width: 32
            sourceSize.height: 32
        }

        MultiEffect {
            anchors.centerIn: parent
            width: 16
            height: 16
            source: colorPickImg
            colorization: 1.0
            colorizationColor: colorMouseArea.containsMouse ? "transparent" : "#ff6b6b"
        }

        MouseArea {
            id: colorMouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true
            onClicked: root.colorClicked(filePath)
        }
    }

    // --- Normal tıklama alanı ---
    MouseArea {
        id: hoverArea
        anchors.fill: parent
        z: 1
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            if (mouse.button === Qt.LeftButton)
                root.cardClicked(filePath)
            else if (mouse.button === Qt.RightButton)
                root.rightClicked(filePath)
        }
    }

    // ---------------------------
    // 🔥 DRAG ALANI (sürükleme tetikleyici)
    // ---------------------------
    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: root
        enabled: true
    }
}
