#include "MainWindow.h"

#include <QDebug>
#include <QtWidgets>

#include "ContentWidget.h"

MainWindow::MainWindow() {
  content_ = new HomeWidget(this);
  setCentralWidget(content_);

  QAction *home_act = new QAction("Home", this);
  connect(home_act, &QAction::triggered, this, &MainWindow::Home);

  QAction *exit_act = new QAction("Exit", this);
  // exit_act->setShortcuts(QKeySequence::Quit);
  // exit_act->setStatusTip("Exit the application");
  connect(exit_act, &QAction::triggered, this, &QWidget::close);

  QAction *manual_act = new QAction("Manual", this);
  connect(manual_act, &QAction::triggered, this, &MainWindow::Manual);

  QAction *about_act = new QAction("About", this);
  connect(about_act, &QAction::triggered, this, &MainWindow::About);

  QAction *about_qt_act = new QAction("About Qt", this);
  connect(about_qt_act, &QAction::triggered, qApp, &QApplication::aboutQt);

  QMenu *file_menu = menuBar()->addMenu("File");
  file_menu->addAction(home_act);
  file_menu->addAction(exit_act);
  QMenu *help_menu = menuBar()->addMenu("Help");
  help_menu->addAction(manual_act);
  help_menu->addAction(about_act);
  help_menu->addAction(about_qt_act);

  // QString message = tr("Status tip");
  // statusBar()->showMessage(message);

  setWindowTitle("Title");
  setMinimumSize(480, 320);
  resize(480, 320);
}

void MainWindow::Home() { ::Home(); }

void MainWindow::About() {
  QMessageBox::about(this, "About", "一个面向对象课程中完成的纸牌游戏");
}

void MainWindow::Manual() { QMessageBox::about(this, "Manual", "TODO"); }

void MainWindow::NewGame() {
  delete content_;
  content_ = new InitOrJoinWidget(this, 0);
  setCentralWidget(content_);
  qDebug() << "new Game";
}

void MainWindow::JoinGame() {
  delete content_;
  content_ = new InitOrJoinWidget(this, 1);
  setCentralWidget(content_);
  qDebug() << "join Game";
}

void MainWindow::DrawHomePage() {
  delete content_;
  content_ = new HomeWidget(this);
  setCentralWidget(content_);
}

void MainWindow::SetInfo(const wstring &info) { content_->SetInfo(info); }

void MainWindow::DrawWaitingPage(const GameType type, const wstring &ip,
                                 bool is_owner) {
  delete content_;
  content_ = new WaitWidget(this, type, ip, is_owner);
  setCentralWidget(content_);
}

bool MainWindow::AddPlayer(const unsigned short id, const wstring &player_name,
                           const double network_status) {
  WaitWidget *cur_widget = dynamic_cast<WaitWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->AddPlayer(id, player_name, network_status);
}

bool MainWindow::RemovePlayer(const unsigned short id) {
  WaitWidget *cur_widget = dynamic_cast<WaitWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->RemovePlayer(id);
}

bool MainWindow::SetNetworkStatus(const unsigned short id,
                                  const double network_status) {
  WaitWidget *cur_widget = dynamic_cast<WaitWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->SetNetworkStatus(id, network_status);
}

void MainWindow::DrawPlayingPage(const GameType type,
                                 const wstring (&player_name)[4],
                                 const unsigned short number_of_cards[4],
                                 const double network_status[4],
                                 const bool controlled_by_bot[4],
                                 const Card cards[]) {
  delete content_;
  content_ = new PlayWidget(this, type, player_name, number_of_cards,
                            network_status, controlled_by_bot, cards);
  setCentralWidget(content_);
}
