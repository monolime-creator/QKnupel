#include "knupel.h"
#include <QMatrix>
QKnupel::QKnupel(QWidget *parent) : QWidget(parent)
{
    //Сброс флага в default нажатия мыши
    setm_pressed(false);
    //Установка текущего стиля для компонента
    setStyle(TYPE_STYLE::Dracula);
    //Установка геометрии компонента
    setGeometry(500,500,400,400);
    //Ограничение по минимальной размерности
    setMinimumSize(150,150);

    //Установка текущего BackGrounda
    setStyleSheet("* {background-color: " + QString(m_color.BackGroundColor.name()) + "}");

    //Запоминание точки центра компонента для малого, основного круга и статичного
    setCenterMain(QPointF(width()/2,height()/2));
    setCenterLittle(QPointF(width()/2,height()/2));
    setCenterStatic(QPointF(width()/2,height()/2));

    //Установка радиуса окружностей в зависимости от размера виджета
    setRadiusMain(std::min(this->width()/2,this->height()/2)*0.8);
    setRadiusLittle(std::min(this->width()/10,this->height()/10));
    setRadiusStatic(std::min(this->width()/15,this->height()/15));

    //Радиус вектор до малого круга по дефолту = 0
    setRadiusToLittle(0);

    //Запоминание старого радиуса для ухода от ошибок в начале работы компонента
    setOldRadius(getRadiusMain());

}

void QKnupel::DrawCircle(QColor _Pen, QColor _Brush, QPointF _Center, qreal _RadiusX, qreal _RadiusY)
{
    //Установка цвета отрисовки
    m_painter->setPen(QColor(_Pen));
    m_painter->setBrush(QBrush(QColor(_Brush),Qt::SolidPattern));
    //Отрисовка окружности
    m_painter->drawEllipse(_Center,_RadiusX,_RadiusY);
}

void QKnupel::MatrixRotate(qreal _Deg)
{
    //Поворот осей для разворота внутреннего квадрата
    Matrix1.translate(-getCenterMainX(),-getCenterMainY());
    Matrix2.rotate(_Deg);
    Matrix3.translate(getCenterMainX(),getCenterMainY());
    //Установка глобальной матрицы с измененными координатами
    m_painter->setWorldMatrix(Matrix1*Matrix2*Matrix3);
}

void QKnupel::setStyle(const QKnupel::TYPE_STYLE &style){

    //Белый тип стиля
    if(style == TYPE_STYLE::SnowWhite)
    {
        m_color.TextColor                         = Styles::Light::DarkGreen;
        m_color.BackGroundColor             = Styles::Light::LightGray;
        m_color.MainCircleColor                = Styles::Light::White;
        m_color.CentralTextColor              = Styles::Light::Cyan;
        m_color.CentralCircleColor            = Styles::Light::LightGray;
        m_color.SwithcerCircleleColorOff  = Styles::Light::Orange;
        m_color.SwithcerCircleleColorOn   =Styles::Light::Cyan;
        m_color.BorderColor                      = Styles::Light::Cyan;
    }
    //Тёмный тип стиля
    else
    {
        m_color.TextColor                         = Styles::Dracula::GrayWhite;
        m_color.BackGroundColor             = Styles:: Dracula::Asphalt;
        m_color.MainCircleColor                = Styles::Dracula::DarkGray;
        m_color.CentralTextColor              = Styles::Dracula::Green;
        m_color.CentralCircleColor            = Styles::Dracula::Asphalt;
        m_color.SwithcerCircleleColorOff  = Styles::Dracula::Red;
        m_color.SwithcerCircleleColorOn   = Styles::Dracula::Green;
        m_color.BorderColor                      = Styles::Dracula::Green;
    }
    //Установка цвета заднего фона
    setStyleSheet("* {background-color: " + QString(m_color.BackGroundColor.name()) + "}");
}

void QKnupel::OutValues(qreal CenterX, qreal CenterY, int DeltaWidth, int DeltaHeight, qreal Radius)
{
    //Запись выходных значений
    setm_outValueX((CenterX - DeltaWidth) / Radius);
    setm_outValueY(-(CenterY - DeltaHeight) / Radius);
    qDebug()<<"X: "<<getm_outValueX()<<"Y: "<<getm_outValueY();
}

void QKnupel::mousePressEvent(QMouseEvent *event)
{
    //Проверка на нажатие правой или левой кнопки мыши
    if(event->button() == Qt::RightButton || event->button() == Qt::LeftButton)
    {
        setm_pressed(true);
        QKnupel::repaint();
    }
}

void QKnupel::mouseReleaseEvent(QMouseEvent *event)
{
    setm_pressed(false);
    if(event->button() == Qt::LeftButton)
    {
      //Возврат малого круга в центр
      setCenterLittle(getCenterMain());
      //Возврат квадрата в дефолтный размер
      setm_rectWidth(0);
    }
    else
      //Расчёт радиуса вектора до центра малого круга
      setRadiusToLittle(sqrt((getCenterLittleX()-getCenterMainX())*(getCenterLittleX()-getCenterMainX()) + (getCenterLittleY()-getCenterMainY()) * (getCenterLittleY()-getCenterMainY())));

    QKnupel::repaint();
}

