#include "szj_qjson.h"

#include <QFile>
#include <QDebug>

SZJ_QJson::SZJ_QJson()
{
  cm_model = new QStandardItemModel();
}

SZJ_QJson::~SZJ_QJson()
{
  if(cm_model != nullptr)
    delete cm_model;
}

QJsonValue SZJ_QJson::openJsonFile(QString strJsonFilePath)
{
  QFile jsonFile(strJsonFilePath);
  if(!jsonFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "could't open default_protocol.json ";
    return QJsonValue::Null;
  }

  QByteArray allData = jsonFile.readAll();
  jsonFile.close();

  QJsonParseError jsonError;
  QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
  if(jsonError.error != QJsonParseError::NoError)
  {
    qDebug() << "json parse error:" << jsonError.error;
    return QJsonValue::Null;
  }

  if(jsonDoc.isObject())
    return jsonDoc.object();
  else if(jsonDoc.isArray())
    return jsonDoc.array();
  else if(jsonDoc.isNull())
    return QJsonValue::Null;
  else
  {
    qDebug() << "program should not run here!";
    return QJsonValue::Null;
  }
}

void SZJ_QJson::travelJson(const QJsonValue& cJsonValue)
{
  QJsonValue::Type jsonType = cJsonValue.type();
  switch(jsonType)
  {
    case QJsonValue::Array :
    {
      for(auto it : cJsonValue.toArray())
      {
        travelJson(it);
      }
      break;
    }
    case QJsonValue::Object :
    {
      for(auto it : cJsonValue.toObject().keys())
      {
        travelJson(cJsonValue[it]);
      }
      break;
    }

    case QJsonValue::String :
    {
      qDebug() << cJsonValue.toString();
      break;
    }

    case QJsonValue::Double :
    {
      qDebug() << cJsonValue.toDouble();
      break;
    }

    default :
    {
      qDebug() << "NULL Type!!!";
      break;
    }
  }
}

void SZJ_QJson::createJsonItem(const QJsonValue& cjsonValue, QStandardItem* parentItem)
{
  QJsonValue::Type jsonType = cjsonValue.type();
  switch(jsonType)
  {
    case QJsonValue::Array :
    {
      QStandardItem *childItem = new QStandardItem();
      parentItem->appendRow(childItem);

      for(auto it : cjsonValue.toArray())
      {
        createJsonItem(it, childItem);
      }
      break;
    }

    case QJsonValue::Object :
    {
      for(auto it : cjsonValue.toObject().keys())
      {
        QStandardItem *childItem = new QStandardItem(it);
        parentItem->appendRow(childItem);
        createJsonItem(cjsonValue[it], childItem);
      }
      break;
    }

    case QJsonValue::String :
    {
      qDebug() << cjsonValue.toString();
      parentItem->appendRow(new QStandardItem(cjsonValue.toString()));
      break;
    }

    case QJsonValue::Double :
    {
      qDebug() << cjsonValue.toDouble();
      parentItem->appendRow(new QStandardItem(QString::number(cjsonValue.toDouble())));
      break;
    }

    default :
    {
      qDebug() << "NULL Type!!!";
      break;
    }
  }
}

void SZJ_QJson::showJsonByTreeview(const QJsonValue& jsonValue, QTreeView* treeView)
{
  QStandardItem* cm_rootItem = new QStandardItem("root");
  createJsonItem(jsonValue, cm_rootItem);
  cm_model->appendRow(cm_rootItem);
  treeView->setModel(cm_model);
}
