#include "exportwdgt2qrcodegen.h"



QrTableMap ExportWdgt2qrcodegen::fromRow(QStandardItemModel *model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen)
{
    QrTableMap map;
    if(model && row >= 0){
        for(int i = 0; i < lkLen; i++){
            const int c = lk.at(i);
            if(c < 0)
                continue;
            map.insert(tableMap.value(c), model->item(row, c)->text()) ;
        }
    }
    return map;
}

QrTableMap ExportWdgt2qrcodegen::fromRow(QStandardItemModel *model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk)
{
    return fromRow(model, row, tableMap, lk, lk.size());
}

QrTableMap ExportWdgt2qrcodegen::fromRow(QStandardItemModel *model, const int &row, const QrTableColumnMap &tableMap)
{
    return fromRow(model, row, tableMap, tableMap.keys());
}



QrTableMap ExportWdgt2qrcodegen::fromRow(MySortFilterProxyModel *proxy_model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen)
{
    return (proxy_model) ? fromRow(MySortFilterProxyModel::getStandardModel(proxy_model), proxy_model->mapToSource(proxy_model->index(row, 0)).row(), tableMap, lk, lkLen) : QrTableMap();
}

QrTableMap ExportWdgt2qrcodegen::fromRow(MySortFilterProxyModel *proxy_model, const int &row, const QrTableColumnMap &tableMap, const QList<int> &lk)
{
    return fromRow(proxy_model, row, tableMap, lk, lk.length());
}

QrTableMap ExportWdgt2qrcodegen::fromRow(MySortFilterProxyModel *proxy_model, const int &row, const QrTableColumnMap &tableMap)
{
    return fromRow(proxy_model, row, tableMap, tableMap.keys());
}


QrList ExportWdgt2qrcodegen::fromModel(QStandardItemModel *model, const QList<int> &onlyRows, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen)
{
    QrList l;
    for(int i = 0, imax = onlyRows.size(); i < imax; i++){
        const QrTableMap m = fromRow(model, onlyRows.at(i), tableMap, lk, lkLen);
        if(m.isEmpty())
            continue;
        l.append(m);
    }
    return l;
}

QrList ExportWdgt2qrcodegen::fromModel(QStandardItemModel *model, const QList<int> &onlyRows, const QrTableColumnMap &tableMap, const QList<int> &lk)
{
    return fromModel(model, onlyRows, tableMap, lk, lk.size());
}

QrList ExportWdgt2qrcodegen::fromModel(QStandardItemModel *model, const QList<int> &onlyRows, const QrTableColumnMap &tableMap)
{
    return fromModel(model, onlyRows, tableMap, tableMap.keys());
}

QrList ExportWdgt2qrcodegen::fromProxyModel(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows, const QrTableColumnMap &tableMap, const QList<int> &lk, const int &lkLen)
{
    return (proxy_model) ? fromModel(MySortFilterProxyModel::getStandardModel(proxy_model), getSourceRows(proxy_model, onlyProxyRows), tableMap, lk, lkLen) : QrList();
}

QrList ExportWdgt2qrcodegen::fromProxyModel(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows, const QrTableColumnMap &tableMap, const QList<int> &lk)
{
    return fromProxyModel(proxy_model, onlyProxyRows, tableMap, lk, lk.size());
}

QrList ExportWdgt2qrcodegen::fromProxyModel(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows, const QrTableColumnMap &tableMap)
{
    return fromProxyModel(proxy_model, onlyProxyRows, tableMap, tableMap.keys());
}

QList<int> ExportWdgt2qrcodegen::getRows(QItemSelectionModel *selmodel)
{
    QList<int> l;
    const QModelIndexList list = selmodel->selectedRows();
    for(int i = 0, imax = list.size(); i < imax; i++){
        const int r = list.at(i).row();
        if(r < 0)
            continue;
        l.append(r);
    }
    return l;
}

QList<int> ExportWdgt2qrcodegen::getSourceRows(MySortFilterProxyModel *proxy_model, const QList<int> &onlyProxyRows)
{//
    QList<int> l;
    for(int i = 0, imax = onlyProxyRows.size(); i < imax; i++){
        const int r = proxy_model->mapToSource(proxy_model->index(onlyProxyRows.at(i), 0)).row();
        if(r < 0)
            continue;
        l.append(r);
    }
    return l;
}

QList<int> ExportWdgt2qrcodegen::getProxyRows(MySortFilterProxyModel *proxy_model)
{
    QList<int> l;
    for(int i = 0, imax = proxy_model->rowCount(); i < imax; i++)
        l.append(i);
    return l;
}
