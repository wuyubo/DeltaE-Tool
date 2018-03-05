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
    QString readFile(QString path);
    bool saveFile(QString path, QString data, bool isCover = true);
    bool saveCompGma(QString path, BYTE *pCompressGma, int size);
    bool saveColorMatrix(QString path, BYTE *sRgbCM, int size);
    bool readCompGma(QString path, BYTE *pCompressGma, int size);
    bool readColorMatrix(QString path, BYTE *psRgbCM, int size);
    BYTE *getCompGma();
    BYTE *getColorMatrix();
signals:

public slots:
public:
    cRGB_t pat_RgbList[_MAX_PATTERN_COUT];
    BYTE m_CompressGma[_MAX_COMP_GMA_COUT];
    BYTE m_ColorMatrix[_MAX_COLOR_MATRIX_COUT];
    int pat_RgbCount;
};

#endif // DATA_H
