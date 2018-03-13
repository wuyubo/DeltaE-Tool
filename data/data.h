#ifndef DATA_H
#define DATA_H
#include "global.h"
#include "ddc/burnsetting.h"
#include <QObject>
using namespace ddc;
class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);
    void init_PatRgbList();
    bool load_PatRgb();
    void createPattern(BYTE Level);
    int getPatternCount();
    void update_PatRgb( bool isCheck);
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
    bool loadPanelNativeData();
    bool saveDeltaEData();
    void setBurnSetting(QString name, QString value);
    bool loadBurnSetting();
    BurnSetting_T *defaultI2CSetting();
    BurnSetting_T *getBurnSetting();
signals:

public slots:
public:
    cRGB_t pat_RgbList[_MAX_PATTERN_COUT];
    BYTE m_CompressGma[_MAX_COMP_GMA_COUT];
    BYTE m_ColorMatrix[_MAX_COLOR_MATRIX_COUT];
    double m_Native_RGBW[_MAX_NATIVE_COUT];
    int pat_RgbCount;
    int pat_Level;
    BurnSetting_T *m_pBurnsettings;
};

#endif // DATA_H
