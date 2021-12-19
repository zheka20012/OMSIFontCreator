#include "customview.h"

CustomView::CustomView(QWidget*& widget) : QGraphicsView(widget), scene(new QGraphicsScene)
{
    vRuler = new Ruler(this, Ruler::Vertical);
    hRuler = new Ruler(this, Ruler::Horizontal);

    connect(vRuler, &Ruler::sizeChanged, [this](QSize const& size) { setViewportMargins(size.width(), RULER_SIZE, 0, 0); });
    connect(hRuler, &Ruler::sizeChanged, [this](QSize const& size) { setViewportMargins(RULER_SIZE, size.height(), 0, 0); });

    setBackgroundBrush(QBrush(QImage(":/img/images/checker.png")));
    setRenderHints(QPainter::LosslessImageRendering);
}

void CustomView::Init(FontFile *fontFile)
{
    openedFile = fontFile;
    Redraw(0);
}

void CustomView::Redraw(const int selectedIndex)
{
    scene->clear();

    if((shouldDrawColor || shouldDrawFinal) && openedFile->getFontColorImage() != "")
    {
        QFileInfo info(openedFile->getFontColorImage());

        QImage *colorImage;

        if(info.isAbsolute())
        {
            colorImage = new QImage(openedFile->getFontAlphaImage());
        }
        else
        {
            colorImage = new QImage(openedFile->getFilePath() + "/" + openedFile->getFontColorImage());
        }


        finalImage = QPixmap::fromImage(*colorImage);
    }

    if(openedFile->getFontAlphaImage() != "" && (shouldDrawAlpha || shouldDrawFinal))
    {
        QFileInfo info(openedFile->getFontAlphaImage());

        QImage *alphaImage;

        if(info.isAbsolute())
        {
            alphaImage = new QImage(openedFile->getFontAlphaImage());
        }
        else
        {
            alphaImage = new QImage(openedFile->getFilePath() + "/" + openedFile->getFontAlphaImage());
        }

        if(!shouldDrawFinal)
        {
            if(shouldDrawAlpha)
            {
                finalImage = QPixmap::fromImage(*alphaImage);
            }
        }
        else
        {
            alphaImage->invertPixels();
            QBitmap bitmap = QBitmap::fromImage(alphaImage->scaled(finalImage.width(), finalImage.height()), Qt::AutoColor);
            finalImage.setMask(bitmap);
        }

        scene->setSceneRect(alphaImage->rect());
    }

    finalImageItem = scene->addPixmap(finalImage);

    selectedChar = selectedIndex;
    setScene(scene);
    show();
    update();
    viewport()->update();
}

void CustomView::ForceRedraw(){
    Redraw(selectedChar);
}

void CustomView::drawCharacterOutline(QPainter *painter, FontCharacter character, QPen pen, qreal scaleX, qreal scaleY)
{
    painter->setPen(pen);

    //Draw Top Line
    painter->drawLine(character.getStartPixel() * scaleX, character.getTopPixel() * scaleY, character.getEndPixel() * scaleX, character.getTopPixel() * scaleY);

    //Draw left bottom to top line
    painter->drawLine(character.getStartPixel() * scaleX, character.getTopPixel() * scaleY, character.getStartPixel() * scaleX, (character.getTopPixel() + openedFile->getCharacterHeight()) * scaleY);

    //Draw right bottom to top line
    painter->drawLine(character.getEndPixel() * scaleX, character.getTopPixel() * scaleY, character.getEndPixel() * scaleX, (character.getTopPixel() + openedFile->getCharacterHeight()) * scaleY);

    //Draw bottom line
    painter->drawLine(character.getStartPixel() * scaleX, (character.getTopPixel()  + openedFile->getCharacterHeight()) * scaleY , character.getEndPixel() * scaleX , (character.getTopPixel()  + openedFile->getCharacterHeight()) * scaleY);
}

void CustomView::setScene(QGraphicsScene* scene)
{
    QGraphicsView::setScene(scene);
}

void CustomView::wheelEvent(QWheelEvent *e)
{
    const ViewportAnchor anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = e->angleDelta().y();
    qreal factor;
    if (angle > 0) {
        factor = 1.1;
    } else {
        factor = 0.9;
    }
    vRuler->setScale(factor);
    hRuler->setScale(factor);
    scale(factor, factor);
    setTransformationAnchor(anchor);
}

void CustomView::paintEvent(QPaintEvent *event)
{
    if(finalImageItem != nullptr)
    {
        vRuler->setOrigin(mapFromScene(finalImageItem->mapToScene(finalImageItem->offset())));
        hRuler->setOrigin(mapFromScene(finalImageItem->mapToScene(finalImageItem->offset())));
    }

    QGraphicsView::paintEvent(event);

    QPainter *painter = new QPainter(viewport());

    QTransform t = viewportTransform();

    qreal scaleX = t.m11();
    qreal scaleY = t.m22();

    painter->setTransform(QTransform(1, t.m12(), t.m13(),t.m21(), 1, t.m23(), t.m31(),t.m32(), t.m33()));

    QPen pen(Qt::gray, 2, Qt::DashLine, Qt::FlatCap);

    for (int i = 0; i < openedFile->CharactersCount(); i++)
    {
        FontCharacter character = openedFile->GetItem(i);

        drawCharacterOutline(painter, character, pen, scaleX, scaleY);
    }

    if(openedFile->CharactersCount() > 0 && selectedChar >= 0 && selectedChar < openedFile->CharactersCount())
    {
        pen.setColor(Qt::green);
        drawCharacterOutline(painter, openedFile->GetItem(selectedChar), pen, scaleX, scaleY);
    }

    delete painter;
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    if(event->button() == Qt::MiddleButton){
        viewport()->setCursor(Qt::ClosedHandCursor);
        setDragMode(QGraphicsView::ScrollHandDrag);

        QMouseEvent* pressEvent = new QMouseEvent(QEvent::GraphicsSceneMousePress,
                                            event->pos(), Qt::MouseButton::LeftButton,
                                            Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);

        this->mousePressEvent(pressEvent);
    }

}

void CustomView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void CustomView::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
}

void CustomView::resizeEvent(QResizeEvent *event)
{
    vRuler->setSize(event->size());
    hRuler->setSize(event->size());
}

void CustomView::enterEvent(QEnterEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
}

void CustomView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    setDragMode(QGraphicsView::NoDrag);
    viewport()->setCursor(Qt::CrossCursor);
}
