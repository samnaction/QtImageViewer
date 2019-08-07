#include "ImageProvider.h"
#include "svsreader.h"
/**
* @brief Image provider that is used to handle the live image stream in the QML viewer.
 */
ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
   {

    SVSReader reader;
    QImage image = reader.Open(id.toStdString());
    reader.closeSVSReader();

    QPixmap map;
    map.convertFromImage(image);

    return map;

   }

