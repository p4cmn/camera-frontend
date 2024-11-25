#pragma once

#include <QThread>
#include <QString>

class InputThread : public QThread {
    Q_OBJECT

public:
    explicit InputThread(QObject* parent = nullptr);
    void run() override;

signals:
    void commandRead(const QString& command);
};
