#include "mediaview.h"
#include <QCoreApplication>

MediaView::MediaView(QObject* parent)
    : QObject(parent), output(stdout) {

    inputThread = new InputThread(this);
    connect(inputThread, &InputThread::commandRead, this, &MediaView::handleCommand);
    connect(this, &MediaView::exitRequested, inputThread, &InputThread::quit);
}

MediaView::~MediaView() {
    inputThread->requestInterruption();
    inputThread->quit();
    inputThread->wait();
}

void MediaView::start() {
    output << "Welcome to the TCP Client CLI!\n";
    output << "Enter 'exit' to quit.\n";
    output << "> ";
    output.flush();

    inputThread->start();
}

void MediaView::handleCommand(const QString& command) {
    if (command.isEmpty()) {
        output << "> ";
        output.flush();
        return;
    }
    if (command == "exit") {
        emit exitRequested();
    } else {
        emit commandEntered(command);
    }
    output << "> ";
    output.flush();
}

void MediaView::displayMessage(const QString& message) {
    output << message << "\n";
    output.flush();
}

void MediaView::displayError(const QString& error) {
    output << "Error: " << error << "\n";
    output.flush();
}
