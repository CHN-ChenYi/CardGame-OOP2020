#pragma once

#include <QWidget>

class MainWindow;

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class ContentWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ContentWidget(QWidget *parent = nullptr);

 signals:
};

class InitWidget : public ContentWidget {
  Q_OBJECT

 public:
  InitWidget(MainWindow *parent);

 private:
  void CreateButtons(MainWindow *parent);
  QPushButton *new_game_button_;
  QPushButton *join_game_button_;
};
