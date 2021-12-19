#include <ruler.h>


Ruler::Ruler(QAbstractScrollArea* parent, Ruler::RulerType rulerType): QWidget(parent), scale(1), originPixel(0), offset(5), mRulerType(rulerType)
{

    if(mRulerType == RulerType::Vertical)
    {
        setMaximumHeight(999999);
        setMaximumWidth(RULER_SIZE);
        //setFixedSize(RULER_SIZE, parent->height());
        move(0, RULER_SIZE);
        //connect(parent->verticalScrollBar(), &QScrollBar::valueChanged, this, &Ruler::setOffset);
    }
    else
    {
        setMaximumWidth(999999);
        setMaximumHeight(RULER_SIZE);
        //setFixedSize(parent->width(), RULER_SIZE);
        move(RULER_SIZE, 0);
        //connect(parent->horizontalScrollBar(), &QScrollBar::valueChanged, this, &Ruler::setOffset);
    }



}

void Ruler::paintEvent(QPaintEvent* event)
{
    setFixedSize(*customSize);

    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing);

    bool isVertical = mRulerType == RulerType::Vertical;

    QRect thisRect = this->rect();

    painter.fillRect(thisRect, QColor(150,150,150));

    if(isVertical)
    {
        painter.drawLine(RULER_SIZE-1, 0, RULER_SIZE-1, customSize->height());
    }else
    {
        painter.drawLine(0, RULER_SIZE-1, customSize->width(), RULER_SIZE-1);
    }

    if(isVertical)
    {
        painter.translate(0, -offset);
    }
    else
    {
         painter.translate(offset, 0);
    }

    painter.setFont(font());

    qreal lineHeight = isVertical ? width() : height();

    drawMeters(&painter, lineHeight, scale, 100, true, true);

    update();
}

void Ruler::drawMeters(QPainter* painter, qreal height, qreal scale, int divider, bool drawNumber, bool drawZero = false)
{
    QFontMetrics fm(font());
    int rulerSize = mRulerType == RulerType::Vertical ? customSize->height() : customSize->width();

    for(int position = 0; position < qMax((int)(rulerSize / scale), rulerSize); ++position)
    {
        int realPosition = position - originPixel - RULER_SIZE;

        if(qAbs(realPosition) % divider != 0) continue;

        qreal unscaledPosition = position * scale;

        if(realPosition == 0 && drawZero)
        {
            painter->setPen(QColor(255,0,0));
        }

        if(mRulerType == RulerType::Vertical)
        {
            painter->drawLine(customSize->width() - height, unscaledPosition, customSize->width(), unscaledPosition);
        }
        else
        {
            painter->drawLine(unscaledPosition, customSize->height() - height,unscaledPosition, customSize->height());
        }

        if(drawNumber)
        {
            if(realPosition == 0 && !drawZero) continue;

            QString const txt = QString::number(qAbs(realPosition));
            QRect txtRect = fm.boundingRect(txt);
            qreal xc = txtRect.width();
            qreal yc = txtRect.height()*0.7;

            if(mRulerType == RulerType::Vertical)
            {
                painter->translate(yc, unscaledPosition + xc*1.2);
                painter->rotate(-90);
            }
            else
            {
                painter->translate(unscaledPosition + xc/5, yc);
            }

            painter->drawText(txtRect, txt);
            painter->resetTransform();

        }

        painter->setPen(QColor(0,0,0));
    }
}

void Ruler::resizeEvent(QResizeEvent* event)
{
    emit sizeChanged(event->size());

    QWidget::resizeEvent(event);
}

void Ruler::setOffset(int value)
{
    offset = value * (mRulerType == RulerType::Vertical ? 1 : -1);
    update();
}

void Ruler::setScale(qreal value)
{
    scale *= value;
    update();
}

void Ruler::setOrigin(const QPointF value)
{
    QPointF transformedPoint = mapFromParent(value/scale);
    originPixel = (mRulerType == RulerType::Vertical ? transformedPoint.y() : transformedPoint.x());
    update();
}

void Ruler::setSize(const QSize newSize)
{
    if(mRulerType == RulerType::Vertical)
    {
        customSize = new QSize(RULER_SIZE, newSize.height());
    }else
    {
        customSize = new QSize(newSize.width(), RULER_SIZE);
    }

    setFixedSize(*customSize);
}


