#pragma once

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject {
    Q_OBJECT

public:
    explicit TcpClient(QObject* parent = nullptr);
    ~TcpClient();

    void start(const QString& address, quint16 port);
    void stop();
    void sendCommand(const QString& command);

signals:
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);

private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket* socket;
    int connectionWaitingTime;
};
