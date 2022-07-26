#ifndef PROCESSREADER_H
#define PROCESSREADER_H

#include <QObject>
#include <QList>
#include <QTimer>

#include "procdesc.h"

class ProcessReader : public QObject
{
    Q_OBJECT
public:
    explicit                ProcessReader (QObject *parent = nullptr);

    virtual QList<ProcDesc> readProcesses                          () = 0;
};

#endif // PROCESSREADER_H
