#ifndef EXPORTWDGT2QRCODEGEN_H
#define EXPORTWDGT2QRCODEGEN_H

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "gui-src/mysortfilterproxymodel.h"
#include "qrcodegeneratortypes.h"

class ExportWdgt2qrcodegen
{
public:
    static QrTableMap fromRow(QStandardItemModel *model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen);

    static QrTableMap fromRow(QStandardItemModel *model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk);

    static QrTableMap fromRow(QStandardItemModel *model, const int &row, const QrTableColumnMap &tableMap);




    static QrTableMap fromRow(MySortFilterProxyModel *proxy_model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen);

    static QrTableMap fromRow(MySortFilterProxyModel *proxy_model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk);

    static QrTableMap fromRow(MySortFilterProxyModel *proxy_model, const int &row, const QrTableColumnMap &tableMap);



    static QrList fromModel(QStandardItemModel *model, const QList<int> &onlyRows, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen);

    static QrList fromModel(QStandardItemModel *model, const QList<int> &onlyRows, const QrTableColumnMap &tableMap, const QList<int> &lk);

    static QrList fromModel(QStandardItemModel *model, const QList<int> &onlyRows, const QrTableColumnMap &tableMap);


    static QrList fromProxyModel(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen);

    static QrList fromProxyModel(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows, const QrTableColumnMap &tableMap, const QList<int> &lk);

    static QrList fromProxyModel(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows, const QrTableColumnMap &tableMap);



    static QList<int> getRows(QItemSelectionModel *selmodel);

    static QList<int> getSourceRows(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows);

    static QList<int> getProxyRows(MySortFilterProxyModel *proxy_model);


};

#endif // EXPORTWDGT2QRCODEGEN_H
