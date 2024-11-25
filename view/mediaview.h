#pragma once

#include <QObject>
#include <QTextStream>
#include "inputthread.h"

class MediaView : public QObject {
    Q_OBJECT

public:
    explicit MediaView(QObject* parent = nullptr);
    ~MediaView();

    void start();

signals:
    void commandEntered(const QString& command);
    void exitRequested();

public slots:
    void displayMessage(const QString& message);
    void displayError(const QString& error);

private:
    QTextStream output;
    InputThread* inputThread;

private slots:
    void handleCommand(const QString& command);
};
