#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   MainWindow w(nullptr, app);

   w.show();

   return app.exec();
}
