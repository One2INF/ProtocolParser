#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QStandardItemModel* model = new QStandardItemModel(this);
  model->setItem(0, 0, new QStandardItem(QString::fromLocal8Bit("张三")));
  model->setItem(0, 1, new QStandardItem("3"));
  model->setItem(0, 2, new QStandardItem(QString::fromLocal8Bit("名字")));
  model->setItem(0, 3, new QStandardItem(QString::fromLocal8Bit("男")));

  ui->tableView->setModel(model);
  ui->tableView->setWindowTitle("pop");
  ui->tableView->show();
}

MainWindow::~MainWindow()
{
  delete ui;
}
