#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QStandardItemModel>
#include <QTreeView>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QStandardItem *qstdmodel;
  QMenu *fileMenu;
  QString cm_strDefaultOpenPath;
  QTreeView *view;

public slots:

private slots:
  void slotCheckJson();
  void slotAboutMe();
  void slotParseProtocol() const;
};

#endif // MAINWINDOW_H
