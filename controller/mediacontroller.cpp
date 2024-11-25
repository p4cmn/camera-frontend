#include "mediacontroller.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>

MediaController::MediaController(MediaView* view, TcpClient* client, QObject* parent)
    : QObject(parent), view(view), client(client) {

    connect(view, &MediaView::commandEntered, this, &MediaController::handleCommand);
    connect(view, &MediaView::exitRequested, this, &MediaController::handleExit);

    connect(client, &TcpClient::dataReceived, this, &MediaController::handleDataReceived);
    connect(client, &TcpClient::errorOccurred, this, &MediaController::handleError);

    connect(this, &MediaController::displayMessage, view, &MediaView::displayMessage);
    connect(this, &MediaController::displayError, view, &MediaView::displayError);
}

void MediaController::handleCommand(const QString& command) {
    QStringList parts = command.trimmed().split(' ');
    if (parts.isEmpty()) {
        return;
    }
    QString cmd = parts.first();
    QStringList args = parts.mid(1);

    if (cmd == "start") {
        if (args.size() < 2) {
            emit displayError("Usage: start <address> <port>");
            return;
        }
        QString address = args.at(0);
        quint16 port = args.at(1).toUShort();
        client->start(address, port);
        emit displayMessage("Client started.");
    } else if (cmd == "stop") {
        client->stop();
        emit displayMessage("Client stopped.");
    } else if (cmd == "help") {
        showHelp();
    } else if (cmd == "exit") {
        handleExit();
    } else {
        client->sendCommand(command);
    }
}

void MediaController::handleDataReceived(const QByteArray& data) {
    static QByteArray buffer;
    static QString currentFileName;
    static qint64 expectedFileSize = 0;
    static QByteArray fileBuffer;
    buffer.append(data);
    while (!buffer.isEmpty()) {
        if (expectedFileSize > 0) {
            qint64 remainingBytes = expectedFileSize - fileBuffer.size();
            qint64 bytesToRead = qMin(remainingBytes, static_cast<qint64>(buffer.size()));
            fileBuffer.append(buffer.left(bytesToRead));
            buffer.remove(0, bytesToRead);
            if (fileBuffer.size() == expectedFileSize) {
                QString savePath =  "D:/PROGRAMMING/C++/QT/Camera-frontend/DATA/" + currentFileName;
                QFile file(savePath);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(fileBuffer);
                    file.close();
                    emit displayMessage("File received and saved: " + savePath);
                } else {
                    emit displayError("Failed to save file: " + savePath);
                }
                currentFileName.clear();
                expectedFileSize = 0;
                fileBuffer.clear();
            }
        } else {
            int headerEndIndex = buffer.indexOf('\n');
            if (headerEndIndex == -1) {
                break;
            }
            QByteArray headerData = buffer.left(headerEndIndex);
            buffer.remove(0, headerEndIndex + 1);
            QString header = QString::fromUtf8(headerData).trimmed();
            if (header.startsWith("FILE:")) {
                QStringList parts = header.split(':');
                if (parts.size() != 3) {
                    emit displayError("Invalid file header received");
                    continue;
                }
                currentFileName = parts[1].trimmed();
                bool ok;
                expectedFileSize = parts[2].trimmed().toLongLong(&ok);
                if (!ok) {
                    emit displayError("Invalid file size in header");
                    currentFileName.clear();
                    expectedFileSize = 0;
                    continue;
                }
            } else {
                emit displayMessage("Server: " + header);
            }
        }
    }
}

void MediaController::handleError(const QString& error) {
    emit displayError("Client error: " + error);
}

void MediaController::handleExit() {
    client->stop();
    emit displayMessage("Exiting application.");
    QCoreApplication::exec();
}

void MediaController::processCommand(const QString& cmd, const QStringList& args) {
    Q_UNUSED(cmd)
    Q_UNUSED(args)
}

void MediaController::showHelp() {
    QString helpText =
        "Available commands:\n"
        "start <address> <port>         - Start client and connect to server\n"
        "stop                           - Stop client\n"
        "help                           - Show this help message\n"
        "get_info_from_all              - Get info from all cameras\n"
        "get_photo_from_all             - Get photos from all cameras\n"
        "get_video_from_all             - Get videos from all cameras\n"
        "get_svideo_from_all            - Get videos with sound from all cameras\n"
        "exit                           - Exit the application";
    emit displayMessage(helpText);
}
