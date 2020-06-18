#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocolpase.h"

#include <QStandardItemModel>
#include <QTableWidgetItem>

QStandardItemModel* model = new QStandardItemModel();

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);



/*
 * QAbstractItemModel::setHeaderData is meant to handle user edits of the header,
 * not setting them programmatically.
 * The model then must implement setHeaderData to handle those edit requests.
 * QStandardItemModel apparently doesn't create header items if they weren't set initially,
 * so nothing happens.
 *
 * As you're using QStandardItemModel,
 * you should use setHorizontalHeaderItem or setHorizontalHeaderLabels (Vertical variants are also available) to set up the header

  model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("字段"));
  model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("字段名称"));
  model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("长度"));
  model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("值"));
  model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("备注"));
*/

  model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("字段")));
  model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromLocal8Bit("字段名称")));
  model->setHorizontalHeaderItem(2, new QStandardItem(QString::fromLocal8Bit("长度")));
  model->setHorizontalHeaderItem(3, new QStandardItem(QString::fromLocal8Bit("值")));
  model->setHorizontalHeaderItem(4, new QStandardItem(QString::fromLocal8Bit("备注")));

  ui->tableView->setModel(model);
  ui->tableView->setWindowTitle("pop");
  ui->tableView->show();

  QObject::connect(ui->DoButton, SIGNAL(clicked()),
                   this, SLOT(slotParseProtocol()));

  QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                   this, SLOT(slotParseProtocol()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotParseProtocol() const
{
  QString str = ui->lineEdit->text();
  ProtocolParseInterface pi(str);
  QList<QStringList> strArray = pi.Parse(str);
  qint8 row = 0, column = 0;
  for(QStringList strlist : strArray)
  {
    ++row;
    for(QString str : strlist)
    {
      qDebug() << str;
      model->setItem(row, column, new QStandardItem(str));
      ++column;
    }
  }

}
