#include<slide.h>
#include<viewspace.h>
#include <QImageReader>

Slide ::Slide(QString _slideName, QString _filePath,QString _modifiedDate)
{
    m_slideName = _slideName;
    m_filePath = _filePath;
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

QString Slide::filePath() const
{
    return m_filePath;
}

void Slide::setfilePath(QString filePath)
{
    m_filePath = filePath;
}



