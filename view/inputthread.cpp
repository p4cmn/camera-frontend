#include "inputthread.h"
#include <QTextStream>
#include <QMutexLocker>

InputThread::InputThread(QObject* parent)
    : QThread(parent) {}

void InputThread::run() {
    QTextStream input(stdin);
    while (!isInterruptionRequested()) {
        if (input.atEnd()) {
            msleep(100);
            continue;
        }
        QString command = input.readLine();
        if (command.isNull()) {
            break;
        }
        emit commandRead(command.trimmed());
    }
}
