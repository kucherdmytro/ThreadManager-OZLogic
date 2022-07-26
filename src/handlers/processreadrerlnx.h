#ifndef PROCESSREADRERLNX_H
#define PROCESSREADRERLNX_H

#include <pwd.h>
#include <unistd.h>

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include "processreader.h"

// /proc/[pid]/statm file
struct StatmData
{
    quint64 size            {0};    /*1.   total program size*/
    quint64 resident        {0};    /*2.   resident set size*/
    quint64 shared          {0};    /*3.   number of resident shared pages (i.e., backed by a file)*/
    quint64 text            {0};    /*4.   text (code)*/
    quint64 lib             {0};    /*5.   library (unused since Linux 2.6; always 0)*/
    quint64 data            {0};    /*6.   data + stack*/
    quint64 dt              {0};    /*7.   dirty pages (unused since Linux 2.6; always 0)*/
};
// /proc/[pid]/stat file
struct StatData
{
    qint32  pid             {0};    /*1.    process id*/
    QString tcomm          {""};    /*2.    filename of the executable*/
    QChar   state           {0};    /*3.    state (R is running, S is sleeping, D is sleeping in an uninterruptible wait, Z is zombie, T is traced or stopped)*/
    qint32  ppid            {0};    /*4.    process id of the parent process*/
    qint32  pgrp            {0};    /*5.    pgrp of the process*/
    qint32  sid             {0};    /*6.    session id*/
    qint32  tty_nr          {0};    /*7.    tty the process uses*/
    qint32  tty_pgrp        {0};    /*8.    pgrp of the tty*/
    quint32 flags           {0};    /*9.    task flags*/
    quint64 min_flt         {0};    /*10.   number of minor faults*/
    quint64 cmin_flt        {0};    /*11.   number of minor faults with child’s*/
    quint64 maj_flt         {0};    /*12.   number of major faults*/
    quint64 cmaj_flt        {0};    /*13.   number of major faults with child’s*/
    quint64 utime           {0};    /*14.   user mode jiffies*/
    quint64 stime           {0};    /*15.   kernel mode jiffies*/
    qint64  cutime          {0};    /*16.   user mode jiffies with child’s*/
    qint64  cstime          {0};    /*17.   kernel mode jiffies with child’s*/
    qint64  priority        {0};    /*18.   priority level*/
    qint64  nice            {0};    /*19.   nice level*/
    qint64  num_threads     {0};    /*20.   number of threads*/
    qint64  it_real_value   {0};    /*21.   (obsolete, always 0)*/
    quint64 start_time      {0};    /*22.   time the process started after system boot*/
    quint64 vsize           {0};    /*23.   virtual memory size*/
    qint64  rss             {0};    /*24.   resident set memory size*/
    quint64 rsslim          {0};    /*25.   current limit in bytes on the rss*/
    quint64 start_code      {0};    /*26.   address above which program text can run*/
    quint64 end_code        {0};    /*27.   address below which program text can run*/
    quint64 start_stack     {0};    /*28.   address of the start of the main process stack*/
    quint64 esp             {0};    /*29.   current value of ESP*/
    quint64 eip             {0};    /*30.   current value of EIP*/
    quint64 pending         {0};    /*31.   bitmap of pending signals*/
    quint64 blocked         {0};    /*32.   bitmap of blocked signals*/
    quint64 sigign          {0};    /*33.   bitmap of ignored signals*/
    quint64 sigcatch        {0};    /*34.   bitmap of caught signals*/
    quint64 wchan           {0};    /*35.   (place holder, used to be the wchan address, use /proc/PID/wchan instead)*/
    quint64 nswap           {0};    /*36.   (place holder)*/
    quint64 cnswap          {0};    /*37.   (place holder)*/
    quint64 exit_signal     {0};    /*38.   signal to send to parent thread on exit*/
    qint32  task_cpu        {0};    /*39.   which CPU the task is scheduled on*/
    quint32 rt_priority     {0};    /*40.   realtime priority*/
    quint32 policy          {0};    /*41.   scheduling policy (man sched_setscheduler)*/
    quint64 blkio_ticks     {0};    /*42.   time spent waiting for block IO*/
    quint64 gtime           {0};    /*43.   guest time of the task in jiffies*/
    qint64  cgtime          {0};    /*44.   guest time of the task children in jiffies*/
    quint64 start_data      {0};    /*45.   address above which program data+bss is placed*/
    quint64 end_data        {0};    /*46.   address below which program data+bss is placed*/
    quint64 start_brk       {0};    /*47.   address above which program heap can be expanded with brk()*/
    quint64 arg_start       {0};    /*48.   address above which program command line is placed*/
    quint64 arg_end         {0};    /*49.   address below which program command line is placed*/
    quint64 env_start       {0};    /*50.   address above which program environment is placed*/
    quint64 env_end         {0};    /*51.   address below which program environment is placed*/
    qint32  exit_code       {0};    /*52.   the thread’s exit_code in the form reported by the waitpid system call*/
};

class ProcessReaderLnx : public ProcessReader
{
public:
    explicit         ProcessReaderLnx    (QObject *parent = nullptr);

public slots:
    QList<ProcDesc>  readProcesses                                () override;

private:
    StatData         readStatData          (const QString &filePath);
    StatmData        readStatmData         (const QString &filePath);
};

#endif // PROCESSREADRERLNX_H
