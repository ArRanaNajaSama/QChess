import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow
{
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600
    property int squareSize: 70
    Item
    {
        id: gameBoard
        x: 0
        y: 0
        width : logic.boardSize * squareSize
        height: logic.boardSize * squareSize
        Image
        {
            source: "/images/chess_board.jpg"
            height: gameBoard.height
            width: gameBoard.width
        }

        ColumnLayout
        {
            anchors.left: gameBoard.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 2
            Label
            {
               Layout.alignment: Qt.AlignCenter
               text: qsTr("CHESS")
               font.pixelSize: 22
            }
            Button
            {
                id: startButton
                text: qsTr("Start")
                onClicked:
                {
                    //starts a new game. Leads to the screen_2
                    console.log("starts a new game");
                }
            }
            Button
            {
                id: loadButton
                text: qsTr("Load")
                onClicked:
                {
                    //allows user to load saved game. Leads to the screen_3
                    console.log("load saved game");
                }
            }
            Button
            {
                id: clearButton
                text: qsTr("Clear")
                onClicked:
                {
                    //Empty chess board
                    console.log("Empty chess board");
                    logic.clear();
                }
            }
        }
    }
}
