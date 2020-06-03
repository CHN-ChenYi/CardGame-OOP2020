#pragma once

#include <QWidget>

class MainWindow;

QT_BEGIN_NAMESPACE
class QPushButton;
class QHBoxLayout;
class QLabel;
class QVBoxLayout;
QT_END_NAMESPACE

class ContentWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ContentWidget(QWidget *parent = nullptr);
  virtual void SetInfo(const wchar_t *info) = 0;
  virtual ~ContentWidget(){};
 signals:
};

class HomeWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit HomeWidget(MainWindow *parent);
  ~HomeWidget();
  void SetInfo(const wchar_t *info) {}

 protected:
  void resizeEvent(QResizeEvent *event);

 private:
  QHBoxLayout *hlayout_;
  QVBoxLayout *vlayout_;
  QPushButton *new_game_button_;
  QPushButton *join_game_button_;
  QPixmap *bg_image_;
  void SetBackgroundImage();
};
