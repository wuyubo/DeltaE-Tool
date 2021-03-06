#include "ddc_aps.h"

namespace ddc {

#define     FEEDBACK_LEN         9
#define     RETRY_CNT            3
#define     COMMON_DELAY         100
#define     LASTPACK_DELAY       600
#define     WR_GAMMA_DELAY       1200
#define     WR_COLORMATRIX_DELAY 600

//example
quint8 enterAtcmdtab[]={0xc0,0x43,0x56,0x54,0x45,0x41,0x54,0x4f,0x4e};

//Delta-E releated instructions
//先发送MS_WR_BLOCK ，进入调试模式
quint8 enterDebugCmd[]={0xCC, 0x90, 0x01};
//进入AutoGamma Pattern
quint8 enterAutoGammaCmd[]={0xCC, 0x30, 0x01};
//输入RGBW 4X9Pattern，每秒2～3个Pattern
quint8 writeRGBPaternCmd[]={0xCC, 0x31, 0x00, 0x00, 0x00};
//退出Pattern 模式，退出调试模式
quint8 exitAutoGammaCmd[]={0xCC, 0x30, 0X00};
quint8 exitDebugCmd[]={0xCC, 0x90, 0X00};
quint8 WRPostGmaCmd[]={0xCC, 0x32, 0X00, 0X00, 0X00, 0X00, 0X00};
quint8 WRColorMatrixCmd[]={0xCC, 0x84, 0X00, 0X00};
quint8 ReadACKCmd[]={0xCC, 0x8E};
quint8 SaveGmaCmd[]={0xCC, 0x33, 0x00, 0x00, 0x16};
quint8 SaveColorMatrix[]={0xCC, 0x86, 0x01};

//verify function declarations

quint8 getFeedbackchecksum(quint8 *data,quint8 datalen)
{
    if (data == NULL) return false;
    quint8 checksum=0;
    for (int i = 0; i < datalen; i++)
    {
        checksum^=data[i];
    }
    return checksum;
}

quint8 getRevChecksum(quint8 *data, quint8 count)
{
   quint8 CheckSum;
   quint8 i = 0;
   CheckSum = 0x50;

   for( ; count > 0; count-- )
   {
       CheckSum ^= data[i++];
   }

   return CheckSum;
}

bool CommonFeedbackverify(quint8 *feedback,quint8 fdlen,quint8 *data,quint8 len)
{
    static quint8 checkSum = 0;
    Q_UNUSED(fdlen);
    Q_UNUSED(data);
    Q_UNUSED(len);
    if(fdlen < 3)
    {
        return false;
    }
    if(len > 0)
    {
        checkSum = getFeedbackchecksum(data, len);
    }
    if(feedback[0] == 0x6E)
    {
        if(feedback[1] == 0x80)
        {
            if( getRevChecksum(feedback, 2) == feedback[2] )
            {
                return true;
            }
        }else if(feedback[1] == 0x81)
        {
            if(checkSum == feedback[2])
            {
                if(getRevChecksum(feedback, 3) == feedback[3])
                {
                    return true;
                }
            }
        }
        else if(feedback[1] == 0x82)
        {
            if( getRevChecksum(feedback, 2) == feedback[2] )
            {
                return true;
            }

        }
    }

    return false;
}

burndata_t CommonAssemble_Alloc(quint8 *head,quint8 headsize,quint8 *body,quint8 bodysize)
{
    quint8 *wholepackage = new quint8[headsize+bodysize];
    burndata_t tmpburndata;

    memcpy(wholepackage,head,headsize);
    memcpy(wholepackage+headsize,body,bodysize);

    tmpburndata.data = wholepackage;
    tmpburndata.size = headsize+bodysize;
    return tmpburndata;
}

int CommonSetpara(QString& text,quint8 *head,quint8& headsize)
{
    Q_UNUSED(headsize);
    QStringList paralist = text.split(' ', QString::SkipEmptyParts);

    for(auto x:paralist)
    {
        if(x.length()>2)
        {
           // DEBUGMSG()<<"error format.";
            return -1;
        }
    }
    if(paralist.size()>1)
    {
        return -2;
    }

    //some defense condition
    bool *ok=0;
    int value = text.toInt(ok,16);
    //DEBUGMSG()<<"para:"<<value;
    *(head+5) = (quint8)value;
    return 1;
}

int SNsetpara(QString& text,quint8 *head,quint8& headsize)
{
    Q_UNUSED(headsize);
    QStringList paralist = text.split(' ', QString::SkipEmptyParts);

    for(auto x:paralist)
    {
        if(x.length()>2)
        {
           // DEBUGMSG()<<"error format.";
            return -1;
        }
    }
    if(paralist.size()>1)
    {
        return -2;
    }

    //some defense condition
    bool *ok=0;
    int value = text.toInt(ok,16);
    *(head+4) = (quint8)value;
    return 1;
}

int SetMultiparas(QString& text,quint8 *head,quint8 &headsize)
{
    if(text.count()>20)
    {
        return -2;
    }
    if(text.contains(' '))
    {
        return -1;
    }

    QByteArray bytes = text.toUtf8();
    //std::cout<<"Size:"<<bytes.size()<<std::endl;

    for(int i =0;i<bytes.size();i++)
    {
        //std::cout<<(int)bytes.at(i)<<std::endl;
        *(head+5+i) = (int)bytes.at(i);
    }

    *(head+4) = bytes.size();
    headsize = bytes.size() + 5;
    return 1;
}


/*******************************************************
 ** No Parameter Instructions.
 *******************************************************/
//example
burnCmd_t enterATcmd =
{
    QString("EnterATStatus"),
    QString("Enter the AT status, so can the other instructions execute!"),
    nullptr,
    enterAtcmdtab,
    sizeof(enterAtcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t enterDeltaEDebugcmd =
{
    QString("EnterDeltaEDebugStatus"),
    QString("Enter the DeltaE Debug Status, so can the other instructions execute!"),
    nullptr,
    enterDebugCmd,
    sizeof(enterDebugCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t enterDeltaEAutoGammacmd =
{
    QString("EnterAutoGammaStatus"),
    QString("Enter the DeltaE Auto Gamma Status, so can the other instructions execute!"),
    nullptr,
    enterAutoGammaCmd,
    sizeof(enterAutoGammaCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t writeDeltaERGBPaternCmd =
{
    QString("RGBPaternStatus"),
    QString("write DeltaE RGB Patern, so can the other instructions execute!"),
    nullptr,
    writeRGBPaternCmd,
    sizeof(writeRGBPaternCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t exitDeltaEDebugcmd =
{
    QString("exitDeltaEDebugStatus"),
    QString("exit the DeltaE Debug Status, so can the other instructions execute!"),
    nullptr,
    exitDebugCmd,
    sizeof(exitDebugCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t exitDeltaEAutoGammacmd =
{
    QString("exitAutoGammaStatus"),
    QString("exit the DeltaE Auto Gamma Status, so can the other instructions execute!"),
    nullptr,
    exitAutoGammaCmd,
    sizeof(exitAutoGammaCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};
burnCmd_t writeDeltaEPostGammacmd =
{
    QString("WRPostGammaStatus"),
    QString("write the DeltaE Post Gamma , so can the other instructions execute!"),
    nullptr,
    WRPostGmaCmd,
    sizeof(WRPostGmaCmd),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    WR_GAMMA_DELAY,
    COMMON_DELAY,
};
burnCmd_t writeDeltaEColorMatrixcmd =
{
    QString("WRColorMatrixStatus"),
    QString("Write the DeltaE Color Matrix, so can the other instructions execute!"),
    nullptr,
    WRColorMatrixCmd,
    sizeof(WRColorMatrixCmd),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    WR_COLORMATRIX_DELAY,
    COMMON_DELAY,
};

burnCmd_t readDeltaEACKcmd =
{
    QString("ReadACKStatus"),
    QString("Read the DeltaE ACK, so can the other instructions execute!"),
    nullptr,
    ReadACKCmd,
    sizeof(ReadACKCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t saveDeltaEGmacmd =
{
    QString("SaveGmaStatus"),
    QString("Save the DeltaE Gamma, so can the other instructions execute!"),
    nullptr,
    SaveGmaCmd,
    sizeof(SaveGmaCmd),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};
burnCmd_t SaveDeltaEColorMatrixcmd =
{
    QString("SaveColorMatrix"),
    QString("Save the DeltaE Gamma, so can the other instructions execute!"),
    nullptr,
    SaveColorMatrix,
    sizeof(SaveColorMatrix),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    RETRY_CNT,
    COMMON_DELAY,
    COMMON_DELAY,
};

}
