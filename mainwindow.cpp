#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocolpase.h"
#include "szj_qjson.h"

#include <QTableWidgetItem>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>

QStandardItemModel *model = new QStandardItemModel();

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
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableView->horizontalHeader()->setMinimumSectionSize(50);
  ui->tableView->show();

  QObject::connect(ui->DoButton, SIGNAL(clicked()),
                   this, SLOT(slotParseProtocol()));

  QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                   this, SLOT(slotParseProtocol()));

  connect(ui->actionCheckJson, SIGNAL(triggered()), this, SLOT(slotCheckJson()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAboutMe()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotParseProtocol() const
{
  QString str = ui->lineEdit->text();
  str.remove(QRegExp("\\s"));

  ProtocolParseInterface pi(str);
  QList<QStringList> strArray = pi.Parse(str);

  qint8 row = 0, column = 0;
  for(QStringList strlist : strArray)
  {
    column = 0;
    qDebug() << strlist;
    for(QString str : strlist)
    {
      model->setItem(row, column, new QStandardItem(str));
      ++column;
    }
    ++row;
  }
}

void MainWindow::slotCheckJson()
{
  cm_strDefaultOpenPath = "E:\\Projects\\QT\\ProtocolParser";
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
      cm_strDefaultOpenPath, tr("json files (*.json)"));

  if(filePath.isEmpty())
  {
    qDebug() << "have not select a json file!";
    return;
  }
  //QJsonValue jsonValue = SZJ_QJson::openJsonFile("E:\\Projects\\QT\\ProtocolParser\\test_protocol.json");
  QJsonValue jsonValue = SZJ_QJson::openJsonFile(filePath);
  qDebug() << jsonValue.type();
  //SZJ_QJson::travelJson(jsonValue);

  QTreeView *treeview = new QTreeView;
  SZJ_QJson::showJsonByTreeview(jsonValue, treeview);

  treeview->setWindowTitle("JSON Parser");
  treeview->expandAll();
  treeview->setMinimumWidth(500);
  treeview->setMinimumHeight(500);
  treeview->show();
}

void MainWindow::slotAboutMe()
{
  QMessageBox::about(nullptr, "About Me", "Author: One2INF\r\n"
                                          "gitee: https://gitee.com/One2INF\r\n"
                                          "Email: 1871750676@qq.com");
}
