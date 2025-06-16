// SPDX-License-Identifier: GPL-2.0
#include <ui/CMainWindow.hpp>

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
