#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainterPath>
#include <QRegion>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [=](){Init();});
    t->setSingleShot(true);
    t->start(10);
}

void MainWindow::Init(){
    /* Create main widget and set mask, style sheet and shadow */
    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);

    QString mainStyle;
    ui->mainWidget->setObjectName("mainWidget");
    mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx", cornerRadius) + "}";
    ui->mainWidget->setStyleSheet(mainStyle);

    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    ui->mainWidget->setGraphicsEffect(windowShadow);
    /**********************************************************/

    /* Create border in order to cover the zigzag edge of the region */
    QWidget *border = new QWidget(this);
    border->move(ui->mainWidget->pos() - QPoint(1, 1));
    border->resize(ui->mainWidget->size() + QSize(2, 2));
    QString borderStyle;
    borderStyle = "background-color:#00FFFFFF;border:1.5px solid #686868; border-radius:" + QString::asprintf("%d",cornerRadius) + "px";
    border->setStyleSheet(borderStyle);
    border->setAttribute(Qt::WA_TransparentForMouseEvents);
    border->show();
    /*****************************************************************/

    SlidePage *page = new SlidePage(cornerRadius, "SETTINGS", ui->mainWidget);//test
    page->show();

    //ui->showBtn->setParent(this);
    //ui->hideBtn->setParent(this);
    //ui->showBtn->raise();
    //ui->hideBtn->raise();
    //ui->showBtn->move(500, 100);
    //ui->hideBtn->move(500, 150);
    //ui->showBtn->show();
    //ui->hideBtn->show();
    connect(ui->showBtn, SIGNAL(clicked()), page, SLOT(slideIn()));
    connect(ui->hideBtn, SIGNAL(clicked()), page, SLOT(slideOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
