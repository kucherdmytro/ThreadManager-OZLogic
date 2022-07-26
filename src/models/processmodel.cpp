#include "processmodel.h"

ProcessModel::ProcessModel(QObject *parent) :QAbstractTableModel(parent)
{
}
// ---------------------------------------------------------------------------------------
Qt::ItemFlags ProcessModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
// ---------------------------------------------------------------------------------------
void ProcessModel::updateProcessData(const QList<ProcDesc> &procList)
{
    // add or update proc data
    foreach(const ProcDesc &proc, procList)
    {
        if(!m_data.contains(proc))
        {
            // add new proc data
            m_data.append(proc);
            beginInsertRows(QModelIndex(), m_data.indexOf(proc), m_data.indexOf(proc));
            insertRow(0);
            endInsertRows();
            emit dataChanged(createIndex(m_data.indexOf(proc), 0), createIndex(m_data.indexOf(proc), 8));
        }
        else
        {
            // update proc data
            m_data[m_data.indexOf(proc)] = proc;
            emit dataChanged(createIndex(m_data.indexOf(proc), 0), createIndex(m_data.indexOf(proc), 8));
        }
    }
    // remove old proc data from model
    foreach(const ProcDesc &proc, m_data)
    {
        if(!procList.contains(proc))
        {
            beginRemoveRows(QModelIndex(), m_data.indexOf(proc), m_data.indexOf(proc));
            removeRow(m_data.indexOf(proc));
            endRemoveRows();

            m_data.removeOne(proc);
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
}
// ---------------------------------------------------------------------------------------
int ProcessModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}
// ---------------------------------------------------------------------------------------
int ProcessModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 9;
}
// ---------------------------------------------------------------------------------------
QVariant ProcessModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        const ProcDesc &pd = m_data.at(index.row());
        switch(index.column())
        {
        case 0:
            return QVariant::fromValue(pd.ppid);
            break;
        case 1:
            return QVariant::fromValue(pd.pid);
            break;
        case 2:
            return QVariant::fromValue(pd.pName);
            break;
        case 3:
            return QVariant::fromValue(pd.threadNum);
            break;
        case 4:
            return QVariant::fromValue(pd.priority);
            break;
        case 5:
            return QVariant::fromValue(pd.nice);
            break;
        case 6:
            if(pd.memTtl < 1024)
            {
                return QVariant::fromValue(QString::number(pd.memTtl) + "B");
            }
            if(pd.memTtl >= 1024 && pd.memTtl < (1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memTtl / 1024) + "K");
            }
            if(pd.memTtl >= (1024 * 1024) && pd.memTtl < (1024 * 1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memTtl / 1024 / 1024) + "M");
            }
            if(pd.memTtl >= (1024 * 1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memTtl / 1024 / 1024 / 1024) + "G");
            }
            break;
        case 7:
            if(pd.memRss < 1024)
            {
                return QVariant::fromValue(QString::number(pd.memRss) + "B");
            }
            if(pd.memRss >= 1024 && pd.memRss < (1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memRss / 1024) + "K");
            }
            if(pd.memRss >= (1024 * 1024) && pd.memRss < (1024 * 1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memRss / 1024 / 1024) + "M");
            }
            if(pd.memRss >= (1024 * 1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memRss / 1024 / 1024 / 1024) + "G");
            }
            break;
        case 8:
            if(pd.memShr < 1024)
            {
                return QVariant::fromValue(QString::number(pd.memShr) + "B");
            }
            if(pd.memShr >= 1024 && pd.memShr < (1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memShr / 1024) + "K");
            }
            if(pd.memShr >= (1024 * 1024) && pd.memShr < (1024 * 1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memShr / 1024 / 1024) + "M");
            }
            if(pd.memShr >= (1024 * 1024 * 1024))
            {
                return QVariant::fromValue(QString::number(pd.memShr / 1024 / 1024 / 1024) + "G");
            }
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
        case 8:
            return Qt::AlignCenter;
            break;
        default:
            return Qt::AlignCenter;
            break;
        }
    }
    if(role == DataRole)
    {
        return QVariant::fromValue(m_data.at(index.row()));
    }
    return QVariant();
}
// ---------------------------------------------------------------------------------------
QVariant ProcessModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return QVariant::fromValue(QString("PPID"));
            break;
        case 1:
            return QVariant::fromValue(QString("PID"));
            break;
        case 2:
            return QVariant::fromValue(QString("Process name"));
            break;
        case 3:
            return QVariant::fromValue(QString("Thread count"));
            break;
        case 4:
            return QVariant::fromValue(QString("Priority"));
            break;
        case 5:
            return QVariant::fromValue(QString("Nice"));
            break;
        case 6:
            return QVariant::fromValue(QString("Memory (TOTAL)"));
            break;
        case 7:
            return QVariant::fromValue(QString("Memory (RSS)"));
            break;
        case 8:
            return QVariant::fromValue(QString("Memory (SHR)"));
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}
// ---------------------------------------------------------------------------------------
void ProcessModel::sort(int column, Qt::SortOrder order)
{
    if(order == Qt::AscendingOrder)
    {
        if(column == 0)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.ppid < b.ppid; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 1)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.pid < b.pid; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 2)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.pName.toLower() < b.pName.toLower(); });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 3)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.threadNum < b.threadNum; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 4)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.priority < b.priority; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 5)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.nice < b.nice; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 6)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.memTtl < b.memTtl; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 7)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.memRss < b.memRss; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 8)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.memShr < b.memShr; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
    else
    {
        if(column == 0)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.ppid > b.ppid; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 1)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.pid > b.pid; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 2)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.pName.toLower() > b.pName.toLower(); });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 3)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.threadNum > b.threadNum; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 4)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.priority > b.priority; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 5)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.nice > b.nice; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 6)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.memTtl > b.memTtl; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 7)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.memRss > b.memRss; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
        if(column == 8)
        {
            std::sort(m_data.begin(), m_data.end(),
                      [](const ProcDesc &a, const ProcDesc &b){ return a.memShr > b.memShr; });
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
}
