#ifndef PROTOCOLPASE_H
#define PROTOCOLPASE_H

#include <QString>
#include <QDebug>

class ProtocolParse
{
public:
  explicit ProtocolParse();
  virtual ~ProtocolParse();
  virtual QList<QStringList> Parse(QString);

};

class ProtocolPaseFFJAA19 : public ProtocolParse
{
public:
  explicit ProtocolPaseFFJAA19();
  ~ProtocolPaseFFJAA19();
  QList<QStringList> Parse(QString);
};

class ProtocolPaseFFJDefault : public ProtocolParse
{
public:
  QList<QStringList> Parse(QString);
};

class ProtocolParseInterface
{
public:
  explicit ProtocolParseInterface(QString strProtocol);
  ~ProtocolParseInterface();
  ProtocolParse *pp;
  QList<QStringList> Parse(QString str);
};

#endif // PROTOCOLPASE_H
