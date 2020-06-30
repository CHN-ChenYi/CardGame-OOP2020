#include <QApplication>
#include "MainWindow.h"
#include "Network/include/MeyaS.h"
MainWindow *window;

int main(int argc, char *argv[]) {
  MeyaS::initialize();
  QApplication app(argc, argv);
  window = new MainWindow();
  window->show();
  return app.exec();
}
