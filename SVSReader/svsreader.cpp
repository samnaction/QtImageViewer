#include "svsreader.h"


SVSReader::SVSReader(): m_FileCount(0)
{
}

void SVSReader::closeSVSReader()
{

    while(m_FileCount)
    {
        --m_FileCount;
        TIFFClose(m_TIFFArray[m_FileCount]);
         m_TIFFArray[m_FileCount] = nullptr;

    }

    delete[] m_ThumbRawData;
}

QImage SVSReader::ConvertByteArrayToQImage()
{
    QImage Image(m_ThumbWidth,m_ThumbHeight, QImage::Format_RGB32);
    for (int r =0; r < m_ThumbHeight; r++)
    {
        for(int c= 0; c < m_ThumbWidth; c++)
        {
            QColor color(m_ThumbRawData[3 * (r * m_ThumbWidth + c)],m_ThumbRawData[3 * (r * m_ThumbWidth + c) + 1],
                    m_ThumbRawData[3 * (r * m_ThumbWidth + c) + 2]);

            Image.setPixel(c,r,color.rgb());
        }

    }

    return Image;
}

QImage SVSReader::Open(std::string filePath)
{

    long xOff = 0, yOff = 0, zOff = 0;

    int fi = m_FileCount;

    if(!openTiff(filePath, 0))
    {
        QImage();
    }

    if(TIFFReadDirectory(m_TIFFArray[fi]))
    {
        int di = 1;

        if(zOff == 0)
        {
            m_ThumbFileIndex = fi;
            m_ThumbDirectoryIndex = di;
        }

        while(TIFFReadDirectory(m_TIFFArray[fi]))
        {
            int subtype = 0;
            di++;
            TIFFGetField(m_TIFFArray[fi], TIFFTAG_SUBFILETYPE, &subtype);

            switch (subtype) {
            case 0:
                openTiff(filePath, di);

            }

        }

        if (zOff == 0)
        {
            TIFFSetDirectory(m_TIFFArray[m_ThumbFileIndex], (uint16)m_ThumbDirectoryIndex);
            loadThumbDataTIFF(m_TIFFArray[m_ThumbFileIndex],
                              xOff, yOff, m_FileTable[0].width, m_FileTable[0].height);
        }

    }

    TIFFSetDirectory(m_TIFFArray[fi],0);

    return ConvertByteArrayToQImage();

}

bool SVSReader::openTiff(std::string filepath, int di)
{
    if(!(m_TIFFArray[m_FileCount] = TIFFOpen(filepath.c_str(), "rm")) )
    {
        return false;
    }
    if(di)
    {
        TIFFSetDirectory(m_TIFFArray[m_FileCount],(uint16)di);
    }

    TIFFGetField(m_TIFFArray[m_FileCount], TIFFTAG_IMAGEWIDTH, &m_FileTable[m_FileCount].width);
    TIFFGetField(m_TIFFArray[m_FileCount], TIFFTAG_IMAGELENGTH, &m_FileTable[m_FileCount].height);

    m_FileCount ++;

    return true;
}

bool SVSReader::loadThumbDataTIFF (TIFF *thfp, long xOff, long yOff, long width, long height)
{
    int	iwidth, iheight,        // image dimensions
            rps;			// image strip height
    short	icspace;		// image color space
    double	izoom;			// image zoom relative to base

    if (TIFFIsTiled(thfp)) {		// if not stripped
        return false;
    }
    // get thumbnail image dimensions
    TIFFGetField(thfp, TIFFTAG_IMAGEWIDTH, &iwidth);
    TIFFGetField(thfp, TIFFTAG_IMAGELENGTH, &iheight);
    TIFFGetField(thfp, TIFFTAG_ROWSPERSTRIP, &rps);

    // get thumbnail's bitspersample and samplesperpixel
    short bitsPerSampl = 0, samplPerPix=0;
    TIFFGetField(thfp, TIFFTAG_BITSPERSAMPLE, &bitsPerSampl);
    TIFFGetField(thfp, TIFFTAG_SAMPLESPERPIXEL, &samplPerPix);

    // get image color space
    TIFFGetField(thfp, TIFFTAG_PHOTOMETRIC, &icspace);
    if (icspace == PHOTOMETRIC_YCBCR)	// if stored as YCC, set JPEG YCC->RGB
        TIFFSetField(thfp, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);

    // allocate thumbnail data buffer
    unsigned char *idata = new unsigned char[3 * iwidth * iheight];
    //byte count in one strip's row
    int rowbytes = (bitsPerSampl*samplPerPix*iwidth)/8;

    // loop through strips, load into buffer
    for (int s = 0; s * rps < iheight; s++)
        TIFFReadEncodedStrip(thfp, s, idata + (s * rps) * rowbytes,
                             (iheight - s * rps > rps ? rps : iheight - s * rps) * rowbytes);

    // calc zoom relative to base
    izoom = (width > height ? (double) width / iwidth : (double) height / iheight);
    // merge thumbnail into existing data

    mergeThumbData(idata, iwidth, iheight, izoom, xOff, yOff);

    return true;				// return; thumbnail loaded
}

