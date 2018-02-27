#ifndef DATA_H
#define DATA_H
#include "global.h"
#include <QObject>

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);
    void init_PatRgbList();
    bool load_PatRgb();
    void update_PatRgb();
signals:

public slots:
public:
    cRGB_t pat_RgbList[_MAX_PATTERN_COUT];
    int pat_RgbCount;
};

#endif // DATA_H
