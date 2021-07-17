#include "customview.h"

CustomView::CustomView(QWidget*& widget) : QGraphicsView(widget), scene(new QGraphicsScene)
{
    vRuler = new Ruler(this, Ruler::Vertical);
    hRuler = new Ruler(this, Ruler::Horizontal);

    connect(vRuler, &Ruler::sizeChanged, [this](QSize const& size) { setViewportMargins(size.width(), RULER_SIZE, 0, 0); });
    connect(hRuler, &Ruler::sizeChanged, [this](QSize const& size) { setViewportMargins(RULER_SIZE, size.height(), 0, 0); });
}

void CustomView::Init(FontFile *fontFile)
{
    openedFile = fontFile;
    Redraw(0);
}

void CustomView::Redraw(const int selectedIndex)
{
    scene->clear();

    if(openedFile->getFontAlphaImage() != "")
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

        alphaImageItem = scene->addPixmap(QPixmap::fromImage(*alphaImage));
    }

    if(openedFile->getFontColorImage() != "")
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

        scene->addPixmap(QPixmap::fromImage(*colorImage));

    }

    QPen pen(Qt::gray, 1, Qt::DashLine, Qt::FlatCap);

    for (int i = 0; i < openedFile->CharactersCount(); i++)
    {
        FontCharacter character = openedFile->GetItem(i);

        DrawCharacterOutline(character, pen);
    }

    if(openedFile->CharactersCount() > 0 && selectedIndex >= 0 && selectedIndex < openedFile->CharactersCount())
    {
        pen.setColor(Qt::green);
        DrawCharacterOutline(openedFile->GetItem(selectedIndex), pen);
    }

    setScene(scene);
    show();
    update();
    viewport()->update();
}

void CustomView::DrawCharacterOutline(FontCharacter character, QPen pen)
{
    //Draw Top Line
    scene->addLine(character.getStartPixel(), character.getTopPixel(), character.getEndPixel(), character.getTopPixel(), pen);

    //Draw left bottom to top line
    scene->addLine(character.getStartPixel(), character.getTopPixel(), character.getStartPixel(), character.getTopPixel() + openedFile->getCharacterHeight(), pen);

    //Draw right bottom to top line
    scene->addLine(character.getEndPixel(), character.getTopPixel(), character.getEndPixel(), character.getTopPixel() + openedFile->getCharacterHeight(), pen);

    //Draw bottom line
    scene->addLine(character.getStartPixel(), character.getTopPixel()  + openedFile->getCharacterHeight() , character.getEndPixel(), character.getTopPixel()  + openedFile->getCharacterHeight(), pen);
}

void CustomView::setScene(QGraphicsScene* scene)
{
    QGraphicsView::setScene(scene);

    if(scene)
    {
        vRuler->setFixedHeight(scene->height());
        hRuler->setFixedWidth(scene->width());
    }
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
    if(alphaImageItem != nullptr && openedFile->getFontAlphaImage() != "")
    {
        vRuler->setOrigin(mapFromScene(alphaImageItem->mapToScene(alphaImageItem->offset())));
        hRuler->setOrigin(mapFromScene(alphaImageItem->mapToScene(alphaImageItem->offset())));
    }

    QGraphicsView::paintEvent(event);
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
