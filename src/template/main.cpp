#include <QApplication>

#include "MainWindow.h"

MainWindow *window;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  window = new MainWindow();
  window->show();
  return app.exec();
}
