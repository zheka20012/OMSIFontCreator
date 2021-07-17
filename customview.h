#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QEnterEvent>
#include <QMatrix4x4>
#include <FontFile.h>
#include <QGraphicsPixmapItem>
#include <ruler.h>
#include <QGridLayout>

class CustomView : public QGraphicsView
{
private:
    QImage* alphaImage;
    QImage* colorImage;

    bool requsetedRedraw;

    Ruler* vRuler;
    Ruler* hRuler;

    FontFile *openedFile;

    QGraphicsScene *scene;

    QGraphicsPixmapItem *alphaImageItem;

    void drawCharacterOutline(QPainter *painter, FontCharacter character, QPen pen, qreal scaleX, qreal scaleY);

    int selectedChar;

public:    
    CustomView();
    CustomView(QWidget*& widget);

    void Init(FontFile *fontFile);

    void Redraw(const int selectedIndex);

    QImage* getAlphaImage() {return alphaImage;}
    void setAlphaImage(QImage *value) {alphaImage = value; requsetedRedraw = true;}

    QImage* getColorImage() {return colorImage;}
    void setColorImage(QImage *value) {colorImage = value; requsetedRedraw = true;}

    void setScene(QGraphicsScene *scene);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void enterEvent(QEnterEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // CUSTOMVIEW_H
