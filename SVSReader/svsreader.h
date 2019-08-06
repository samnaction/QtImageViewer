#ifndef SVSREADER_H
#define SVSREADER_H

#include "svsreader_global.h"
#include <iostream>
#include <string>
#include "tiffio.h"
#include <QImage>

#define	MAXLAYERS 100
#define	MAXLEVELS 10
#define	MAXFILES (MAXLAYERS * MAXLEVELS)
#define	maxThumbWidth 2048		// max thumbnail dimensions
#define	maxThumbHeight 1536

class SVSREADERSHARED_EXPORT SVSReader
{

public:
    SVSReader();

    QImage Open(std::string filePath);
    void closeSVSReader();

private:
    std::string m_FilePath;
    unsigned char * m_ThumbRawData;
    long m_ThumbWidth;
    long m_ThumbHeight;
    double	m_ThumbZoom;

    struct
    {
        long	width,
        height;
    }
    m_FileTable[MAXFILES];

    TIFF *m_TIFFArray[MAXFILES];

    int m_FileCount;
    int m_ThumbFileIndex;
    int m_ThumbDirectoryIndex;

private:
    bool openTiff(std:: string filepath, int di);

    bool loadThumbDataTIFF (TIFF *thfp, long xOff, long yOff, long width, long height);

    void mergeThumbData (unsigned char *iData,int iWidth, int iHeight, double iZoom, long xOff, long yOff);

    void MoveImage (unsigned char *dBuff, int dRW, int dC, int dR, int dW, int dH,
                       unsigned char *sBuff, int sRW, int sC, int sR);

    void ScaleImage (uchar *dBuff, int dRW, int dC, int dR, int dW, int dH,
                        uchar *sBuff, int sRW, int sC, int sR, int sW, int sH);

    QImage ConvertByteArrayToQImage();


};

#endif // SVSREADER_H
