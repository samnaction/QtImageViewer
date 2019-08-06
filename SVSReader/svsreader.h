#ifndef SVSREADER_H
#define SVSREADER_H

#include "svsreader_global.h"
#include <iostream>
#include <string>
#include "tiffio.h"
#include <QImage>

#define	AIC_MAXLAYERS 100
#define	AIC_MAXLEVELS 10
#define	AIC_MAXFILES (AIC_MAXLAYERS * AIC_MAXLEVELS)
#define	maxThumbWidth 2048		// max thumbnail dimensions
#define	maxThumbHeight 1536

class SVSREADERSHARED_EXPORT SVSReader
{

public:
    SVSReader();

    QImage Open(std::string filePath);

private:
    std::string m_FilePath;
    unsigned char * m_ThumbRawData;
    long m_ThumbWidth;
    long m_ThumbHeight;
    double	m_ThumbZoom;

    struct	{			// file table entry
        long	width,		// file image dimensions
        height;
        long	xOff,		// positional offsets in global grid
        yOff;
        int		tileWidth,	// file tile dimensions
        tileHeight,
        rowsPerStrip;	// 0=tiled, >0=width,height canonical
        int		xTiles,		// number of tiles in X, Y
        yTiles;
        double	zoom;		// zoom relative to base (4 = 25% level)
        int		bitsPerSampl;	// sample bit depth
        int		samplPerPix;	// number of pixel spectral components
    }
    m_FileTable[AIC_MAXFILES];

    TIFF *m_TIFFArray[AIC_MAXFILES];

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
