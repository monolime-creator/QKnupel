#ifndef QKNUPEL_H
#define QKNUPEL_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <cmath>
#include "stylescolors.h"

// Компонент кнюпель, в виджет помещены две окружности
// 1: Основная большая статичная
// 2: Дочерняя для возможности изменения значений
// Ось X компонента направлена из центра виджета налево.
// Ось Y направлена из центра вверх.

class QKnupel : public QWidget
{
    Q_OBJECT
public:

    //Конструктор
    explicit QKnupel(QWidget *parent = nullptr);

    //Перечисление типов стилей
    enum TYPE_STYLE
    {
      Dracula = 1,
      SnowWhite   = 2
    };

    //----------------PUBLIC_FUNCTIONS-----------------
    //Функция установки текущего стиля отрисовки
    void setStyle(const TYPE_STYLE &style);
    //-------------------------------------------------

public slots:

private:

    //-------------------------------------------------------------
    //-------------------Для основной окружности-------------------
    //-------------------------------------------------------------
    //Центральная точка основной окружности
    QPointF m_centerMain;
    qreal   getCenterMainX()const{return m_centerMain.x();}
    qreal   getCenterMainY()const{return m_centerMain.y();}
    QPointF getCenterMain()const{return m_centerMain;}
    void    setCenterMain(QPointF _Point){m_centerMain = _Point;}

    //Радиус основной окружности
    qreal m_radiusMain;
    qreal getRadiusMain()const{return m_radiusMain;}
    void  setRadiusMain(qreal _Value){m_radiusMain = _Value;}

    //Старый радиус основной окружности для сравнения и сохранения масштаба при вызове ResizeEvents
    qreal OldRadius;
    qreal getOldRadius()const {return  OldRadius;}
    void  setOldRadius(qreal _Value) {OldRadius = _Value;}
    //-------------------------------------------------------------

    //-------------------------------------------------------------
    //-------------------Для малой окружности----------------------
    //-------------------------------------------------------------
    //Центральная точка малой окружности
    QPointF m_centerLittle;
    qreal   getCenterLittleX()const{return m_centerLittle.x();}
    qreal   getCenterLittleY()const{return m_centerLittle.y();}
    QPointF getCenterLittle()const{return m_centerLittle;}
    void    setCenterLittle(QPointF _Point){m_centerLittle = _Point;}

    //Радиус вектор до малой окружности относительно центра
    qreal   m_RadiusToLittle;
    qreal   getRadiusToLittle() const{return m_RadiusToLittle;}
    void    setRadiusToLittle(qreal _Value){m_RadiusToLittle = _Value;}

    //Радиус малой окружности
    qreal m_radiusLittle;
    qreal getRadiusLittle()const{return m_radiusLittle;}
    void  setRadiusLittle(qreal _Value){m_radiusLittle = _Value;}
    //--------------------------------------------------------------

    //-------------------------------------------------------------
    //------------------Для статичной окружности-------------------
    //-------------------------------------------------------------
    //Центральная точка статичной окружности
    QPointF m_centerStatic;
    qreal   getCenterStaticX()const{return m_centerStatic.x();}
    qreal   getCenterStaticY()const{return m_centerStatic.y();}
    QPointF getCenterStatic(){return m_centerStatic;}
    void    setCenterStatic(QPointF _Point){m_centerStatic = _Point;}

    //Радиус статичной окружности
    qreal m_radiusStatic;
    qreal getRadiusStatic()const{return m_radiusStatic;}
    void  setRadiusStatic(qreal _Value){m_radiusStatic = _Value;}
    //--------------------------------------------------------------

    //-------------------------------------------------------------
    //----------------------Для прямоугольника---------------------
    //-------------------------------------------------------------
    //Параметры квадрата
    QRect m_rect;
    QRect getm_rect ()const{return m_rect;}
    void  setm_rect(QRect _Rect){m_rect = _Rect;}
    void  setm_rectWidth(int _Value){m_rect.setWidth(_Value);}
    void  setm_rectHeight(int _Value){m_rect.setHeight(_Value);}
    //--------------------------------------------------------------

    //Радиус вектор
    qreal RadVec;
    qreal getRadVec(){return RadVec;}
    void  setRadVec(qreal _Value){RadVec = _Value;}

    //Угол поворота в радианах
    qreal Rad;
    qreal getRad(){return Rad;}
    void  setRad(qreal _Value){Rad = _Value;}

    //Угол поворота в градусах
    qreal Deg;
    qreal getDeg(){return Deg;}
    void  setDeg(qreal _Value){Deg = _Value;}

    //Выходное значение относительно измененного положения дочернего круга X
    qreal m_outValueX;
    qreal getm_outValueX()const{return m_outValueX;}
    void  setm_outValueX(qreal _Value){m_outValueX = _Value; emit outValueXChanged(m_outValueX);}

    //Выходное значение относительно измененного положения дочернего круга Y
    qreal m_outValueY;
    qreal getm_outValueY()const{return m_outValueY;}
    void  setm_outValueY(qreal _Value){m_outValueY = _Value; emit outValueYChanged(m_outValueY);}

    //Флаг оповещающий что вызвано нажатие мыши
    bool m_pressed;
    bool getm_pressed()const{return m_pressed;}
    void setm_pressed(bool _Value){m_pressed = _Value; emit m_pressedChanged(m_pressed);}


    //-------------------------STRUCTS---------------------------------
    //Структура гаммы цветов
    typedef struct{
      QColor SwithcerCircleleColorOff;
      QColor SwithcerCircleleColorOn;
      QColor BackGroundColor;
      QColor CentralTextColor;
      QColor TextColor;
      QColor CentralCircleColor;
      QColor MainCircleColor;
      QColor BorderColor;
    }Colors;
    //----------------------------------------------------------------

    //---------------------------COLORS--------------------------------
    //Перечисление для выбора расчёта текущего значения относительно угла поворота
    enum TYPE_CALC {First,Second};
    //---------------------------------------------------------------------

    //Компонент отрисовки
    QPainter   *m_painter;
    //Цвет объекта
    Colors     m_color;
    //Матрицы поворота осей для поворота квадрата
    QMatrix Matrix1;
    QMatrix Matrix2;
    QMatrix Matrix3;

    //----------------------------PRIVATE_FUNCTIONS----------------------------
    //Функция отрисовки окружностей
    void DrawCircle(QColor _Pen, QColor _Brush, QPointF _Center, qreal _RadiusX, qreal _RadiusY);
    //Функция поворота глобальных осей
    void MatrixRotate(qreal Deg);
    //Функция формирования выходных значений
    void OutValues(qreal CenterX, qreal CenterY, int DeltaWidth, int DeltaHeight, qreal Radius);
    //------------------------------------------------------------------------

signals:

  //Signals
  void m_pressedChanged(bool _Value);
  void outValueXChanged(qreal _Value);
  void outValueYChanged(qreal _Value);


protected:

    //Events
    void mousePressEvent        (QMouseEvent *event)  override;
    void mouseReleaseEvent      (QMouseEvent *event)  override;
    void mouseMoveEvent         (QMouseEvent *event)  override;
    void paintEvent             (QPaintEvent *event)  override;
    void resizeEvent            (QResizeEvent *event) override;
};

#endif // QKNUPEL_H
