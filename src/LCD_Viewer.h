#ifndef __LCD_VIEWER_H__
#define __LCD_VIEWER_H__

enum Color_Txt {
    LCD_TEXT_COLOR_RED,
    LCD_TEXT_COLOR_BLUE,
    LCD_TEXT_COLOR_WHITE
};

void LCD_Viewer_setup(void);
void LCD_Viewer_drawString(String inStr, Color_Txt color);
void LCD_Viewer_drawRSSString(void);

#endif /* __LCD_VIEWER_H__ */
