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

class MainView : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> slideList READ slideList WRITE addSlides NOTIFY slidesChanged)

public:
    explicit MainView(QObject *parent = nullptr);
    bool initialize();
    Q_INVOKABLE QList<QObject *> slides() const;
    Q_INVOKABLE void setSlides(QList<QUrl> fileNames);
    Q_INVOKABLE void clearSlides();
    Q_INVOKABLE void deleteSlide(int index);
    QList<QObject*> slideList() const;
     void addSlides(QList<QObject*> slideList);
    ~MainView();

signals:
    void slidesChanged(QList<QObject*> slideList);

private:
     QList<QObject*> mSlides;
     QQmlApplicationEngine mEngine;
     void resetModel();
};
#endif // MAINVIEW_H