void SVSReader::mergeThumbData (unsigned char *iData,int iWidth, int iHeight, double iZoom, long xOff, long yOff)
{
    xOff = (long) (xOff / iZoom + .99999);	// convert offsets to new data's resolution
    yOff = (long) (yOff / iZoom + .99999);

    double	tZoom = std::max(m_ThumbZoom, iZoom);  // default new zoom to whichever is lower res
    // calc new thumbnail dimensions
    int	tWidth = (int) std::max(m_ThumbWidth * m_ThumbZoom / tZoom + .99999, \
                                (xOff + iWidth) * iZoom / tZoom + .99999);
    int tHeight = (int) std::max(m_ThumbHeight * m_ThumbZoom / tZoom + .99999, \
                                 (yOff + iHeight) * iZoom / tZoom + .99999);

    int maxHeight = maxThumbHeight;
    int maxWidth = maxThumbWidth;
    if (tHeight > tWidth) {
        maxHeight = maxThumbWidth;	// for tall thumbnails (e.g. mrxs), swap max dimensions
        maxWidth = maxThumbHeight;
    }
    // if thumbnail too large, half each dimension
    while (tWidth > maxWidth || tHeight > maxHeight) {
        tZoom = tZoom * 2;
        tHeight = (tHeight + 1) / 2;
        tWidth = (tWidth + 1) / 2;
    }
    // allocate new thumbnail buffer
    unsigned char *tData = new unsigned char[3 * tWidth * tHeight];
    memset(tData, 255, 3 * tWidth * tHeight);  // fill buffer to initialize


    if (iZoom == tZoom)			// if new zoom same as existing zoom
        MoveImage(tData, 3 * tWidth, xOff, yOff,  // move new image data into buffer
                  iWidth, iHeight,
                  iData, 3 * iWidth, 0, 0);
    else					// zoom not same, scale required
        ScaleImage(tData, 3 * tWidth,		// scale new image data into buffer
                   (long) (xOff * iZoom / tZoom), (long) (yOff * iZoom / tZoom),
                   (long) (iWidth * iZoom / tZoom + .99999), (long) (iHeight * iZoom / tZoom + .99999),
                   iData, 3 * iWidth, 0, 0, iWidth, iHeight);
    delete [] iData;			// free image data buffer

    m_ThumbRawData = tData;			// setup new buffer for thumbnail data
    m_ThumbWidth = tWidth;
    m_ThumbHeight = tHeight;
    m_ThumbZoom = tZoom;
}

void SVSReader::MoveImage (unsigned char *dBuff, int dRW, int dC, int dR, int dW, int dH,
                           unsigned char *sBuff, int sRW, int sC, int sR)
{
    if (dRW == 3 * dW && sRW == 3 * dW) {			// if possible to move directly
        memcpy(dBuff + dR * dRW, sBuff + sR * sRW, dH * dRW);
        return;
    }
    for (int r = 0; r < dH; r++)				// copy region row by row
        memcpy(dBuff + (dR + r) * dRW + 3 * dC, sBuff + (sR + r) * sRW + 3 * sC, 3 * dW);
}

