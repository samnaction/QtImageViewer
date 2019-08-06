#include<slide.h>
#include<viewspace.h>
#include <QImageReader>

Slide ::Slide(QString _slideName,QString _modifiedDate)
{
    m_slideName = _slideName;
    m_modifiedDate = _modifiedDate;
}

QString Slide::slideName() const
{
    return m_slideName;
}

void Slide::setSlideName(QString name)
{
    m_slideName = name;
    emit slideNameChanged(m_slideName);
}

QString Slide::modifiedDate() const
{
    return m_modifiedDate;
}

void Slide::setModifiedDate(QString date)
{
    m_modifiedDate = date;
    emit modifiedDateChanged(m_modifiedDate);
}

QImage Slide::thumbnailImage() const
{
    return m_thumbnailImage;
}

void Slide::setThumbnailImage(QImage image)
{
    m_thumbnailImage = image;
    ip.updateImage(m_thumbnailImage);
    emit modifiedThumbnailImage(m_thumbnailImage);
}

void Slide::getThumbnail()
 {

 }


