#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#pragma once
#include "DeltaEInterface.h"
#include <QObject>
#include <QThread>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = 0);
    ~WorkerThread();
    void setInterface(DeltaEInterface *pInterface);
    void setStatus(FUNCSTATUS_t status);
protected:
    void run();
signals:
    void signalSendFeeback(FUNCSTATUS_t status, bool result);
private:
    DeltaEInterface *m_pDteInterface;
    FUNCSTATUS_t m_status;
};

#endif // WORKERTHREAD_H
