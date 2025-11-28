import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    width: 200
    height: 200
    color: "#2A2846"

    // C++ tarafına gidecek sinyal
    signal createClicked()

    // Hover durumunu takip ediyoruz
    property bool isHovered: hoverArea.containsMouse

    // Hover değiştiğinde Canvas'ı yeniden çizdir
    onIsHoveredChanged: dashedCanvas.requestPaint()

    // --- 1. Arka Plan Dolgusu (Hover'da hafif aydınlık) ---
    Rectangle {
        anchors.fill: parent
        radius: 12
        color: root.isHovered ? "#353355" : "transparent" // #2A2846'nın biraz açığı

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    // --- 2. Kesik Çizgili Çerçeve (CANVAS Yöntemi) ---
    Canvas {
        id: dashedCanvas
        anchors.fill: parent
        // Çizim kalitesi için
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            // Çizgi Ayarları
            ctx.lineWidth = 2;
            // Hover ise SARI (#FFFB7D), değilse SOLUK MOR (#585675)
            ctx.strokeStyle = root.isHovered ? "#FFFB7D" : "#585675";

            // Kesik Çizgi Deseni: [Dolu, Boş]
            ctx.setLineDash([10, 6]);

            // Yuvarlak Köşeli Dikdörtgen Çizimi
            var x = 1; // Çizgi kalınlığı taşmasın diye az içeriden
            var y = 1;
            var w = width - 2;
            var h = height - 2;
            var r = 12; // Radius

            ctx.beginPath();
            ctx.moveTo(x + r, y);
            ctx.lineTo(x + w - r, y);
            ctx.arcTo(x + w, y, x + w, y + r, r);
            ctx.lineTo(x + w, y + h - r);
            ctx.arcTo(x + w, y + h, x + w - r, y + h, r);
            ctx.lineTo(x + r, y + h);
            ctx.arcTo(x, y + h, x, y + h - r, r);
            ctx.lineTo(x, y + r);
            ctx.arcTo(x, y, x + r, y, r);
            ctx.closePath();

            ctx.stroke();
        }
    }

    // --- 3. İkon ve Yazı ---
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 15

        Image {
            id: iconImg
            source: "qrc:/resource/icons/add.png"
            sourceSize.width: 32
            sourceSize.height: 32
            Layout.alignment: Qt.AlignHCenter
            opacity: root.isHovered ? 1.0 : 0.5

        }

        Text {
            text: "Create New or\nAdd Existing\nNote"
            font.family: "Segoe UI"
            font.pixelSize: 16
            font.bold: true
            color: root.isHovered ? "#FFFB7D" : "#585675"
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter // Metni kendi içinde ortalar

            Behavior on color { ColorAnimation { duration: 100 } }
        }
    }

    // --- 4. Tıklama Alanı ---
    MouseArea {
        id: hoverArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true // Hover'ı algılaması için şart!

        onClicked: {
            root.createClicked()
        }
    }
}

