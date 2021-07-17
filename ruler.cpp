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

    int rulerSize = isVertical ? customSize->height() : customSize->width();

    QFontMetrics fm(font());

    for (int position = 0; position < qMax((int)(rulerSize / scale), rulerSize); ++position)
    {
        int unscaledPosition = int(position * scale);

        int fixedPosition = position - originPixel - RULER_SIZE;

        if(fixedPosition % 10 == 0)
        {
            int lineHeight = fixedPosition % 100 == 0 ? (isVertical ? width() : height()) : fixedPosition % 50 == 0 ? 8 : 5;

            if(fixedPosition == 0)
            {
                painter.setPen(QColor(255,0,0));
            }

            if(isVertical)
            {
                painter.drawLine(customSize->width() - lineHeight, unscaledPosition, customSize->width(), unscaledPosition);
            }
            else
            {
                painter.drawLine(unscaledPosition, customSize->height() - lineHeight,unscaledPosition, customSize->height());
            }
        }

        int scaleDivider = scale > 9 ? 10 : scale > 3 ? 20 : scale > 1 ? 50 : 100;


        if (fixedPosition % scaleDivider == 0)
        {
            QString const txt = QString::number(qAbs(fixedPosition));
            QRect txtRect = fm.boundingRect(txt);
            qreal xc = txtRect.width();
            qreal yc = txtRect.height()*0.7;

            if(isVertical)
            {
                painter.translate(yc, unscaledPosition + xc*1.2);
                painter.rotate(-90);
            }
            else
            {
                painter.translate(unscaledPosition + xc/5, yc);
            }

            painter.drawText(txtRect, txt);
            painter.resetTransform();
        }

        painter.setPen(QColor(0,0,0));
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


