#ifndef MAINVIEW_H
#define MAINVIEW_H
#include <QAbstractListModel>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <vector>
#include <QObject>
#include <iostream>
#include <list>
#include <string>
#include  <QQuickView>
#include <slide.h>

class MainView : public QQuickView
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> slideList READ slideList WRITE addSlides NOTIFY slidesChanged)
   Q_PROPERTY(QString selectedSlide READ selectedSlide WRITE setSelctedSlide NOTIFY selectedSlideChanged)

public:
    explicit MainView(QQuickView *parent = nullptr);
    bool initialize();
    Q_INVOKABLE QList<QObject *> slides() const;
    Q_INVOKABLE void setSlides(QList<QUrl> fileNames);
    Q_INVOKABLE void clearSlides();
    Q_INVOKABLE void deleteSlide(int index);
    Q_INVOKABLE void setSelected(int index);
    QList<QObject*> slideList() const;
     void addSlides(QList<QObject*> slideList);
    ~MainView();
     QString selectedSlide() const;
     void setSelctedSlide(QString slide);

signals:
    void slidesChanged(QList<QObject*> slideList);
    void selectedSlideChanged(QString favoriteColor);
private:
     QList<QObject*> mSlides;
     QString mSlideName;
     QQmlApplicationEngine mEngine;
     void resetModel();
};
#endif // MAINVIEW_H
