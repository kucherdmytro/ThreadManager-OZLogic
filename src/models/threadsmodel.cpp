#include "threadsmodel.h"

ThreadsModel::ThreadsModel(QObject *parent) : QAbstractTableModel{parent}
{
}
// ---------------------------------------------------------------------------------------
Qt::ItemFlags ThreadsModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
// ---------------------------------------------------------------------------------------
int ThreadsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_threads.size();
}
// ---------------------------------------------------------------------------------------
int ThreadsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
}
// ---------------------------------------------------------------------------------------
QVariant ThreadsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        const ThreadStat &ts { m_threads.at(index.row()) };
        switch(index.column())
        {
        case 0:
            return QVariant::fromValue(m_pid);
            break;
        case 1:
            return QVariant::fromValue(ts.tid);
            break;
        case 2:
            return QVariant::fromValue(ts.tName);
            break;
        case 3:
            return QVariant::fromValue(ts.priority);
            break;
        case 4:
            return QVariant::fromValue(ts.nice);
            break;
        case 5:
            if(ts.state == 'R')
            {
                return QVariant::fromValue(QString("Running"));
            }
            if(ts.state == 'S')
            {
                return QVariant::fromValue(QString("Sleeping"));
            }
            if(ts.state == 'D')
            {
                return QVariant::fromValue(QString("Waiting"));
            }
            if(ts.state == 'Z')
            {
                return QVariant::fromValue(QString("Zombie"));
            }
            if(ts.state == 'T')
            {
                return QVariant::fromValue(QString("Stopped"));
            }
            if(ts.state == 'X')
            {
                return QVariant::fromValue(QString("Dead"));
            }
            if(ts.state == 'I')
            {
                return QVariant::fromValue(QString("Idle"));
            }
            return QVariant::fromValue(ts.state);
            break;
        case 6:
            return QVariant::fromValue(QTime(0, 0, ts.cpuTime).toString("hh:mm:ss"));
            break;
        case 7:
            return QVariant::fromValue(QTime(0, 0, ts.userTime).toString("hh:mm:ss"));
            break;
        default:
            return QVariant();
            break;
        }
    }
    if(role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
        case 0:
            return Qt::AlignCenter;
            break;
        case 1:
            return Qt::AlignCenter;
            break;
        case 2:
            return Qt::AlignVCenter;
            break;
        case 3:
            return Qt::AlignCenter;
            break;
        case 4:
            return Qt::AlignCenter;
            break;
        case 5:
            return Qt::AlignCenter;
            break;
        case 6:
            return Qt::AlignCenter;
            break;
        case 7:
            return Qt::AlignCenter;
            break;
        default:
            return Qt::AlignCenter;
            break;
        }
    }
    if(role == DataRole)
    {
        return QVariant::fromValue(m_threads.at(index.row()));
    }
    return QVariant();
}
// ---------------------------------------------------------------------------------------
QVariant ThreadsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return QVariant::fromValue(QString("PID"));
            break;
        case 1:
            return QVariant::fromValue(QString("TID"));
            break;
        case 2:
            return QVariant::fromValue(QString("Thread name"));
            break;
        case 3:
            return QVariant::fromValue(QString("Priority"));
            break;
        case 4:
            return QVariant::fromValue(QString("Nice"));
            break;
        case 5:
            return QVariant::fromValue(QString("State"));
            break;
        case 6:
            return QVariant::fromValue(QString("CPU Time"));
            break;
        case 7:
            return QVariant::fromValue(QString("User Time"));
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}
// ---------------------------------------------------------------------------------------
void ThreadsModel::sort(int column, Qt::SortOrder order)
{
    if(order == Qt::AscendingOrder)
    {
        if(column == 1)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.tid < b.tid; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 2)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.tName.toLower() < b.tName.toLower(); });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 3)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.priority < b.priority; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 4)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.nice < b.nice; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 5)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.state < b.state; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 6)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.cpuTime < b.cpuTime; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 7)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.userTime < b.userTime; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
    else
    {
        if(column == 1)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.tid > b.tid; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 2)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.tName.toLower() > b.tName.toLower(); });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 3)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.priority > b.priority; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 4)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.nice > b.nice; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 5)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.state > b.state; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 6)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.cpuTime > b.cpuTime; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 7)
        {
            std::sort(m_threads.begin(), m_threads.end(),
                      [](const ThreadStat &a, const ThreadStat &b){ return a.userTime > b.userTime; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
}
// ---------------------------------------------------------------------------------------
void ThreadsModel::updateProcessData(const QList<ProcDesc> &procList)
{
    // add or update proc data
    foreach(const ProcDesc &proc, procList)
    {
        if(!m_processes.contains(proc))
        {
            // add new proc data
            m_processes.append(proc);
        }
        else
        {
            // update proc data
            m_processes[m_processes.indexOf(proc)] = proc;
        }
    }
    // remove old proc data from model
    foreach(const ProcDesc &proc, m_processes)
    {
        if(!procList.contains(proc))
        {
            m_processes.removeOne(proc);
        }
    }
    showThreadInfo();
}
// ---------------------------------------------------------------------------------------
void ThreadsModel::setPid(const qint32 &pid)
{
    m_pid = pid;
}
// ---------------------------------------------------------------------------------------
void ThreadsModel::setTid(const qint32 &tid)
{
    m_tid = tid;
}
// ---------------------------------------------------------------------------------------
void ThreadsModel::showThreadInfo()
{
    if(m_pid == 0 && !m_processes.isEmpty())
    {
        m_pid = m_processes.first().pid;
    }

    auto procIt = std::find_if(m_processes.begin(), m_processes.end(), [&](const ProcDesc &proc){ return proc.pid == m_pid; });
    if(procIt != m_processes.end())
    {
        const ProcDesc &proc = *procIt;
        // add or update stat data from selected proc
        foreach(const ThreadStat &stat, proc.threadStats)
        {
            if(!m_threads.contains(stat))
            {
                // add new stat data
                m_threads.append(stat);
                beginInsertRows(QModelIndex(), m_threads.indexOf(stat), m_threads.indexOf(stat));
                insertRow(0);
                endInsertRows();
                emit dataChanged(createIndex(m_threads.indexOf(stat), 0), createIndex(m_threads.indexOf(stat), 6));
            }
            else
            {
                // update proc data
                m_threads[m_threads.indexOf(stat)] = stat;
                emit dataChanged(createIndex(m_threads.indexOf(stat), 0), createIndex(m_threads.indexOf(stat), 6));
            }
        }
        // remove old stat data from model
        foreach(const ThreadStat &stat, m_threads)
        {
            if(!proc.threadStats.contains(stat))
            {
                beginRemoveRows(QModelIndex(), m_threads.indexOf(stat), m_threads.indexOf(stat));
                removeRow(m_threads.indexOf(stat));
                endRemoveRows();

                m_threads.removeOne(stat);
                emit dataChanged(QModelIndex(), QModelIndex());
            }
        }
    }
    if(m_tid == 0 && !m_threads.isEmpty())
    {
        m_tid = m_threads.first().tid;
    }
    auto statIt = std::find_if(m_threads.begin(), m_threads.end(), [&](const ThreadStat &stat){ return stat.tid == m_tid; });
    if(statIt != m_threads.end())
    {
        const ThreadStat &stat = *statIt;
        emit showThreadStatus(stat);
    }
}
