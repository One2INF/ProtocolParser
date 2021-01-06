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

  model->setHeaderData(0, Qt::Horizontal, "字段");
  model->setHeaderData(1, Qt::Horizontal, "字段名称");
  model->setHeaderData(2, Qt::Horizontal, "长度");
  model->setHeaderData(3, Qt::Horizontal, "值");
  model->setHeaderData(4, Qt::Horizontal, "备注");
*/

  model = new QStandardItemModel();
  model->setHorizontalHeaderItem(0, new QStandardItem("字段"));
  model->setHorizontalHeaderItem(1, new QStandardItem("字段名称"));
  model->setHorizontalHeaderItem(2, new QStandardItem("长度"));
  model->setHorizontalHeaderItem(3, new QStandardItem("值"));
  model->setHorizontalHeaderItem(4, new QStandardItem("备注"));

  ui->tableView->setModel(model);
  ui->tableView->setWindowTitle("pop");
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableView->horizontalHeader()->setMinimumSectionSize(50);
  ui->tableView->show();

  /* status bar */
  ui->statusBar->showMessage("hello protocolpaser : )");

  QObject::connect(ui->DoButton, SIGNAL(clicked()),
                   this, SLOT(slotParseProtocol()));

  QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                   this, SLOT(slotParseProtocol()));

  QObject::connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this, SLOT(slotSetJsonfile(QListWidgetItem*)));

  connect(ui->actionCheckJson, SIGNAL(triggered()), this, SLOT(slotCheckJson()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAboutMe()));

  cm_treeview = new QTreeView();
  setWindowTitle("ProtocolParser");
}

MainWindow::~MainWindow()
{
  delete ui;
  if(cm_treeview != nullptr)
    delete cm_treeview;
}

void MainWindow::slotParseProtocol() const
{

  QJsonValue jsonValue = SZJ_QJson::openJsonFile(":/default_protocol.json");
  if(jsonValue.isNull())
     ui->statusBar->showMessage("can't open or parse json file!");

  QString str = ui->lineEdit->text();
  str.remove(QRegExp("\\s"));

  ProtocolParseInterface parserInterface(str, &jsonValue);
  QList<QStringList> strArray = parserInterface.Parse2List();

  model->removeRows(0, model->rowCount());
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

void MainWindow::slotSetJsonfile(QListWidgetItem *item)
{
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
      ".", tr("json files (*.json)"));

  if(filePath != "")
    item->setText(QFileInfo(filePath).fileName());
}

void MainWindow::slotCheckJson()
{
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
      "E:\\Projects\\QT\\ProtocolParser", tr("json files (*.json)"));

  if(filePath.isEmpty())
  {
    qDebug() << "have not select a json file!";
    return;
  }

  QJsonValue jsonValue = szjQJson.openJsonFile(filePath);

  /* for test: to check if jasonvalue is valid */
  //szjQJson.travelJson(jsonValue);

  szjQJson.showJsonByTreeview(jsonValue, cm_treeview);

  cm_treeview->setWindowTitle("JSON Parser");
  cm_treeview->expandAll();
  cm_treeview->setMinimumWidth(500);
  cm_treeview->setMinimumHeight(500);
  cm_treeview->show();
}

void MainWindow::slotAboutMe()
{
  QMessageBox::about(nullptr, "About Me", "Author: One2INF\r\n"
                                          "gitee: https://gitee.com/One2INF\r\n"
                                          "Email: 1871750676@qq.com");
}
