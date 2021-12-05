#include "slidepage.h"

SlidePage::SlidePage(int radius, QString name, QWidget *parent) :
    QWidget(parent),
    cornerRadius(radius),
    pageName(name)
{
    if(parent)
        resize(parent->width() * 0.8 <= preferWidth ? parent->width() * 0.8 : preferWidth, parent->height());
    this->move(QPoint(-this->width() - 30, 0));

    pageContentContainer = new ScrollAreaCustom(this);
    //> note: Important!!!
    pageContentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    nameLabel = new QLabel(pageName, this);
    textFont.setStyleStrategy(QFont::PreferAntialias);
    nameLabel->setFont(textFont);

    backIcon = new customIcon(":/icons/icons/back.svg", "", 5, this);

    opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(0);
    pageContentContainer->setGraphicsEffect(opacity);
    nameLabel->setGraphicsEffect(opacity);

    QString style;
    style = "background-color:white;border-radius:" + QString::asprintf("%d", cornerRadius) + "px";
    QWidget *backGround = new QWidget(this);
    backGround->lower();
    backGround->resize(this->size());
    backGround->setStyleSheet(style);
    backGround->show();

    /* Intialize layout */
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 40, 20, 20);
    QWidget *titleBar = new QWidget(this);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setAlignment(Qt::AlignLeft);
    titleBar->setLayout(titleLayout);
    titleLayout->addWidget(backIcon);
    titleLayout->addWidget(nameLabel);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(pageContentContainer);
    mainLayout->setAlignment(Qt::AlignTop);
    this->setLayout(mainLayout);

    /* Set shadow */
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0));
    shadow->setOffset(0, 0);
    this->setGraphicsEffect(shadow);

    /* connect */
    connect(backIcon, &QPushButton::clicked, this, [=](){slideOut();});
}

void SlidePage::SetRadius(int radius){
    cornerRadius = radius;
    QString style;
    style = "background-color:white;border-radius:" + QString::asprintf("%d", cornerRadius) + "px";
    this->setStyleSheet(style);
}

void SlidePage::SetName(QString name){
    pageName = name;
    nameLabel->setText(pageName);
}

void SlidePage::slideIn(){
    if(curAni){
        curAni->stop();
        curAni->deleteLater();
        curAni = nullptr;
    }
    QParallelAnimationGroup *inGroup = new QParallelAnimationGroup(this);
    QPropertyAnimation *slideInAni = new QPropertyAnimation(this, "pos", this);
    slideInAni->setStartValue(this->pos());
    slideInAni->setEndValue(QPoint(0, 0));
    slideInAni->setDuration(1000);
    slideInAni->setEasingCurve(QEasingCurve::InOutExpo);
    QPropertyAnimation *fadeInAni = new QPropertyAnimation(opacity, "opacity", this);
    fadeInAni->setStartValue(opacity->opacity());
    //> note: DO NOT CHANGE 0.99 TO 1!!!!!
    //>       Will cause unexpected position shift (maybe qt's bug)
    fadeInAni->setEndValue(0.99);
    fadeInAni->setDuration(750);
    QSequentialAnimationGroup *rotate = new QSequentialAnimationGroup(this);
    QPropertyAnimation *rotateAni = new QPropertyAnimation(backIcon, "rotationAngle", this);
    rotateAni->setStartValue(180);
    rotateAni->setEndValue(360);
    rotateAni->setDuration(750);
    rotateAni->setEasingCurve(QEasingCurve::InOutExpo);
    rotate->addPause(250);
    rotate->addAnimation(rotateAni);
    inGroup->addAnimation(slideInAni);
    inGroup->addAnimation(fadeInAni);
    inGroup->addAnimation(rotate);
    connect(inGroup, &QParallelAnimationGroup::finished, this, [=](){this->curAni = nullptr;});
    inGroup->start();
    curAni = inGroup;
}

void SlidePage::slideOut(){
    if(curAni){
        curAni->stop();
        curAni->deleteLater();
        curAni = nullptr;
    }
    QParallelAnimationGroup *outGroup = new QParallelAnimationGroup(this);
    QPropertyAnimation *slideOutAni = new QPropertyAnimation(this, "pos", this);
    slideOutAni->setStartValue(this->pos());
    slideOutAni->setEndValue(QPoint(-this->width() - 30, 0));
    slideOutAni->setDuration(1000);
    slideOutAni->setEasingCurve(QEasingCurve::InOutExpo);
    QPropertyAnimation *fadeOutAni = new QPropertyAnimation(opacity, "opacity", this);
    fadeOutAni->setStartValue(opacity->opacity());
    fadeOutAni->setEndValue(0);
    fadeOutAni->setDuration(750);
    QPropertyAnimation *rotateAni = new QPropertyAnimation(backIcon, "rotationAngle", this);
    rotateAni->setStartValue(360);
    rotateAni->setEndValue(180);
    rotateAni->setDuration(750);
    rotateAni->setEasingCurve(QEasingCurve::InOutExpo);
    outGroup->addAnimation(slideOutAni);
    outGroup->addAnimation(fadeOutAni);
    outGroup->addAnimation(rotateAni);
    connect(outGroup, &QPropertyAnimation::finished, this, [=](){this->curAni = nullptr;});
    outGroup->start();
    curAni = outGroup;
}
