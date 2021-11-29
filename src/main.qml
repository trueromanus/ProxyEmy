import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import ProxyEmy.Backend

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: backend.windowTitle

    ProxyEmyBackend {
        id: backend
    }

    HttpProxyServer {
        id: httpProxyServer
        Component.onCompleted: {
            httpProxyServer.startServer();
        }
    }

    ConfigurationViewModel {
        id: configurationViewModel
    }

    background: Rectangle {
        anchors.fill: parent
        color: "green"
    }

    Component.onDestruction: {
        httpProxyServer.stopServer();
    }
}
