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

    QPixmap finalImage;

    Ruler* vRuler;
    Ruler* hRuler;

    FontFile *openedFile;

    QGraphicsScene *scene;

    QGraphicsPixmapItem *finalImageItem;

    void drawCharacterOutline(QPainter *painter, FontCharacter character, QPen pen, qreal scaleX, qreal scaleY);

    int selectedChar;

    bool shouldDrawAlpha = true;
    bool shouldDrawColor;
    bool shouldDrawFinal;

public:    
    CustomView();
    CustomView(QWidget*& widget);

    void Init(FontFile *fontFile);

    void Redraw(const int selectedIndex);
    void ForceRedraw();

    void setDrawAlpha(bool value) {shouldDrawAlpha = value; shouldDrawColor = !shouldDrawAlpha; Redraw(selectedChar);}
    bool getDrawAlpha() {return shouldDrawAlpha;}

    void setDrawColor(bool value) {shouldDrawColor = value; shouldDrawAlpha = !shouldDrawColor; Redraw(selectedChar);}
    bool getDrawColor() {return shouldDrawColor;}

    void setDrawFinal(bool value) {shouldDrawFinal = value; Redraw(selectedChar);}
    bool getDrawFinal() {return shouldDrawFinal;}

    QImage* getAlphaImage() {return alphaImage;}

    QImage* getColorImage() {return colorImage;}

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
