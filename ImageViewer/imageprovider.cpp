#include "imageprovider.h"
#include "svsreader.h"
#include<QFileInfo>
/**
* @brief Image provider that is used to handle the live image stream in the QML viewer.
 */
ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QFileInfo info(id);
    QPixmap map;
    if(info.suffix() == "svs")
    {
        SVSReader reader;
        QImage image = reader.Open(id.toStdString());
        reader.closeSVSReader();
        map.convertFromImage(image);
    }
    else
    {
        map.load(id);
    }
    return map;
}

