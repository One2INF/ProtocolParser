﻿#include "protocolpase.h"

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QTreeView>
#include <QStandardItem>
#include <QFile>

#include "szj_qjson.h"

/* base class */
ProtocolParse::ProtocolParse()
{
  qDebug() << "ProtocolPase Constructor";
}

ProtocolParse::~ProtocolParse()
{
  qDebug() << "ProtocolPase Destructor";
}

QList<QStringList> ProtocolParse::Parse(QString strProtocol)
{
  qDebug() << "ProtocolPase none:" << strProtocol;
  QList<QStringList> strArray;
  return strArray;
}

/* derived class */
ProtocolPaseFFJAA19::ProtocolPaseFFJAA19()
{
  qDebug() << "ProtocolPaseFFJAA19 Constructor";
}

ProtocolPaseFFJAA19::~ProtocolPaseFFJAA19()
{
  qDebug() << "ProtocolPaseFFJAA19 Destructor";
}

QList<QStringList> ProtocolPaseFFJAA19::Parse(QString strProtocol)
{
  QList<QStringList> strArray;

  /* use for test, delete later */
  QJsonValue jsonValue = SZJ_QJson::openJsonFile("E:\\Projects\\QT\\ProtocolParser\\test_protocol.json");
  QJsonArray infoArray = jsonValue["AA19"]["INFO"].toArray();
  QJsonObject relationObject = jsonValue["AA19"]["REALATION"].toObject();

  QJsonObject jsonParsed;
  for(auto it : infoArray)
  {
    QJsonArray tempArray = it.toArray();
    /* if is string, means it's length is up to other field
     * and the string show the field name.
     */
    if(tempArray[2].isString())
    {
      QString fieldName = tempArray[2].toString();
      if(!jsonParsed.contains(fieldName))
        qDebug() << "Error: cannot find parent field!";

      QString typeName = tempArray[0].toString();
      QString fieldLength = jsonParsed[typeName].toString();

      /* search by typename */
      if(relationObject.contains(typeName))
      {
        double datalength = relationObject[typeName].toObject()[fieldLength].toDouble();
        QJsonArray Array;
        Array.append(tempArray[1]);
        Array.append(datalength);
        jsonParsed[tempArray[0].toString()] = Array;
      }
      else
        qDebug() << "Error: search failed!";
    }
    else if(tempArray[2].isDouble())
    {
      QJsonArray Array;
      Array.append(tempArray[1]);
      Array.append(tempArray[2]);
      jsonParsed[tempArray[0].toString()] = Array;
    }
    else
      qDebug() << "Error: element type not supported!";
  }
  qDebug() << jsonParsed;
  return strArray;
}

QList<QStringList> ProtocolPaseFFJDefault::Parse(QString strProtocol)
{
  qDebug() << "Parse Default: " << strProtocol;
  QList<QStringList> strArray;
  return strArray;
}

/* factory interface class */
ProtocolParseInterface::ProtocolParseInterface(QString strProtocol)
{
  QString strType = strProtocol.mid(0, 4);
  if("AA19" == strType)
  {
    pp = new ProtocolPaseFFJAA19();
  }
  else
  {
    pp = new ProtocolPaseFFJDefault();
  }
  qDebug() << "ProtocolParseInterface Constructor";
}

ProtocolParseInterface::~ProtocolParseInterface()
{
  qDebug() << "ProtocolParseInterface Destructor";
  if(pp)
  {
    delete pp;
  }
}

QList<QStringList> ProtocolParseInterface::Parse(QString str)
{
  return  pp->Parse(str);;
}


