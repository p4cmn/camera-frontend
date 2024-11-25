#include <QCoreApplication>
#include "view/mediaview.h"
#include "client/tcpclient.h"
#include "controller/mediacontroller.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    MediaView view;
    TcpClient client;
    MediaController controller(&view, &client);

    view.start();

    return app.exec();
}
