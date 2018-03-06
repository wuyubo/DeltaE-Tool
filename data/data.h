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
    bool saveCompGma(BYTE *pCompressGma, int size);
    bool saveColorMatrix(BYTE *sRgbCM, int size);
    bool readCompGma(BYTE *pCompressGma, int size);
    bool readColorMatrix(BYTE *psRgbCM, int size);
    BYTE *getCompGma();
    BYTE *getColorMatrix();
    void getPanelNativeData(double * panelNativeData);
    bool setPanelNativeData(int rgbw, int index, double dX, double dY,
                            double dZ, double d_y, double d_x);
    bool savePanelNativeData();
signals:

public slots:
public:
    cRGB_t pat_RgbList[_MAX_PATTERN_COUT];
    BYTE m_CompressGma[_MAX_COMP_GMA_COUT];
    BYTE m_ColorMatrix[_MAX_COLOR_MATRIX_COUT];
    double m_Native_RGBW[_MAX_NATIVE_COUT];
    int pat_RgbCount;
};

#endif // DATA_H
