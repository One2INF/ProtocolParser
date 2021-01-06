#include "protocolpase.h"

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QTreeView>
#include <QStandardItem>
#include <QFile>

#include "szj_qjson.h"

typedef enum
{
  IDX_FIELD_NAME = 0,
  IDX_FIELD_DESCRIPTION,
  IDX_FIELD_LENGTH,
  IDX_FIELD_DATA
}IDX_EN;

typedef struct
{
  QString fieldName;
  QString fieldDescription;
  double fieldLength;
  QString filedData;
}SECTION_UNIT_ST;

/* base class */
ProtocolParser::ProtocolParser()
{
  qDebug() << "ProtocolPase Constructor";
}

ProtocolParser::~ProtocolParser()
{
  qDebug() << "ProtocolPase Destructor";
}

QList<QStringList> ProtocolParser::Parse2List(QString str, QJsonValue* jsonValue)
{
  //TODO
  qDebug() << "ProtocolPase none:" << str;
  (void)jsonValue;
  QList<QStringList> strArray;
  return strArray;
}

QJsonValue ProtocolParser::Parse2Json(QString str, QJsonValue *jsonValue)
{
  //TODO
  qDebug() << "ProtocolPase none:" << str;

  return *jsonValue;
}

/* derived class */
ProtocolPaser_CunKou::ProtocolPaser_CunKou()
{
  qDebug() << "ProtocolPase_CunKou Constructor";
}

ProtocolPaser_CunKou::~ProtocolPaser_CunKou()
{
  qDebug() << "ProtocolPase_CunKou Destructor";
}

QList<QStringList> ProtocolPaser_CunKou::Parse2List(QString str, QJsonValue* jsonValue)
{
  QJsonObject jsonObj = jsonValue->toObject();
  QJsonArray sectionArray = jsonObj[".SECTION"].toArray();
  QJsonObject relationObject = jsonObj[".REALATION"].toObject();

  /* for parsing: to facilitate the query */
  QJsonArray jsonArrayParsed;
  /* for save */
  QJsonObject jsonObjParsed;

  SECTION_UNIT_ST unit;

  /* 遍历 section 数组，获取字段信息 */
  for(auto it : sectionArray)
  {
    QJsonArray unitSection = it.toArray();
    unit.fieldName = unitSection[IDX_FIELD_NAME].toString();
    unit.fieldDescription = unitSection[IDX_FIELD_DESCRIPTION].toString();
    unit.fieldLength = 0;
    unit.filedData = "";

    /* if FIELD_LENGTH is string, means it's length is up to the other field
     * and the string show the field name it depend on(parent name).
     */
    if(unitSection[IDX_FIELD_LENGTH].isString())
    {
      QString parentName = unitSection[IDX_FIELD_LENGTH].toString();
      if(!jsonObjParsed.contains(parentName))
      {
        qDebug() << "Error: cannot find parent field! Check the section unit order!";
        break;
      }

      QString parentData = jsonObjParsed[parentName].toArray()[2].toString();

      /* get field length in relation section */
      if(relationObject.contains(unit.fieldName))
      {
        unit.fieldLength = relationObject[unit.fieldName].toObject()[parentData].toDouble();
      }
      else
      {
        qDebug() << "Error: search failed!";
        break;
      }
    }
    else if(unitSection[IDX_FIELD_LENGTH].isDouble())
    {
      unit.fieldLength = unitSection[IDX_FIELD_LENGTH].toDouble();
    }
    else
    {
      qDebug() << "Error: element type not supported!";
      break;
    }

    unit.filedData = str.left(static_cast<int>(unit.fieldLength));

    /* add to jsonObjParsed and jsonArrayParsed */
    QJsonArray Array;
    Array.append(unit.fieldDescription);
    Array.append(unit.fieldLength);
    Array.append(unit.filedData);
    jsonObjParsed[unit.fieldName] = Array;

    Array.push_front(unit.fieldName);
    jsonArrayParsed.append(Array);

    str = str.right(str.size() - static_cast<int>(unit.fieldLength));
  }

  QList<QStringList> strArray;

  for(auto it : jsonArrayParsed)
  {
    QJsonArray jsonArray = it.toArray();
    QStringList strList;
    strList << jsonArray[IDX_FIELD_NAME].toString() << jsonArray[IDX_FIELD_DESCRIPTION].toString();
    strList << QString::number(jsonArray[IDX_FIELD_LENGTH].toDouble()) << jsonArray[IDX_FIELD_DATA].toString();
    strArray << strList;
  }

  return strArray;
}

