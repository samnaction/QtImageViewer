#ifndef SLIDE_H
#define SLIDE_H

#include <QObject>
#include <QImage>
#include <QGraphicsScene>
#include <imageprovider.h>

class Slide : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString slideName READ slideName WRITE setSlideName NOTIFY slideNameChanged)
    Q_PROPERTY(QString modifiedDate READ modifiedDate WRITE setModifiedDate NOTIFY modifiedDateChanged)
    Q_PROPERTY(QImage thumbnailImage READ thumbnailImage WRITE setThumbnailImage NOTIFY modifiedThumbnailImage)
public:
    explicit Slide(QString _slideName = "", QString _modifiedDate="");
    QString m_slideName;
    QString m_modifiedDate;
    QImage  m_thumbnailImage;
    ImageProvider ip;
    QString slideName() const;
    void setSlideName(QString name);
    QString modifiedDate() const;
    void setModifiedDate(QString date);

    QImage thumbnailImage() const;
    void setThumbnailImage(QImage image);
    void getThumbnail();

private:

signals:

    void slideNameChanged(QString names);

    void modifiedDateChanged(QString favoriteColor);

    void modifiedThumbnailImage(QImage image);

};

#endif // SLIDE_H
