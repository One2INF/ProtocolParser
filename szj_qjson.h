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
  static void createJsonItem(const QJsonValue& jsonValue, QStandardItem* stdItem);
public:
  SZJ_QJson();
  static QJsonValue openJsonFile(QString strJsonFilePath);
  static void travelJson(const QJsonValue& JsonValue);
  static void showJsonByTreeview(const QJsonValue& jsonValue, QTreeView* treeView);
};

#endif // SZJ_QJSON_H