QJsonValue ProtocolPaser_CunKou::Parse2Json(QString str, QJsonValue* jsonValue)
{
  QJsonObject jsonObj = jsonValue->toObject();
  QJsonArray sectionArray = jsonObj[".SECTION"].toArray();
  QJsonObject relationObject = jsonObj[".REALATION"].toObject();

  /* for parsing: to facilitate the query */
  QJsonArray jsonArrayParsed;
  /* for save */
  QJsonObject jsonObjParsed;

  SECTION_UNIT_ST unit;
  QJsonArray unitSection;
  /* 遍历 section 数组，获取字段信息 */
  for(auto it : sectionArray)
  {
    QJsonArray unitSection = it.toArray();
    unit.fieldName = unitSection[IDX_FIELD_NAME].toString();
    unit.fieldDescription = unitSection[IDX_FIELD_DESCRIPTION].toString();
    unit.fieldLength = 0;
    unit.filedData = "";

    /* if FIELD_LENGTH is string, means it's length is up to the other field
     * and the string show the field name it depend on(parent name).
     */
    if(unitSection[IDX_FIELD_LENGTH].isString())
    {
      QString parentName = unitSection[IDX_FIELD_LENGTH].toString();
      if(!jsonObjParsed.contains(parentName))
      {
        qDebug() << "Error: cannot find parent field! Check the section unit order!";
        break;
      }

      QString parentData = QString::number(jsonObjParsed[parentName].toArray()[2].toDouble());

      /* get field length in relation section */
      if(relationObject.contains(unit.fieldName))
      {
        unit.fieldLength = relationObject[unit.fieldName].toObject()[parentData].toDouble();
      }
      else
      {
        qDebug() << "Error: search failed!";
        break;
      }
    }
    else if(unitSection[IDX_FIELD_LENGTH].isDouble())
    {
      unit.fieldLength = unitSection[IDX_FIELD_LENGTH].toDouble();
    }
    else
    {
      qDebug() << "Error: element type not supported!";
      break;
    }

    unit.filedData = str.left(static_cast<int>(unit.fieldLength));

    /* add to jsonObjParsed and jsonArrayParsed */
    QJsonArray Array;
    Array.append(unit.fieldDescription);
    Array.append(unit.fieldLength);
    Array.append(unit.filedData);
    jsonObjParsed[unit.fieldName] = Array;

    Array.push_front(unit.fieldName);
    jsonArrayParsed.append(Array);

    str = str.right(str.size() - static_cast<int>(unit.fieldLength));
  }

  QJsonValue jsonValueRet;

  for(auto it : jsonArrayParsed)
  {
    qDebug() << it;
  }

  return jsonValueRet;
}

QList<QStringList> ProtocolPaserDefault::Parse2List(QString str, QJsonValue* jsonValue)
{
  (void)str;
  (void)jsonValue;

  qDebug() << "Default Parser";

  QList<QStringList> strArray;
  return strArray;
}

QJsonValue ProtocolPaserDefault::Parse2Json(QString str, QJsonValue *jsonValue)
{
  (void)str;
  (void)jsonValue;

  qDebug() << "Default Parser";

  QJsonValue jsonValueRet;
  return jsonValueRet;
}

/* factory interface class */
ProtocolParseInterface::ProtocolParseInterface(QString str, QJsonValue* jsonValue)
{
  QJsonObject jsonObj = jsonValue->toObject();

  this->str = str;
  this->jsonValue = jsonObj;

  parser = new ProtocolPaser_CunKou();
//  parser = new ProtocolPaseDefault();

  qDebug() << "ProtocolParseInterface Constructor";
}

ProtocolParseInterface::~ProtocolParseInterface()
{
  qDebug() << "ProtocolParseInterface Destructor";
  if(parser)
  {
    delete parser;
  }
}

QList<QStringList> ProtocolParseInterface::Parse2List()
{
  QString strHeader = str.left(4);
  QJsonValue jsonValueUnit= jsonValue.toObject()[strHeader];

  return parser->Parse2List(str, &jsonValueUnit);
}

QJsonValue ProtocolParseInterface::Parse2Json()
{
  QString strHeader = str.left(4);
  QJsonValue jsonValueUnit= jsonValue.toObject()[strHeader];

  return parser->Parse2Json(str, &jsonValueUnit);
}
