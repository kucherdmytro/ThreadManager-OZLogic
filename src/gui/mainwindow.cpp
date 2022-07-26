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
    quint64 memRss = stat.memRss;
    QString rssStr;

    if(memRss < 1024)
    {
        rssStr = QString::number(memRss) + "B";
    }
    if(memRss >= 1024 && memRss < (1024 * 1024))
    {
        rssStr = QString::number(memRss / 1024) + "K";
    }
    if(memRss >= (1024 * 1024) && memRss < (1024 * 1024 * 1024))
    {
        rssStr = QString::number(memRss / 1024 / 1024) + "M";
    }
    if(memRss >= (1024 * 1024 * 1024))
    {
        rssStr = QString::number(memRss / 1024 / 1024 / 1024) + "G";
    }

    QString message = "TID: %0, Memory (RSS): %1, Thread Time: %2";

    ui->statusbar->showMessage(message.arg(tid).arg(rssStr).arg(QTime(0, 0, cTime).toString("hh:mm:ss")));
}
// ---------------------------------------------------------------------------------------
void MainWindow::slot_updateProcData()
{
    QList<ProcDesc> procList = m_procReader->readProcesses();
    m_processModel->updateProcessData(procList);
    m_threadModel->updateProcessData(procList);
}

