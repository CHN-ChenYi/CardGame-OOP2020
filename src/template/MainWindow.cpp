#include "MainWindow.h"

#include <QtWidgets>

#include "ContentWidget.h"

MainWindow::MainWindow() {
  content_ = new InitWidget(this);
  setCentralWidget(content_);

  CreateActions();
  CreateMenus();

  // QString message = tr("Status tip");
  // statusBar()->showMessage(message);

  setWindowTitle("Title");
  setMinimumSize(160, 160);
  resize(480, 320);
}

void MainWindow::CreateActions() {
  home_act_ = new QAction("Home", this);
  connect(home_act_, &QAction::triggered, this, &MainWindow::Home);

  exit_act_ = new QAction("Exit", this);
  exit_act_->setShortcuts(QKeySequence::Quit);
  // exit_act_->setStatusTip("Exit the application");
  connect(exit_act_, &QAction::triggered, this, &QWidget::close);

  manual_act_ = new QAction("Manual", this);
  connect(manual_act_, &QAction::triggered, this, &MainWindow::Manual);

  about_act_ = new QAction("About", this);
  connect(about_act_, &QAction::triggered, this, &MainWindow::About);

  about_qt_act_ = new QAction("About Qt", this);
  connect(about_qt_act_, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::CreateMenus() {
  file_menu_ = menuBar()->addMenu("File");
  file_menu_->addAction(home_act_);
  file_menu_->addAction(exit_act_);
  help_menu_ = menuBar()->addMenu("Help");
  help_menu_->addAction(manual_act_);
  help_menu_->addAction(about_act_);
  help_menu_->addAction(about_qt_act_);
}

void MainWindow::Home() { qDebug() << "home"; }

void MainWindow::About() {
  QMessageBox::about(this, "About", "一个面向对象课程中完成的纸牌游戏");
}

void MainWindow::Manual() { QMessageBox::about(this, "Manual", "TODO"); }

void MainWindow::NewGame() {
  qDebug() << "new Game";
  content_->close();
  delete content_;

  content_ = new InitWidget(this);
  setCentralWidget(content_);
}
void MainWindow::JoinGame() { qDebug() << "join Game"; }
