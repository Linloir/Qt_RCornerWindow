#include "customWidgets.h"

customIcon::customIcon(QString iconPath, QString hint, int r, QWidget *parent):
    QPushButton(parent),
    radius(r),
    iconHint(hint){
    QSvgRenderer renderer;
    renderer.load(iconPath);
    QSize size = renderer.defaultSize();
    iconImg = new QPixmap(size);
    iconImg->fill(Qt::transparent);
    QPainter painter(iconImg);
    painter.setRenderHints(QPainter::Antialiasing);
    renderer.render(&painter);

    widgetRatio = iconImg->height() / iconImg->width();
    bgColor = defaultColor;
}

customIcon::customIcon(const QPixmap &icon, QString hint, int r, QWidget *parent):
    QPushButton(parent),
    radius(r),
    iconHint(hint){
    iconImg = new QPixmap(icon);

    widgetRatio = iconImg->height() / iconImg->width();
    bgColor = defaultColor;
}

void customIcon::paintEvent(QPaintEvent *event){
    resize(height() / widgetRatio, height());

    QPainter bgPainter(this);
    bgPainter.setRenderHints(QPainter::Antialiasing);
    bgPainter.setPen(Qt::NoPen);
    bgPainter.setBrush(bgColor);
    bgPainter.drawRoundedRect(this->rect(), radius, radius);

    QPainter pixmapPainter(this);
    pixmapPainter.setRenderHints(QPainter::Antialiasing);
    pixmapPainter.translate(width() / 2, height() / 2);
    pixmapPainter.rotate(rotation);
    pixmapPainter.translate(-width() / 2, -height() / 2);
    int w = iconSizeRate * width();
    int h = iconSizeRate * height();
    pixmapPainter.drawPixmap(width() / 2 - w / 2, height() / 2 - h / 2, w, h, *iconImg);
}

void customIcon::enterEvent(QEnterEvent *event){
    bgColor = hoverColor;
    update();
}

void customIcon::leaveEvent(QEvent *event){
    bgColor = defaultColor;
    update();
}

void customIcon::mousePressEvent(QMouseEvent *event){
    emit clicked();
    iconSizeRate -= 0.1;
    update();
}

void customIcon::mouseReleaseEvent(QMouseEvent *event){
    iconSizeRate += 0.1;
    update();
}
