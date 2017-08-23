import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow
{
    id: main
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600

    DSMStateMachine {
        id: ma_machine
        anchors.fill: parent
     }
}
