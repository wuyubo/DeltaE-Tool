#include "workerthread.h"

WorkerThread::WorkerThread(QObject *parent):
    QThread(parent)
{
    setInterface(NULL);
    setStatus(FUNC_NONE);
}

WorkerThread::~WorkerThread()
{
    this->quit();
    this->wait();
}

void WorkerThread::run()
{
    bool isRun = true;
    bool result = false;

    while(isRun)
    {
        if(m_pDteInterface)
        {
            switch (m_status) {
            case FUNC_CHECK:
                result = m_pDteInterface->dteCheck();
                break;
            case FUNC_ADJUST:
                result = m_pDteInterface->dteAdjust();
                break;
            case FUNC_RUN:
                result = m_pDteInterface->dteRun();
                break;
            default:
                break;
            }
            emit signalSendFeeback(m_status, result);
            isRun = false;
        }
    }
}

void WorkerThread::setInterface(DeltaEInterface *pInterface)
{
    m_pDteInterface = pInterface;
}

void WorkerThread::setStatus(FUNCSTATUS_t status)
{
    m_status = status;
}
