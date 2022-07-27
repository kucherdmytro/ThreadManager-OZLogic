#include "processreadrerlnx.h"

ProcessReaderLnx::ProcessReaderLnx(QObject *parent) : ProcessReader(parent)
{
}
// ---------------------------------------------------------------------------------------
QList<ProcDesc> ProcessReaderLnx::readProcesses()
{
    QDir              procDir("/proc");
    StatData          procStat;
    QList<ProcDesc>   procList;

    if(procDir.exists())
    {
        QStringList pids = procDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        foreach(QString pidStr, pids)
        {
            bool ok;
            qint32 pid = QString(pidStr).toInt(&ok);
            if(ok)
            {
                ProcDesc  procDesc;
                StatData  procData  = readStatData ("/proc/" + QString::number(pid) + "/stat" );
                StatmData statmData = readStatmData("/proc/" + QString::number(pid) + "/statm");

                procDesc.pid        = procData.pid;
                procDesc.ppid       = procData.ppid;
                procDesc.pName      = procData.tcomm;
                procDesc.threadNum  = procData.num_threads;
                procDesc.priority   = procData.priority;
                procDesc.nice       = procData.nice;

                procDesc.memTtl     = statmData.size     * sysconf(_SC_PAGESIZE);
                procDesc.memRss     = statmData.resident * sysconf(_SC_PAGESIZE);
                procDesc.memShr     = statmData.shared   * sysconf(_SC_PAGESIZE);

                QDir threadDir("/proc/" + QString::number(pid) + "/task");
                if(threadDir.exists())
                {
                    QStringList tids = threadDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
                    foreach(QString tidStr, tids)
                    {
                        bool ok;
                        qint32 tid = QString(tidStr).toInt(&ok);
                        if(ok)
                        {
                            ThreadStat threadStat;
                            StatData   threadData = readStatData ("/proc/" + QString::number(pid) + "/task/" + QString::number(tid) + "/stat");
                            StatmData  statmData  = readStatmData("/proc/" + QString::number(pid) + "/task/" + QString::number(tid) + "/statm");

                            threadStat.tid        = threadData.pid;
                            threadStat.tName      = threadData.tcomm;
                            threadStat.priority   = threadData.priority;
                            threadStat.nice       = threadData.nice;
                            threadStat.state      = threadData.state;
                            threadStat.cpuTime    = (threadData.utime + threadData.stime) / sysconf(_SC_CLK_TCK);
                            threadStat.userTime   = threadData.utime / sysconf(_SC_CLK_TCK);

                            threadStat.memTtl     = statmData.size     * sysconf(_SC_PAGESIZE);
                            threadStat.memRss     = statmData.resident * sysconf(_SC_PAGESIZE);
                            threadStat.memShr     = statmData.shared   * sysconf(_SC_PAGESIZE);

                            procDesc.threadStats.append(threadStat);
                        }
                    }
                    procDesc.valid = true;
                    procList.append(procDesc);
                }
            }
        }
    }
    return procList;
}
// ---------------------------------------------------------------------------------------
StatData ProcessReaderLnx::readStatData(const QString &filePath)
{
    QFile statFile(filePath);
    if(!statFile.open(QIODevice::ReadOnly))
    {
        return StatData();
    }

    StatData statData;
    QString  line = statFile.readLine().trimmed();

    // read proc name
    statData.tcomm = line.mid(line.indexOf(" ("), line.indexOf(") ") - line.indexOf(" (") + 1).trimmed();
    statData.tcomm.remove('(').remove(')');
    // remove proc name val from the line
    line.remove(line.indexOf(" ("), line.indexOf(") ") - line.indexOf(" (") + 1);

    QStringList values = line.replace("  ", " ").split(" ");

    statData.pid           = values.at(0).toInt();          // %d
    statData.state         = values.at(1).at(0);            // %c
    statData.ppid          = values.at(2).toInt();          // %d
    statData.pgrp          = values.at(3).toInt();          // %d
    statData.sid           = values.at(4).toInt();          // %d
    statData.tty_nr        = values.at(5).toInt();          // %d
    statData.tty_pgrp      = values.at(6).toInt();          // %d
    statData.flags         = values.at(7).toUInt();         // %u
    statData.min_flt       = values.at(8).toULong();        // %lu
    statData.cmin_flt      = values.at(9).toULong();       // %lu
    statData.maj_flt       = values.at(10).toULong();       // %lu
    statData.cmaj_flt      = values.at(11).toULong();       // %lu
    statData.utime         = values.at(12).toULong();       // %lu
    statData.stime         = values.at(13).toULong();       // %lu
    statData.cutime        = values.at(14).toLong();        // %ld
    statData.cstime        = values.at(15).toLong();        // %ld
    statData.priority      = values.at(16).toLong();        // %ld
    statData.nice          = values.at(17).toLong();        // %ld
    statData.num_threads   = values.at(18).toLong();        // %ld
    statData.it_real_value = values.at(19).toLong();        // %ld
    statData.start_time    = values.at(20).toULongLong();   // %llu
    statData.vsize         = values.at(21).toULong();       // %lu
    statData.rss           = values.at(22).toLong();        // %ld
    statData.rsslim        = values.at(23).toULong();       // %lu
    statData.start_code    = values.at(24).toULong();       // %lu
    statData.end_code      = values.at(25).toULong();       // %lu
    statData.start_stack   = values.at(26).toULong();       // %lu
    statData.esp           = values.at(27).toULong();       // %lu
    statData.eip           = values.at(28).toULong();       // %lu
    statData.pending       = values.at(29).toULong();       // %lu
    statData.blocked       = values.at(30).toULong();       // %lu
    statData.sigign        = values.at(31).toULong();       // %lu
    statData.sigcatch      = values.at(32).toULong();       // %lu
    statData.wchan         = values.at(33).toULong();       // %lu
    statData.nswap         = values.at(34).toULong();       // %lu
    statData.cnswap        = values.at(35).toULong();       // %lu
    statData.exit_signal   = values.at(36).toLong();        // %ld
    statData.task_cpu      = values.at(37).toLong();        // %ld
    statData.rt_priority   = values.at(38).toUInt();        // %u
    statData.policy        = values.at(39).toUInt();        // %u
    statData.blkio_ticks   = values.at(40).toULongLong();   // %llu
    statData.gtime         = values.at(41).toULong();       // %lu
    statData.cgtime        = values.at(42).toLong();        // %ld
    statData.start_data    = values.at(43).toULong();       // %lu
    statData.end_data      = values.at(44).toULong();       // %lu
    statData.start_brk     = values.at(45).toULong();       // %lu
    statData.arg_start     = values.at(46).toULong();       // %lu
    statData.arg_end       = values.at(47).toULong();       // %lu
    statData.env_start     = values.at(48).toULong();       // %lu
    statData.env_end       = values.at(49).toULong();       // %lu
    statData.exit_code     = values.at(50).toInt();         // %d

    statFile.close();
    return statData;
}
// ---------------------------------------------------------------------------------------
StatmData ProcessReaderLnx::readStatmData(const QString &filePath)
{
    QFile    statmFile(filePath);
    if(statmFile.open(QIODevice::ReadOnly))
    {
        StatmData   statmData;
        QStringList values = QString(statmFile.readAll()).remove('\n').split(" ");

        statmData.size     = values.at(0).toUInt();
        statmData.resident = values.at(1).toUInt();
        statmData.shared   = values.at(2).toUInt();
        statmData.text     = values.at(3).toUInt();
        statmData.lib      = values.at(4).toUInt();
        statmData.data     = values.at(5).toUInt();
        statmData.dt       = values.at(6).toUInt();

        statmFile.close();
        return statmData;
    }
    return StatmData();
}
