#include "data.h"
#include <QFile>
Data::Data(QObject *parent) : QObject(parent)
{
    init_PatRgbList();
}

void Data::init_PatRgbList()
{
    int i;
    cRGB_t defaultPatternList[32] = {
        {32, 32, 32},
        {64, 64, 64},
        {96, 96, 96},
        {128, 128, 128},
        {160, 160, 160},
        {192, 192, 192},
        {224, 224, 224},
        {255, 255, 255},
        {128, 0, 0},
        {192, 64, 64},
        {255, 0, 0},
        {255, 128, 128},
        {0, 128, 0},
        {64, 192, 64},
        {0, 255, 0},
        {0, 255, 0},
        {0, 0, 128},
        {64, 64, 192},
        {0, 0, 255},
        {128, 128, 255},
        {128, 128, 0},
        {192, 192, 64},
        {255, 255, 0},
        {255, 255, 128},
        {128, 0, 128},
        {192, 64, 192},
        {255, 0, 255},
        {255, 128, 255},
        {0, 128, 128},
        {64, 192, 192},
        {0, 255, 255},
        {128, 255, 255},
    };
    pat_RgbCount = 32;
    for(i = 0; i < 32; i++)
    {
        pat_RgbList[i] = defaultPatternList[i];
    }
}

bool Data::load_PatRgb()
{
    QString temp;
    QStringList ListRgb;
    cRGB_t rgb;
    int index = 0;
    char cBuf[128];
    qint64 LineLen;
    QFile file(SRGB_PATTERN_PATN);      //---打开文件
    if (file.open(QIODevice :: ReadOnly)) //  以只读的方式打开
    {
        do
        {
            LineLen = file.readLine(cBuf, sizeof(cBuf)); //---读取文本文件的一行
            if (-1 != LineLen)                          //---读取成功，将返回读取的字节，读取失败，将返回-1
            {
                temp = QString(QLatin1String(cBuf));
                if(index == 0)
                {
                    pat_RgbCount = temp.toInt();
                }
                else if(index > 1)
                {
                    ListRgb.clear();
                    ListRgb = temp.split(' ', QString::SkipEmptyParts);//temp.split(",");
                    if(ListRgb.length() < 4)
                    {
                        continue;
                    }
                    rgb.red = ListRgb[1].toInt();
                    rgb.green = ListRgb[2].toInt();
                    rgb.blue = ListRgb[3].toInt();
                    pat_RgbList[index-2] = rgb;
                }
                index++;
            }
        }while(-1 != LineLen);
        if(pat_RgbCount > index-2)
        {
            pat_RgbCount = index-2;
        }
    }
    else
    {
        goto loaderr;
    }
    //----关闭文件，这里请注意，打开文件后，在不对文件操作时，请关闭文件，避免数据丢失
    file.close();
    return true;
loaderr:
    file.close();
    return false;
}

void Data::update_PatRgb()
{
    if(load_PatRgb() == false)
    {
        init_PatRgbList();
    }
}
QString Data::readFile(QString path)
{
    QString data;
    char cBuf[128];
    qint64 LineLen;
    QFile file(path);      //---打开文件
    data.clear();
    if (file.open(QIODevice :: ReadOnly)) //  以只读的方式打开
    {
        do
        {
            LineLen = file.readLine(cBuf, sizeof(cBuf)); //---读取文本文件的一行
            if (-1 != LineLen)                          //---读取成功，将返回读取的字节，读取失败，将返回-1
            {
                data.append(QString(QLatin1String(cBuf)));
            }
        }while(-1 != LineLen);
    }
    file.close();
    return data;
}

bool Data::saveFile(QString path, QString data, bool isCover)
{
    QFile file(path);
    isCover = isCover;//not uses
    if (file.open(QIODevice::WriteOnly)) //打开方式：可写、二进制方式
    {
        //按二进制写入时，须进行类型转换，这也是QFile使用上比较麻烦的地方
        file.write(data.toStdString().c_str()); //参数为char*，须转换
        file.close();
        return true;
    }
    return false;
}
bool Data::saveCompGma(QString path, BYTE *pCompressGma, int size)
{
    //QString data;
    int i;
    if(size > _MAX_COMP_GMA_COUT)
    {
        return false;
    }
    for(i = 0; i < size; i++)
    {
        m_CompressGma[i] = pCompressGma[i];
    }

    return true;//saveFile(path, data);
}

bool Data::saveColorMatrix(QString path, BYTE *psRgbCM, int size)
{
    //QString data;
    int i;
    if(size > _MAX_COLOR_MATRIX_COUT)
    {
        return false;
    }
    for(i = 0; i < size; i++)
    {
        m_ColorMatrix[i] = psRgbCM[i];
    }
    return true;//saveFile(path, data);
}

bool Data::readCompGma(QString path, BYTE *pCompressGma, int size)
{
    //QString data = readFile(path);
    return false;
}

bool Data::readColorMatrix(QString path, BYTE *sRgbCM, int size)
{
    //QString data = readFile(path);
    return false;
}

BYTE *Data::getCompGma()
{
    return m_CompressGma;
}

BYTE *Data::getColorMatrix()
{
    return m_ColorMatrix;
}
