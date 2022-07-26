#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "processreadrerlnx.h"
#include "processmodel.h"
#include "threadsmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
                     MainWindow        (QWidget *parent = nullptr);
                    ~MainWindow                                 ();

private slots:
    void             slot_updateProcData                        ();
    void             slot_processClicked(const QModelIndex &index);
    void             slot_threadClicked (const QModelIndex &index);
    void             slot_showThreadInfo  (const ThreadStat &stat);

private:
    Ui::MainWindow  *ui;
    ProcessReader   *m_procReader                        = nullptr;
    ProcessModel    *m_processModel                      = nullptr;
    ThreadsModel    *m_threadModel                       = nullptr;
    QTimer          *m_updateTimer                       = nullptr;
};
#endif // MAINWINDOW_H
