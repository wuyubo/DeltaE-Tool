#include "data.h"
#include <QFile>
const cRGB_t defaultPatternList[32] = {
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

double default_Native_RGB_XYZxy[4*5*9] = {
    //RX
     0.107084296643734,
     0.345137089490891,
     1.49937164783478,
     3.6866021156311,
     7.04250001907349,
     11.6406202316284,
     17.6222400665283,
     24.7057495117188,
     33.4616317749023,
     //RY
     0.1120890006423,
     0.243019700050354,
     0.881242513656616,
     2.08757400512695,
     3.93095850944519,
     6.46041202545166,
     9.75764751434326,
     13.6366443634033,
     18.4037284851074,
    //RZ
     0.195870697498322,
     0.214340701699257,
     0.312890201807022,
     0.493769288063049,
     0.748154520988464,
     1.08822524547577,
     1.42198657989502,
     1.70512223243713,
     1.81727421283722,
    //R_x
     0.258007109165192,
     0.430078715085983,
     0.556662023067474,
     0.588167548179626,
     0.600813210010529,
     0.60662168264389,
     0.611843526363373,
     0.616910934448242,
     0.623323202133179,
    //R_y
     0.27006533741951,
     0.30282923579216,
     0.327173233032227,
     0.333055555820465,
     0.335359841585159,
     0.336668163537979,
     0.338785141706467,
     0.340511620044708,
     0.34282460808754,
    //GX
     0.107084296643734,
     0.315345406532288,
     1.36632812023163,
     3.37892985343933,
     6.5303316116333,
     10.8149785995483,
     16.4813404083252,
     24.0750026702881,
     33.8349723815918,
    //GY
     0.1120890006423,
     0.529766321182251,
     2.63785624504089,
     6.69684982299805,
     12.8432950973511,
     21.2975044250488,
     32.3627243041992,
     46.8918876647949,
     64.3945999145508,
    //GZ
     0.195870697498322,
     0.245300605893135,
     0.50388491153717,
     0.987670600414276,
     1.7710794210434,
     2.77366948127747,
     4.0166802406311,
     5.51784133911133,
     6.87772369384766,
    //G_x
     0.258007109165192,
     0.289198309183121,
     0.303084999322891,
     0.305413752794266,
     0.308840036392212,
     0.310007780790329,
     0.311787903308868,
     0.314768731594086,
     0.321908891201019,
     //G_y
     0.27006533741951,
     0.485840350389481,
     0.585141003131866,
     0.605312943458557,
     0.607400000095367,
     0.610485911369324,
     0.612226009368896,
     0.613088190555573,
     0.612655878067017,
     //BX
     0.107084296643734,
     0.1961620002985,
     0.720336079597473,
     1.74482154846191,
     3.32229566574097,
     5.41422557830811,
     8.15475749969482,
     11.5544490814209,
     16.3915195465088,
    //BY
     0.1120890006423,
     0.149847403168678,
     0.377540111541748,
     0.800832688808441,
     1.49423980712891,
     2.41188263893127,
     3.64533162117004,
     5.26720571517944,
     8.68611621856689,
    //BZ
     0.195870697498322,
     0.662986516952515,
     3.42801117897034,
     8.84207630157471,
     17.0297832489014,
     28.001953125,
     42.2718811035156,
     59.8446006774902,
     82.2953338623047,
    // B_x
     0.258007109165192,
     0.194413080811501,
     0.15915909409523,
     0.153219431638718,
     0.15207576751709,
     0.151116892695427,
     0.150813028216362,
     0.150711014866829,
     0.152659639716148,
    //B_y
     0.27006533741951,
     0.148511409759521,
     0.0834179222583771,
     0.0703241676092148,
     0.0683977827429771,
     0.0673182532191277,
     0.067416287958622,
     0.0687030553817749,
     0.0808966755867004,
    //WX
     0.107084296643734,
     0.652173101902008,
     3.4422402381897,
     8.85540390014648,
     17.0583400726318,
     28.2264614105225,
     42.8356590270996,
     60.9521636962891,
     83.5444259643555,
    // WY
     0.1120890006423,
     0.709529399871826,
     3.74653053283691,
     9.57651710510254,
     18.4557781219482,
     30.5781631469727,
     46.4085311889648,
     66.467887878418,
     91.3735733032227,
    //WZ
     0.195870697498322,
     0.742805480957031,
     3.92300033569336,
     10.1739349365234,
     19.5670108795166,
     32.064094543457,
     48.1594047546387,
     67.6458282470703,
     90.6668167114258,
    //W_x
     0.258007109165192,
     0.309893369674683,
     0.309783220291138,
     0.309566110372543,
     0.309694826602936,
     0.3106290102005,
     0.311750650405884,
     0.312469631433487,
     0.314567774534225,
     //W_y
     0.27006533741951,
     0.337147384881973,
     0.337167710065842,
     0.334774702787399,
     0.335065364837646,
     0.336509227752686,
     0.337753385305405,
     0.340745836496353,
     0.34404668211937,

};
Data::Data(QObject *parent) : QObject(parent)
{
    init_PatRgbList();
}

void Data::init_PatRgbList()
{
    int i;
    pat_RgbCount = 32;
    for(i = 0; i < 32; i++)
    {
        pat_RgbList[i] = defaultPatternList[i];
    }
    for(i = 0; i < 4*5*9; i++)
    {
        m_Native_RGBW[i] = default_Native_RGB_XYZxy[i];
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
bool Data::saveCompGma(BYTE *pCompressGma, int size)
{
    QString data, temp, path;
    int i;
    path = GAMMA_TABLE_PATH;
    if(size > _MAX_COMP_GMA_COUT)
    {
        return false;
    }
    for(i = 0; i < size; i++)
    {
        m_CompressGma[i] = pCompressGma[i];
    }
    data.append("code byte tNormal_GamaR[]=\n{\n ");
    for(i = 0; i < size; i++)
    {
        temp.sprintf("0x%x, ", m_CompressGma[i]);

        if(i/64 == 1 && i%64 == 0)
        {
            data.append("};\n");
            data.append("code byte tNormal_GamaG[]=\n{\n ");
        }
        if(i/64 == 2 && i%64 == 0)
        {
            data.append("};");
            data.append("code byte tNormal_GamaB[]=\n{\n ");
        }
        if((i+1)%4==0)
        {
            temp.append("\n ");
        }
        data.append(temp);
    }
    data.append("};");
    return saveFile(path, data);
}

bool Data::saveColorMatrix(BYTE *psRgbCM, int size)
{
    QString data, temp, path;
    int i;
    path = COLOR_MATRIX_PATH;
    if(size > _MAX_COLOR_MATRIX_COUT)
    {
        return false;
    }
    for(i = 0; i < size; i++)
    {
        m_ColorMatrix[i] = psRgbCM[i];
    }
    data.append("code byte ColorMatrixTable[]=\n{\n ");
    for(i = 0; i < size; i++)
    {
        temp.sprintf("0x%x, ", m_ColorMatrix[i]);
        if((i+1)%6 == 0)
        {
            temp.append("\n ");
        }
        data.append(temp);
    }
    data.append("\n};\n");
    return saveFile(path, data);
}

bool Data::readCompGma(BYTE *pCompressGma, int size)
{
    QString data = readFile(GAMMA_TABLE_PATH);
    QStringList dataList;
    int i;
    bool ok;
    data.replace(QString("\n"), QString(""));
    data.replace(QString("};"), QString(""));
    data.replace(QString("code byte tNormal_GamaR[]={"), QString(""));
    data.replace(QString("code byte tNormal_GamaG[]={"), QString(""));
    data.replace(QString("code byte tNormal_GamaB[]={"), QString(""));
    data.replace(QString(" "), QString(""));
    data.replace(QString("0x"), QString(""));

    dataList = data.split(",");
    if(dataList.length() < size)
    {
        return false;
    }
    for(i = 0; i < size; i++)
    {
        pCompressGma[i] = dataList[i].toInt(&ok, 16);
    }
    return true;
}

bool Data::readColorMatrix(BYTE *sRgbCM, int size)
{
    QString data = readFile(COLOR_MATRIX_PATH);
    QStringList dataList;
    int i;
    bool ok;
    data.replace(QString("\n"), QString(""));
    data.replace(QString("};"), QString(""));
    data.replace(QString("code byte ColorMatrixTable[]={"), QString(""));
    data.replace(QString(" "), QString(""));
    data.replace(QString("0x"), QString(""));

    dataList = data.split(",");
    if(dataList.length() < size)
    {
        return false;
    }
    for(i = 0; i < size; i++)
    {
        sRgbCM[i] = dataList[i].toInt(&ok, 16);
    }
    return true;
}

BYTE *Data::getCompGma()
{
    //readCompGma(m_CompressGma, 3*64);
    return m_CompressGma;
}

BYTE *Data::getColorMatrix()
{
    //readColorMatrix(m_ColorMatrix, 18);
    return m_ColorMatrix;
}

void Data::getPanelNativeData(double * panelNativeData)
{
    int i;
    for(i = 0; i < 4*5*9; i++)
    {
        panelNativeData[i] = m_Native_RGBW[i];
    }
}
bool Data::setPanelNativeData(int rgbw, int index, double dX, double dY,
                        double dZ, double d_y, double d_x)
{
    int i;
    i = rgbw*5*9;
    i += index;
    if(i >= _MAX_NATIVE_COUT)
    {
        return false;
    }
    m_Native_RGBW[i] = dX;
    i += 9;
    m_Native_RGBW[i] = dY;
    i += 9;
    m_Native_RGBW[i] = dZ;
    i += 9;
    m_Native_RGBW[i] = d_x;
    i += 9;
    m_Native_RGBW[i] = d_y;
    return true;
}
bool Data::savePanelNativeData()
{
    QString data, temp, path;
    int i, rgbw = 0;
    path = "./Natvie_PL9.cvt";
    data.append("LEVEL9\n");
    for(i = 0 ; i<4*5*9; i++)
    {
       temp.sprintf("%lf\n", m_Native_RGBW[i]);
       rgbw = i/(5*9);
       if(rgbw == 0 && i%9 == 0)
       {
           data.append("\nR");
           switch((i-rgbw*(5*9))/9)
           {
                case 0:
                    data.append("X\n");
                    break;
                case 1:
                    data.append("Y\n");
                    break;
                case 2:
                    data.append("Z\n");
                    break;
                case 3:
                    data.append("_x\n");
                    break;
                case 4:
                    data.append("_y\n");
                break;
                default:
                break;
           }
       }
       else if(rgbw == 1 && i%9 == 0)
       {
            data.append("\nG");
            switch((i-rgbw*(5*9))/9)
            {
                 case 0:
                     data.append("X\n");
                     break;
                 case 1:
                     data.append("Y\n");
                     break;
                 case 2:
                     data.append("Z\n");
                     break;
                 case 3:
                     data.append("_x\n");
                     break;
                 case 4:
                     data.append("_y\n");
                 break;
                 default:
                 break;
            }
       }
       else if(rgbw == 2 && i%9 == 0)
       {
            data.append("\nB");
            switch((i-rgbw*(5*9))/9)
            {
                 case 0:
                     data.append("X\n");
                     break;
                 case 1:
                     data.append("Y\n");
                     break;
                 case 2:
                     data.append("Z\n");
                     break;
                 case 3:
                     data.append("_x\n");
                     break;
                 case 4:
                     data.append("_y\n");
                 break;
                 default:
                 break;
            }
       }
       else if(rgbw == 3 && i%9 == 0)
       {
            data.append("\nW");
            switch((i-rgbw*(5*9))/9)
            {
                 case 0:
                     data.append("X\n");
                     break;
                 case 1:
                     data.append("Y\n");
                     break;
                 case 2:
                     data.append("Z\n");
                     break;
                 case 3:
                     data.append("_x\n");
                     break;
                 case 4:
                     data.append("_y\n");
                 break;
                 default:
                 break;
            }
       }
       data.append(temp);
    }
    return saveFile(path, data);
}
