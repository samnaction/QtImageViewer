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
    Q_PROPERTY(QString filePath READ filePath WRITE setfilePath)
public:
    explicit Slide(QString _slideName = "", QString _filePath = "", QString _modifiedDate="");
    QString m_slideName;
    QString m_modifiedDate;
    QString  m_filePath;
    QString slideName() const;
    void setSlideName(QString name);
    QString modifiedDate() const;
    void setModifiedDate(QString date);

    QString filePath() const;
    void setfilePath(QString image);

private:

signals:

    void slideNameChanged(QString names);

    void modifiedDateChanged(QString favoriteColor);
};

#endif // SLIDE_H
