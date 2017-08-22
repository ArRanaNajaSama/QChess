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
    property var images:
    [
        {'imgPath' : "/images/white_pawn.svg"},
        {'imgPath' : "/images/black_pawn.svg"},
    ]
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
        Repeater
        {
            model: logic
            Image
            {
                height: squareSize
                width : squareSize
                x: squareSize * positionX
                y: squareSize * positionY
                source: images[type].imgPath
                MouseArea
                {
                    anchors.fill: parent
                    drag.target: parent

                    property int startX: 0
                    property int startY: 0

                    onPressed:
                    {
                        startX = parent.x;
                        startY = parent.y;
                    }

                    onReleased:
                    {
                        var fromX = startX / squareSize;
                        var fromY = startY / squareSize;
                        var toX   = (parent.x + mouseX) / squareSize;
                        var toY   = (parent.y + mouseY) / squareSize;

                        if (!logic.move(fromX, fromY, toX, toY))
                        {
                            parent.x = startX;
                            parent.y = startY;
                        }
                    }
                }
            }
        }
    }
    ColumnLayout
    {
        anchors.left: gameBoard.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 2
        Button
        {
            id: stopButton
            text: qsTr("Stop")
            onClicked:
            {
                //application stops the game and leads to the screen_1
                console.log("stops the game and leads to the screen_1");
            }
        }
        Button
        {
            id: saveButton
            text: qsTr("Save")
            onClicked:
            {
                //saves the game history to a file
                console.log("saves the game history to a file");
            }
        }
    }
}
