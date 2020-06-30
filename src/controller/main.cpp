#include <QApplication>
#include "src/network/include/MeyaS.h"
#include "src/template/MainWindow.h"
MainWindow *window;

int main(int argc, char *argv[]) {
  MeyaS::initialize();
  QApplication app(argc, argv);
  window = new MainWindow();
  window->show();
  return app.exec();
}
