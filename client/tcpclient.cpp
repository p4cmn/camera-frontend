#include "tcpclient.h"
#include <QDebug>

TcpClient::TcpClient(QObject* parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
    , connectionWaitingTime(5000) {

    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TcpClient::onErrorOccurred);
}

TcpClient::~TcpClient() {
    stop();
}

void TcpClient::start(const QString& address, quint16 port) {
    if (socket->isOpen()) {
        emit errorOccurred("Client is already connected.");
        return;
    }

    socket->connectToHost(address, port);

    if (!socket->waitForConnected(connectionWaitingTime)) {
        QString errorMsg = "Failed to connect to server: " + socket->errorString();
        emit errorOccurred(errorMsg);
        qDebug() << errorMsg;
    } else {
        qDebug() << "Connected to server at" << address << ":" << port;
    }
}

void TcpClient::stop() {
    if (socket->isOpen()) {
        socket->close();
        qDebug() << "Client disconnected.";
    }
}

void TcpClient::sendCommand(const QString& command) {
    if (!socket->isOpen()) {
        QString errorMsg = "Cannot send data. Socket is not connected.";
        emit errorOccurred(errorMsg);
        qDebug() << errorMsg;
        return;
    }

    QByteArray data = command.toUtf8() + "\n";
    socket->write(data);
    socket->flush();
    qDebug() << "Command sent to server:" << command;
}

void TcpClient::onReadyRead() {
    QByteArray data = socket->readAll();
    emit dataReceived(data);
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    QString errorMsg = "Socket error: " + socket->errorString();
    emit errorOccurred(errorMsg);
    qDebug() << errorMsg;
}
