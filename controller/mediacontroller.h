#pragma once

#include <QObject>
#include "view/mediaview.h"
#include "client/tcpclient.h"

class MediaController : public QObject {
    Q_OBJECT

public:
    explicit MediaController(MediaView* view, TcpClient* client, QObject* parent = nullptr);

private slots:
    void handleCommand(const QString& command);
    void handleDataReceived(const QByteArray& data);
    void handleError(const QString& error);
    void handleExit();

signals:
    void displayMessage(const QString& message);
    void displayError(const QString& error);

private:
    MediaView* view;
    TcpClient* client;

    void processCommand(const QString& cmd, const QStringList& args);
    void showHelp();
};
