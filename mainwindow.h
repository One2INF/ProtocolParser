#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "szj_qjson.h"

#include <QMainWindow>
#include <QDebug>
#include <QStandardItemModel>
#include <QListWidgetItem>
#include <QTreeView>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow *ui;
  QStandardItem *qstdmodel;
  QStandardItemModel *model;
  QMenu *fileMenu;
  QTreeView* cm_treeview;
  SZJ_QJson szjQJson;
  QList<QJsonValue> jsonvalueList;

public slots:

private slots:
  void slotCheckJson();
  void slotAboutMe();
  void slotParseProtocol() const;
  void slotSetJsonfile(QListWidgetItem *item);
};

#endif // MAINWINDOW_H
