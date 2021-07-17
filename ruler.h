#ifndef RULER_H
#define RULER_H
#define RULER_SIZE 20

#include <QWidget>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QResizeEvent>
#include <QFontMetrics>
#include <QGraphicsView>
#include <QColor>
#include <QSizePolicy>

class Ruler : public QWidget
{
    Q_OBJECT
public:
    enum RulerType { Horizontal, Vertical };

    Ruler(QAbstractScrollArea* parent=nullptr, Ruler::RulerType rulerType=RulerType::Vertical);
    void setOffset(int value);
    void setOrigin(const QPointF value);

    void setScale(qreal value);

    void setSize(const QSize newSize);

protected:
   virtual void paintEvent(QPaintEvent *event);
   virtual void resizeEvent(QResizeEvent *event);



private:
    int offset;
    qreal scale;
    int originPixel;
    RulerType mRulerType;

    QSize* customSize;

    void zoomEvent(qreal scale);

    // изменение точки отсчета
    void mouseMoveOrigin(QPoint origin);

    // изменение позиций маркеров курсора
    void mouseMoveCoords(QPoint origin, QPoint global, qreal scale);

    void rulerClickedH(QPoint);
    void rulerClickedV(QPoint);
    void moveSelection(qreal,qreal);

signals:
    void sizeChanged(QSize const&);

};

#endif // RULER_H