void QKnupel::mouseMoveEvent(QMouseEvent *event)
{
    if(getm_pressed())
    {
        //Расчёт расположения осей
        qreal AxisX  =  (event->x() - getCenterMainX());
        qreal AxisY  = -(event->y() - getCenterMainY());

        //Расчёт угла
        setRad(std::atan(AxisY/AxisX));
        setDeg(Rad*180/M_PI);

        //Уход от нулевых значений
        if(AxisX < 0.01 && AxisX > -0.01)
            AxisX = 0.01;
        if(AxisY < 0.01 && AxisY > - 0.01)
            AxisY = 0.01;

        //Корректирование по текущему диапазону
        if(AxisX < 0 && AxisY > 0)
            setDeg(getDeg()+180);
        else if(AxisX < 0 && AxisY < 0)
            setDeg(getDeg()+180);
        else if(AxisX > 0 && AxisY < 0)
            setDeg(getDeg()+360);

        //Расчёт радиуса вектора для ограничения движения по окружности
        setRadVec(std::sqrt(std::pow(AxisX,2) + std::pow(AxisY,2)));

        //Если расчитанный радиус вектор вышел за границы главной окружности
        if(getRadVec() > getRadiusMain())
        {
          //Ограничиваем его по окружности
          setRadVec(getRadiusMain());
        }
        //Формирование выходных значений
        OutValues(getCenterLittleX(),getCenterLittleY(),width()/2,height()/2,getRadiusMain());
        //Установка ширины квадрата по радиусу вектора
        setm_rectWidth(static_cast<int>(getRadVec()));

        //Установка центра малой окружности
        setCenterLittle(QPointF(getCenterMainX() + std::cos(getDeg()*M_PI/180)*getRadVec(),getCenterMainY() - std::sin(getDeg()*M_PI/180)*getRadVec()));  //Движение малого круга по положению мыши

        //Запоминание старого радиуса после выставления точки
        setOldRadius(getRadiusMain());

        QKnupel::repaint();
    }
}

void QKnupel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter paint(this);
    m_painter = &paint;

    // Расчёт центра окружности для дальнейших действий в полярных координатах
    m_painter->setRenderHint(QPainter::Antialiasing, true);

    //Сброс предыдущих значений координат
    Matrix1.reset();
    Matrix2.reset();
    Matrix3.reset();

    //Отрисовка основной окружности
    DrawCircle(m_color.MainCircleColor,m_color.MainCircleColor,getCenterMain(),getRadiusMain(),getRadiusMain());

    //Поворот осей относительно рассчитанного угла
    MatrixRotate(-getDeg());

    //Установка цвета отрисовки для прямоугольника
    m_painter->setPen(QColor(m_color.BorderColor));
    m_painter->setBrush(QBrush(QColor(m_color.BackGroundColor)));
    //Отрисовка квадрата в центре
    m_painter->drawRect(static_cast<int>(getCenterMainX() - getRadiusStatic()/2),static_cast<int>(getCenterMainY() - getRadiusStatic()/2),getm_rect().width(),static_cast<int>(getRadiusStatic()));

    //Отрисовка статичной окружности
    DrawCircle(m_color.BorderColor,m_color.BackGroundColor,getCenterStatic(),getRadiusStatic(),getRadiusStatic());

    //Сброс в дефолт предыдущих измененных координат
    Matrix1.reset();
    Matrix2.reset();
    Matrix3.reset();

    //Поворот осей относительно рассчитанного угла
    MatrixRotate(0);

    //Отрисовка малой окружности
    DrawCircle(m_color.TextColor,getm_pressed() ? m_color.SwithcerCircleleColorOn : m_color.SwithcerCircleleColorOff,getCenterLittle(),getRadiusLittle(),getRadiusLittle());

}

void QKnupel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    //Ограничение по минимальной размерности
    setMinimumSize(150,150);
    //Запоминание точки центра компонента
    setCenterMain(QPointF(width()/2,height()/2));
    setCenterStatic(QPointF(width()/2,height()/2));

    //Установка радиуса окружностей в зависимости от размера виджета
    setRadiusMain(std::min(this->width()/2,this->height()/2)*0.8);
    setRadiusLittle(std::min(this->width()/10,this->height()/10));
    setRadiusStatic(std::min(this->width()/15,this->height()/15));

    //Расчёт отношения текущего радиуса и измененного
    auto Delta = getRadiusMain()/getOldRadius();

    //Расчёт положения и фиксация малой окружности на текущем положении
    setCenterLittle(QPointF(getCenterMainX()+(getRadiusToLittle()*Delta)*cos(getDeg()*M_PI/180), getCenterMainY()-(getRadiusToLittle()*Delta)*sin(getDeg()*M_PI/180)));

    //Установка длины малого квадрата по рассчитанному радиусу вектору
    setm_rectWidth(static_cast<int>(getRadiusToLittle() * Delta));

}
