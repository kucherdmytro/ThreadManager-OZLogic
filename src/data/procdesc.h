#ifndef PROCDESC_H
#define PROCDESC_H

#include <QObject>
#include <QList>

struct ThreadStat
{
    qint32              tid         {0};
    QString             tName      {""};
    qint64              priority    {0};
    qint32              nice        {0};
    QChar               state       {0};
    quint64             cpuTime     {0};
    quint64             userTime    {0};
    quint64             memTtl      {0}; // bytes
    quint64             memRss      {0}; // bytes
    quint64             memShr      {0}; // bytes

    bool operator==(const ThreadStat &other) const
    {
        return tid == other.tid;
    }

    void operator=(const ThreadStat &other)
    {
        tid      = other.tid;
        tName    = other.tName;
        priority = other.priority;
        nice     = other.nice;
        state    = other.state;
        cpuTime  = other.cpuTime;
        userTime = other.userTime;
        memRss   = other.memRss;
    }
};

struct ProcDesc
{
    qint32            pid         {0};
    qint32            ppid        {0};
    QString           pName      {""};
    qint64            threadNum   {0};
    qint64            priority    {0};
    qint32            nice        {0};
    quint64           memTtl      {0}; // bytes
    quint64           memRss      {0}; // bytes
    quint64           memShr      {0}; // bytes
    QList<ThreadStat> threadStats;

    bool              valid = false;

    bool operator==(const ProcDesc &other) const
    {
        return pid == other.pid;
    }

    void operator=(const ProcDesc &other)
    {
        pid         = other.pid;
        ppid        = other.ppid;
        pName       = other.pName;
        threadNum   = other.threadNum;
        priority    = other.priority;
        nice        = other.nice;
        memTtl      = other.memTtl;
        memRss      = other.memRss;
        memShr      = other.memShr;
        threadStats = other.threadStats;

        valid       = other.valid;
    }
};

Q_DECLARE_METATYPE(ProcDesc)
Q_DECLARE_METATYPE(ThreadStat)
Q_DECLARE_METATYPE(QList<ThreadStat>)

#endif // PROCDESC_H
