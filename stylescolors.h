#ifndef STYLESCOLORS_H
#define STYLESCOLORS_H
#include <QColor>
struct Styles
{
  struct Dracula{
    enum{
      GrayWhite = 0xc5c9ca, // 197, 201,202
      Red       = 0xa52a2a, // 165, 42, 42
      Green     = 0x23a96e, // 35, 169,110
      Asphalt   = 0x3e4649, // 44, 48, 49
      DarkGray  = 0x2c3031, // 62, 70, 73
      Black     = 0x000000,  //  0,  0,  0
      LightCyan    = 0x01abaa // 110, 172,221
    };
  };
  struct Light{
    //Перечисление для установки цвета отрисовки в светлом стиле
    enum{
      White     = 0xffffff, // 255, 255, 255
      Cyan      = 0x01abaa, //   1, 171, 170
      Orange    = 0xec8215, // 236, 130, 21
      LightGray = 0xdedede, // 222, 222, 222
      DarkGreen = 0x011b1a, //   1,  27,  26
      Gray      = 0x9b9aa2  // 155, 154, 162
    };
  };
};


#endif // STYLESCOLORS_H
