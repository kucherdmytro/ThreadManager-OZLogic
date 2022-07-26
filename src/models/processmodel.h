#ifndef PROCESSMODEL_H
#define PROCESSMODEL_H

#include <QSortFilterProxyModel>
#include <QAbstractTableModel>
#include "procdesc.h"


class ProcessModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit        ProcessModel                           (QObject *parent = nullptr);

    Qt::ItemFlags   flags                                   (const QModelIndex &index) const override;
    int             rowCount               (const QModelIndex &parent = QModelIndex()) const override;
    int             columnCount            (const QModelIndex &parent = QModelIndex()) const override;
    QVariant        data        (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant        headerData    (int section, Qt::Orientation orientation, int role) const override;
    void            sort        (int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void            updateProcessData               (const QList<ProcDesc> &procList);

    enum Roles    { DataRole = Qt::UserRole + 1 };

private:
    QList<ProcDesc> m_data;
};

#endif // PROCESSMODEL_H
