#ifndef SLIDEPAGE_H
#define SLIDEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "customScrollContainer.h"
#include "customWidgets.h"

class SlidePage : public QWidget
{
    Q_OBJECT
private:
    int preferWidth = 350;
    int cornerRadius;
    QString pageName;
    ScrollAreaCustom *pageContentContainer;
    QLabel *nameLabel;
    customIcon *backIcon;
    QFont textFont = QFont("Corbel Light", 24);

    QParallelAnimationGroup *curAni = nullptr;
    QGraphicsOpacityEffect *opacity;

public:
    explicit SlidePage(int radius, QString name, QWidget *parent = nullptr);
    void SetRadius(int radius);
    void SetName(QString name);
    //void Resize(int w, int h);
    //void Resize(const QSize &size){Resize(size.width(), size.height());}
    void AddContent(QWidget* widget){pageContentContainer->addWidget(widget, false);}

signals:

public slots:
    void slideIn();
    void slideOut();

};

#endif // SLIDEPAGE_H
