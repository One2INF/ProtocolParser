#include "protocolpase.h"

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
  qDebug() << "Parse AA19: " << strProtocol;

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


