#include<slide.h>
#include<mainview.h>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>
#include <QFileInfo>
#include <QDateTime>
#include <imageprovider.h>
#include <QQuickStyle>

MainView::MainView(QQuickView *parent): QQuickView(parent)
{

}

bool MainView::initialize()
{
    resetModel();
    QQuickStyle::setStyle("Universal");
    mEngine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (mEngine.rootObjects().isEmpty())
        return false;

    return true;
}

void MainView::resetModel()
{
    mEngine.addImageProvider(QLatin1String("colors"), new ImageProvider());
    mEngine.rootContext()->setContextProperty("Wrapper",this);
}

QList<QObject *> MainView::slides() const
{
    return mSlides;
}

void MainView::setSlides(QList<QUrl> fileNames)
{
    if (fileNames.size()==0)
        return;
    std::vector<Slide> slides;

    for(QUrl qStrFilePath : fileNames)
    {

        QString file = qStrFilePath.toLocalFile();
        QImage image(file);
        QFileInfo fi(file);
        QString base = fi.baseName();
        QDateTime created = fi.lastModified();
        Slide *s= new Slide(base, fi.filePath(), created.toString("dd-MM-yyyy"));
        mSlides.append(s);
        addSlides(mSlides);
        emit slidesChanged(mSlides);
    }
}


void MainView::clearSlides()
{
    if(!mSlides.empty())
    {
        mSlides.clear();
        emit slidesChanged(mSlides);
    }
}

void MainView::deleteSlide(int index)
{
    if(index >= mSlides.size())
        return;
    mSlides.removeAt(index);
    emit slidesChanged(mSlides);
}

void MainView::setSelected(int index)
{
    if(index >= mSlides.size())
        return;
    Slide * slide = static_cast<Slide *> (mSlides.at(index));
    setSelctedSlide(slide->filePath());
}

QList<QObject *> MainView::slideList() const
{
    return mSlides;
}

void MainView::addSlides(QList<QObject *> slideList)
{
    if (mSlides == slideList)
        return;
    mSlides = slideList;
    emit slidesChanged(mSlides);
}

MainView::~MainView()
{
    if(!mSlides.empty())
    {
        mSlides.clear();
    }
}

QString MainView::selectedSlide() const
{
    return mSlideName;
}

void MainView::setSelctedSlide(QString slide)
{
    if (mSlideName == slide)
        return;
    mSlideName = slide;
    emit selectedSlideChanged(mSlideName);
}






