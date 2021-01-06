#ifndef PROTOCOLPASE_H
#define PROTOCOLPASE_H

#include <QString>
#include <QDebug>
#include <QJsonValue>

class ProtocolParser
{
public:
  explicit ProtocolParser();
  virtual ~ProtocolParser();
  virtual QList<QStringList> Parse2List(QString str, QJsonValue* jsonValue);
  virtual QJsonValue Parse2Json(QString str, QJsonValue* jsonValue);
};

class ProtocolPaser_CunKou : public ProtocolParser
{
public:
  explicit ProtocolPaser_CunKou(void);
  ~ProtocolPaser_CunKou() override;

  QList<QStringList> Parse2List(QString str, QJsonValue* jsonValue) override;
  QJsonValue Parse2Json(QString str, QJsonValue* jsonValue) override;
};

class ProtocolPaserDefault : public ProtocolParser
{
public:
  QList<QStringList> Parse2List(QString str, QJsonValue* jsonValue) override;
  QJsonValue Parse2Json(QString str, QJsonValue* jsonValue) override;
};

class ProtocolParseInterface
{
private:
  QString str;
  QJsonValue jsonValue;

public:
  explicit ProtocolParseInterface(QString str, QJsonValue* jsonValue);
  ~ProtocolParseInterface();
  ProtocolParser *parser;
  QList<QStringList> Parse2List(void);
  QJsonValue Parse2Json(void);
};

#endif // PROTOCOLPASE_H
