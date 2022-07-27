#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_procReader(new ProcessReaderLnx(this)),
    m_processModel(new ProcessModel(this)),
    m_threadModel(new ThreadsModel(this)),
    m_updateTimer(new QTimer(this))
{
    ui->setupUi(this);

    connect(m_threadModel, &ThreadsModel::showThreadStatus, this, &MainWindow::slot_showThreadInfo);

    ui->processTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->processTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->processTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->processTableView->setModel(m_processModel);

    connect(ui->processTableView, &QTableView::clicked, this, &MainWindow::slot_processClicked);

    ui->threadsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->threadsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->threadsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->threadsTableView->setModel(m_threadModel);

    connect(ui->threadsTableView, &QTableView::clicked, this, &MainWindow::slot_threadClicked);

    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::slot_updateProcData);
    m_updateTimer->start(200);
}
// ---------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
// ---------------------------------------------------------------------------------------
void MainWindow::slot_processClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }

    qint32 pid = qvariant_cast<ProcDesc>(m_processModel->data(index, ProcessModel::DataRole)).pid;
    m_threadModel->setPid(pid);
}
// ---------------------------------------------------------------------------------------
void MainWindow::slot_threadClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }

    qint32 tid = qvariant_cast<ThreadStat>(m_threadModel->data(index, ThreadsModel::DataRole)).tid;
    m_threadModel->setTid(tid);
}
// ---------------------------------------------------------------------------------------
void MainWindow::slot_showThreadInfo(const ThreadStat &stat)
{
    qint32  tid     = stat.tid;
    quint64 cTime   = stat.cpuTime;

    QString ttlStr;
    if(stat.memTtl < 1024)
    {
        ttlStr = QString::number(stat.memTtl) + "B";
    }
    if(stat.memTtl >= 1024 && stat.memTtl < (1024 * 1024))
    {
        ttlStr = QString::number(stat.memTtl / 1024) + "K";
    }
    if(stat.memTtl >= (1024 * 1024) && stat.memTtl < (1024 * 1024 * 1024))
    {
        ttlStr = QString::number(stat.memTtl / 1024 / 1024) + "M";
    }
    if(stat.memTtl >= (1024 * 1024 * 1024))
    {
        ttlStr = QString::number(stat.memTtl / 1024 / 1024 / 1024) + "G";
    }

    QString rssStr;
    if(stat.memRss < 1024)
    {
        rssStr = QString::number(stat.memRss) + "B";
    }
    if(stat.memRss >= 1024 && stat.memRss < (1024 * 1024))
    {
        rssStr = QString::number(stat.memRss / 1024) + "K";
    }
    if(stat.memRss >= (1024 * 1024) && stat.memRss < (1024 * 1024 * 1024))
    {
        rssStr = QString::number(stat.memRss / 1024 / 1024) + "M";
    }
    if(stat.memRss >= (1024 * 1024 * 1024))
    {
        rssStr = QString::number(stat.memRss / 1024 / 1024 / 1024) + "G";
    }

    QString shrStr;
    if(stat.memShr < 1024)
    {
        shrStr = QString::number(stat.memShr) + "B";
    }
    if(stat.memShr >= 1024 && stat.memShr < (1024 * 1024))
    {
        shrStr = QString::number(stat.memShr / 1024) + "K";
    }
    if(stat.memShr >= (1024 * 1024) && stat.memShr < (1024 * 1024 * 1024))
    {
        shrStr = QString::number(stat.memShr / 1024 / 1024) + "M";
    }
    if(stat.memShr >= (1024 * 1024 * 1024))
    {
        shrStr = QString::number(stat.memShr / 1024 / 1024 / 1024) + "G";
    }

    QString message = "TID: %0, Memory (TOTAL): %1, (RSS): %2, (SHR): %3 Thread Time: %4";
    ui->statusbar->showMessage(message.arg(tid)
                                      .arg(ttlStr)
                                      .arg(rssStr)
                                      .arg(shrStr)
                                      .arg(QTime(0, 0, cTime).toString("hh:mm:ss")));
}
// ---------------------------------------------------------------------------------------
void MainWindow::slot_updateProcData()
{
    QList<ProcDesc> procList = m_procReader->readProcesses();
    m_processModel->updateProcessData(procList);
    m_threadModel->updateProcessData(procList);
}