void SVSReader::ScaleImage (uchar *dBuff, int dRW, int dC, int dR, int dW, int dH,
                            uchar *sBuff, int sRW, int sC, int sR, int sW, int sH)
{
#define	minF	.001			// minimum weighting factor
    float	fX = (float) sW / dW,		// calc scaling factors
            fY = (float) sH / dH;

    int	r, c;				// row, column indices (dest)
    int	ri, ci;				// row, column indices (source)
    float	aY, aX;				// aperture sizes
    int	ys, ye, xs, xe;			// source pel indices
    float	ysF, yeF, xsF, xeF;		// weighting factors
    int	dpi;				// dest pel index
    int	R, G, B;			// pel value accumulators
    /*
    *   Logic for scaling down.  Loop through all destination pels.  Each pel "projects" onto
    *   one or more source pels.  The source pels form a 2D array of the following form:
    *
    *	@****@
    *	*++++*
    *	*++++*
    *	@****@
    *
    *   The "@" pels are partial in X and Y - the fraction to which they contribute is computed.
    *   The "*" pels are partial in X or Y - the fraction to which they contribute is computed.
    *   The "+" pels are full and added into the sum.  The entire sum is divided by the number
    *   of pels which contributed.
    */
    if (fX >= 1 && fY >= 1)			// scaling down

        for (r = 0; r < dH; r++) {		// loop through destination rows
            ysF = r * fY;			// calc Y indices, weighting factors
            yeF = ysF + fY;
            ys = (int) ysF;
            ye = (int) yeF;
            ysF = 1 - (ysF - ys);
            yeF = yeF - ye;
            ys += sR;
            ye += sR;

            for (c = 0; c < dW; c++) {	// loop through destination columns
                xsF = c * fX;		// calc X indices, weighting factors
                xeF = xsF + fX;
                xs = (int) xsF;
                xe = (int) xeF;
                xsF = 1 - (xsF - xs);
                xeF = xeF - xe;
                xs += sC;
                xe += sC;
                // upper left corner
                R = (int) (sBuff[ys * sRW + 3 * xs] * xsF * ysF);
                G = (int) (sBuff[ys * sRW + 3 * xs + 1] * xsF * ysF);
                B = (int) (sBuff[ys * sRW + 3 * xs + 2] * xsF * ysF);
                // middle of top row
                for (ci = xs + 1; ci < xe; ci++) {
                    R += (int) (sBuff[ys * sRW + 3 * ci] * ysF);
                    G += (int) (sBuff[ys * sRW + 3 * ci + 1] * ysF);
                    B += (int) (sBuff[ys * sRW + 3 * ci + 2] * ysF);
                }
                // upper right corner
                if (xeF > minF) {
                    R += (int) (sBuff[ys * sRW + 3 * xe] * xeF * ysF);
                    G += (int) (sBuff[ys * sRW + 3 * xe + 1] * xeF * ysF);
                    B += (int) (sBuff[ys * sRW + 3 * xe + 2] * xeF * ysF);
                }
                // middle rows
                for (ri = ys + 1; ri < ye; ri++) {
                    // left edge of middle rows
                    R += (int) (sBuff[ri * sRW + 3 * xs] * xsF);
                    G += (int) (sBuff[ri * sRW + 3 * xs + 1] * xsF);
                    B += (int) (sBuff[ri * sRW + 3 * xs + 2] * xsF);
                    // middle columns of middle rows
                    for (ci = xs + 1; ci < xe; ci++) {
                        R += sBuff[ri * sRW + 3 * ci];
                        G += sBuff[ri * sRW + 3 * ci + 1];
                        B += sBuff[ri * sRW + 3 * ci + 2];
                    }
                    // right edge of middle rows
                    if (xeF > minF) {
                        R += (int) (sBuff[ri * sRW + 3 * xe] * xeF);
                        G += (int) (sBuff[ri * sRW + 3 * xe + 1] * xeF);
                        B += (int) (sBuff[ri * sRW + 3 * xe + 2] * xeF);
                    }
                }
                // bottom row
                if (yeF > minF) {
                    // lower left corner
                    R += (int) (sBuff[ye * sRW + 3 * xs] * xsF * yeF);
                    G += (int) (sBuff[ye * sRW + 3 * xs + 1] * xsF * yeF);
                    B += (int) (sBuff[ye * sRW + 3 * xs + 2] * xsF * yeF);
                    // middle of bottom row
                    for (ci = xs + 1; ci < xe; ci++) {
                        R += (int) (sBuff[ye * sRW + 3 * ci] * yeF);
                        G += (int) (sBuff[ye * sRW + 3 * ci + 1] * yeF);
                        B += (int) (sBuff[ye * sRW + 3 * ci + 2] * yeF);
                    }
                    // bottom right corner
                    if (xeF > minF) {
                        R += (int) (sBuff[ye * sRW + 3 * xe] * xeF * yeF);
                        G += (int) (sBuff[ye * sRW + 3 * xe + 1] * xeF * yeF);
                        B += (int) (sBuff[ye * sRW + 3 * xe + 2] * xeF * yeF);
                    }
                }
                // calc R,G,B values, store as dest pel
                dpi = (dR + r) * dRW + 3 * (dC + c);

                dBuff[dpi]     = (uchar) (R / (fX * fY));
                dBuff[dpi + 1] = (uchar) (G / (fX * fY));
                dBuff[dpi + 2] = (uchar) (B / (fX * fY));
            }			// bottom of column loop
        }				// bottom of row loop
    /*
        *   Logic for scaling up.  Loop through all destination pels.  Each pel is "projected"
        *   onto one or more source pels, using an "aperture".  Pels at the left, right, top,
        *   or bottom of the destination region have an aperture of 1 (straight projection).
        *   Other pels have a larger aperture, bounded by the smaller of the size of a source pel
        *   and number of pels available.
        */
    else					// scaling up

        for (r = 0; r < dH; r++) {		// loop through rows in destination
            aY = 1 / fY;			// calc Y aperture
            if (r < (aY - 1) / 2)
                aY = 1.0f + 2 * r;
            else if (dH - 1 - r < (aY - 1) / 2)
                aY = 1.0f + 2 * (dH - 1 - r);
            // calc Y indices, weighting factors
            ysF = (r - (aY - 1) / 2) * fY;
            yeF = (r + 1 + (aY - 1) / 2) * fY;
            ys = (int) ysF;
            ye = (int) yeF;
            if (ys == ye || yeF - ye < minF) {
                ye = ys;
                ysF = 1;
                yeF = 0;
            } else {
                ysF = (1 - (ysF - ys)) / (aY * fY);
                yeF = (yeF - ye) / (aY * fY);
            }
            ys += sR;
            ye += sR;

            for (c = 0; c < dW; c++) {	// loop through columns in destination row
                aX = 1 / fX;		// calc X aperture
                if (c < (aX - 1) / 2)
                    aX = 1.0f + 2 * c;
                else if (dW - 1 - c < (aX - 1) / 2)
                    aX = 1.0f + 2 * (dW - 1 - c);
                // calc X indices, weighting factors
                xsF = (c - (aX - 1) / 2) * fX;
                xeF = (c + 1 + (aX - 1) / 2) * fX;
                xs = (int) xsF;
                xe = (int) xeF;
                if (xs == xe || xeF - xe < minF) {
                    xe = xs;
                    xsF = 1;
                    xeF = 0;
                } else {
                    xsF = (1 - (xsF - xs)) / (aX * fX);
                    xeF = (xeF - xe) / (aX * fX);
                }
                xs += sC;
                xe += sC;
                // each destination pel is combination of four source pels
                dpi = (dR + r) * dRW + 3 * (dC + c);

                dBuff[dpi]     = (uchar) (
                            sBuff[ys * sRW + 3 * xs] * ysF * xsF +
                        sBuff[ys * sRW + 3 * xe] * ysF * xeF +
                        sBuff[ye * sRW + 3 * xs] * yeF * xsF +
                        sBuff[ye * sRW + 3 * xe] * yeF * xeF);
                dBuff[dpi + 1] = (uchar) (
                            sBuff[ys * sRW + 3 * xs + 1] * ysF * xsF +
                        sBuff[ys * sRW + 3 * xe + 1] * ysF * xeF +
                        sBuff[ye * sRW + 3 * xs + 1] * yeF * xsF +
                        sBuff[ye * sRW + 3 * xe + 1] * yeF * xeF);
                dBuff[dpi + 2] = (uchar) (
                            sBuff[ys * sRW + 3 * xs + 2] * ysF * xsF +
                        sBuff[ys * sRW + 3 * xe + 2] * ysF * xeF +
                        sBuff[ye * sRW + 3 * xs + 2] * yeF * xsF +
                        sBuff[ye * sRW + 3 * xe + 2] * yeF * xeF);
            }			// column loop
        }				// row loop

}
