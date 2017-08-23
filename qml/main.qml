import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600

    property int squareSize: 70

    property var images: [
        {'imgPath' : "/images/white_pawn.svg"},
        {'imgPath' : "/images/black_pawn.svg"},
    ]

    Item {
        id: gameBoard
        x: 0
        y: 0
        width : logic.boardSize * squareSize
        height: logic.boardSize * squareSize

        Image {
            source: "/images/chess_board.jpg"
            height: gameBoard.height
            width: gameBoard.width
        }

        Repeater {
            model: logic

            Image {
                height: squareSize
                width : squareSize

                x: squareSize * positionX
                y: squareSize * positionY

                source: images[type].imgPath

                MouseArea {
                    anchors.fill: parent
                    drag.target: parent

                    property int startX: 0
                    property int startY: 0

                    onPressed: {
                        startX = parent.x;
                        startY = parent.y;
                    }

                    onReleased: {
                        var fromX = startX / squareSize;
                        var fromY = startY / squareSize;
                        var toX   = (parent.x + mouseX) / squareSize;
                        var toY   = (parent.y + mouseY) / squareSize;

                        if (!logic.move(fromX, fromY, toX, toY)) {
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
        spacing: 5

        Button {
            id: startButton
            visible: true
            text: qsTr("Start")
            onClicked: {
                logic.startNewGame()
                startButton.visible = false
                loadButton.visible = false
                game_nav.visible = false
                stopButton.visible = true
                saveButton.visible = true
                console.log("Starting new game...")
            }
        }

        Button {
            id: loadButton
            visible: true
            text: qsTr("Load")
            onClicked: {
                logic.loadGame()
                startButton.visible = true
                game_nav.visible = true
                console.log("Loading game...")
            }
        }

        RowLayout
        {
            id: game_nav
            visible: false
            spacing: 2
            Button
            {
                id: prevButton
                text: qsTr("Previous")
                onClicked:
                {
                    //logic.loadGame()
                    console.log("Previous move...")
                }
            }
            Button
            {
                id: nextButton
                text: qsTr("Next")
                onClicked:
                {
                    //logic.loadGame()
                    startButton.visible = true;
                    console.log("Next move...")
                }
            }
        }

        Button {
            id: stopButton
            visible: false
            text: qsTr("Stop")
            onClicked: {
                logic.clear()
                startButton.visible = true
                loadButton.visible = true
                stopButton.visible = false
                saveButton.visible = false
                console.log("Stoping game... Returning to starting screen")
            }
        }

        Button {
            id: saveButton
            visible: false
            text: qsTr("Save")
            onClicked: {
                //logic.saveGame()
                console.log("Saving game...")
            }
        }
    }
}
