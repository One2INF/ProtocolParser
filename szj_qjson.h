#ifndef SZJ_QJSON_H
#define SZJ_QJSON_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QTreeView>
#include <QStandardItem>

class SZJ_QJson
{
private:
  QStandardItemModel* cm_model;
  void createJsonItem(const QJsonValue& jsonValue, QStandardItem* stdItem);
public:
  SZJ_QJson();
  ~SZJ_QJson();
  static QJsonValue openJsonFile(QString strJsonFilePath);
  static void travelJson(const QJsonValue& JsonValue);
  void showJsonByTreeview(const QJsonValue& jsonValue, QTreeView* treeView);
};

#endif // SZJ_QJSON_H
