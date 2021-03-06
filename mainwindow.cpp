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

  QObject::connect(ui->AddButton, SIGNAL(clicked()),
                   this, SLOT(slotAddListWidgetItem()));
  QObject::connect(ui->DeleteButton, SIGNAL(clicked()),
                   this, SLOT(slotDeleteListWidgetItem()));

  QObject::connect(ui->lineEdit, SIGNAL(returnPressed()),
                   this, SLOT(slotParseProtocol()));

  QObject::connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this, SLOT(slotSetJsonfile(QListWidgetItem*)));

  QObject::connect(ui->actionCheckJson, SIGNAL(triggered()),
                   this, SLOT(slotCheckJson()));
  QObject::connect(ui->actionAbout, SIGNAL(triggered()),
                   this, SLOT(slotAboutMe()));

  cm_treeview = new QTreeView();
  setWindowTitle("ProtocolParser");
}

void MainWindow::slotAddListWidgetItem()
{
  ui->listWidget->addItem(new QListWidgetItem(QString("新建项目%1").arg(ui->listWidget->count())));
  jsonvalueList << QJsonValue::Null;
}

void MainWindow::slotDeleteListWidgetItem()
{
  if(ui->listWidget->selectedItems().isEmpty())
  {
    ui->statusBar->showMessage("please select a item to be deleted!");
    return;
  }

  /* delete it in listWidget and jsonvalueList */
  for(auto item : ui->listWidget->selectedItems())
  {
    int row = ui->listWidget->row(item);
    ui->listWidget->takeItem(row);
    jsonvalueList.removeAt(row);
  }

  ui->statusBar->showMessage("item deleted!");
}

void MainWindow::slotParseProtocol() const
{
  QString str = ui->lineEdit->text();
  str.remove(QRegExp("\\s"));

  QList<QStringList> strArray;
  if(jsonvalueList.isEmpty())
  {
    ui->statusBar->showMessage("no json file select, use default json file!");
    QJsonValue jsonValue = SZJ_QJson::openJsonFile(":/default_protocol.json");
    ProtocolParseInterface parserInterface(str, &jsonValue);
    strArray = parserInterface.Parse2List();
  }
  else
  {
    for(QJsonValue jsonValue : jsonvalueList)
    {
      ProtocolParseInterface parserInterface(str, &jsonValue);
      strArray = parserInterface.Parse2List();
      if(!strArray.isEmpty())
      {
        ui->statusBar->showMessage("text parsed!");
        break;
      }
    }
  }

  if(strArray.isEmpty())
  {
    ui->statusBar->showMessage("invalid text!");
    return;
  }

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
  ui->statusBar->showMessage("please select a json file!");

  QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
      ".", tr("json files (*.json)"));

  if(filePath == "")
    return;

  item->setText(QFileInfo(filePath).fileName());

  QJsonValue jsonValue = szjQJson.openJsonFile(filePath);
  if(jsonValue.isNull())
     ui->statusBar->showMessage("can't open or parse json file!");
  else
  {
    ui->statusBar->showMessage("selected json file parsed!");
    int row = ui->listWidget->row(item);
    jsonvalueList[row] = jsonValue;
  }
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
  QString msgAuthor = QString("Author: One2INF (咸个和和和)<p>");
  QString msgGitee = QString("Gitee: <a href=https://gitee.com/One2INF>https://gitee.com/One2INF</a><p>");
  QString msgEmail = QString("Email: <a href=1871750676@qq.com>1871750676@qq.com</a><p>");
  QString msg = msgAuthor + msgGitee + msgEmail;

  QMessageBox::about(nullptr, "About Me", msg);
}

MainWindow::~MainWindow()
{
  delete ui;
  if(cm_treeview != nullptr)
    delete cm_treeview;
}
