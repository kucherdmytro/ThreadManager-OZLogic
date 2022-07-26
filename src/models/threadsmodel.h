#ifndef THREADSMODEL_H
#define THREADSMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include "procdesc.h"

class ThreadsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit          ThreadsModel                           (QObject *parent = nullptr);

    Qt::ItemFlags     flags                                   (const QModelIndex &index) const override;
    int               rowCount               (const QModelIndex &parent = QModelIndex()) const override;
    int               columnCount            (const QModelIndex &parent = QModelIndex()) const override;
    QVariant          data        (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant          headerData    (int section, Qt::Orientation orientation, int role) const override;
    void              sort        (int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void              updateProcessData               (const QList<ProcDesc> &procList);
    void              setPid                                        (const qint32 &pid);
    void              setTid                                        (const qint32 &tid);

    enum Roles      { DataRole = Qt::UserRole + 1 };

signals:
    void              showThreadStatus                         (const ThreadStat &stat);

private:
    void              showThreadInfo                                                 ();

private:
    qint32            m_tid = 0;
    qint32            m_pid = 0;
    QList<ProcDesc>   m_processes;
    QList<ThreadStat> m_threads;
};

#endif // THREADSMODEL_H
