/*
接线说明:
(单片机为esp32s3,tft屏幕的驱动芯片为ST7735S)

sck(SCL) =Pin(18)
mosi（SDA）=Pin(17)
dc = Pin(6)
cs = Pin(14)
rst（RES）= Pin(9)
BL--不接(控制屏幕背光的,可以不接)

程序说明:
本例能够显示中文


注意事项:
需要在TFT_eSPI 中的User_Setup.h 文件中修改一下:
1:根据驱动芯片取消对应的注释(#define ST7735_DRIVER      // Define additional parameters below for this display)
2:根据芯片引脚设置对应的接线同时`取消对应的注释
(// For ESP32 Dev board (only tested with GC9A01 display)
// The hardware SPI can be mapped to any pins

#define TFT_MOSI 17 // In some display driver board, it might be written as "SDA" and so on.
#define TFT_SCLK 18
#define TFT_CS  14  // Chip select control pin
#define TFT_DC   6  // Data Command control pin
#define TFT_RST  9  // Reset pin (could connect to Arduino RESET pin)
//#define TFT_BL   22  // LED back-light

//#define TOUCH_CS 21     // Chip select pin (T_CS) of touch screen

//#define TFT_WR 22    // Write strobe for modified Raspberry Pi TFT only
)



显示中文:
一:借助已有的字库进行显示
首先引用库文件 #include "zh_front_20.h" (将该库文件放置在TFT_eSPI文件夹中即可)
Setup()函数中再加载字体即可正常使用 tft.loadFont(font_20); font_20是那个.h文件中的数组名称
(该方法优点是简单,缺点是只能使用一些文字,不能自定义文字)

二:通过一系列操作实现自定义字库
1:找到一种字体(以ttf为后缀的文件)(可以在该网站下载 https://www.fonts.net.cn )(以下操作均以 SanJiShouJinTiJian-2.ttf 该字体为示例)
2:将该字体文件放置在 TFT_eSPI\Tools\Create_Smooth_Font\Create_font\data\SanJiShouJinTiJian-2.ttf   (SanJiShouJinTiJian-2.ttf为该字体的名称,将该字体放置在data文件夹中)
同时将 Create_font.pde 该文件放置于 TFT_eSPI\Tools\Create_Smooth_Font\Create_font\Create_font.pde  (即将该文件放置在Create_font文件夹中)
3:使用processing打开Create_font.pde文件进行配置:
(1)根据采用的字体文件修改这里   >>>   String fontName = "对应字体名称(例如:SanJiShouJinTiJian-2)"
(2)之后再在 static final int[] specificUnicodes = {}; 数组中添加对应中文的Unicode编码
(3)如何得到该编码呢?:
打开该网址: https://tool.chinaz.com/tools/unicode.aspx 即可找到对应的功能
转化得到的编码为  例如:\u5b9e\u65f6\u6e29\u5ea6\u6e7f\u5ea6\u7a0b\u5e8f\u8fd0\u884c\u5e02\u573a\u4f5c\u8005\u7075\u9996
再将这些编码粘贴到 static final int[] specificUnicodes = {}; 这个数组中去
static final int[] specificUnicodes = 
{
  0x5b9e,0x65f6,0x6e29,0x5ea6,0x6e7f,0x5ea6,0x7a0b,0x5e8f,
  0x8fd0,0x884c,0x5e02,0x573a,0x4f5c,0x8005,0x7075,0x9996,
};
此时需要对这些数据去掉\u加上0x
(4)上述操作完成后运行processing会得到   SanJiShouJinTiJian-220.vlw  这样一个以vlw为后缀的文件,复制一下,稍后会用
(5) 打开该网站 : https://tomeko.net/online_tools/file_to_hex.php?lang=zh 将SanJiShouJinTiJian-220.vlw进行转换,会得到数组,将数组复制粘贴记事本中等待使用
(6) 在vscode中对应的项目中打开include这个文件夹,在该文件夹下创建一个文件(名称随意(我取的是font_shoujiti.h),以.h结尾)并在该文件中写入

#include <pgmspace.h>
#include <stdint.h>
#ifndef _font_shoujiti_h_
#define _font_shoujiti_h_

const uint8_t font_shoujiti[]  PROGMEM =
{
  .....
  .....
  .....
  (填充刚才的那段数组即可)
};
#endif

这样一段程序,此时将刚才放在记事本中的数组内容放在程序中

(7)应用示例:
void setup(void) 
{
  //常用的API函数  tft.init();//初始化函数
  tft.setRotation(0);//屏幕反转位置,0~3(4是镜像,要自己添加相关代码)
  tft.fillScreen(TFT_WHITE);//颜色填充,相当于清屏  
  tft.setTextColor(TFT_GREEN,16);//设置字体颜色,前一个参数为字体颜色,后一个参数为背景颜色,可以多次出现实现不同颜色文字的出现
  tft.loadFont(font_shoujiti);//加载字体
  tft.drawString("作者",20,30);//显示中文字符

}

作者:灵首

时间:2023_3_8

*/



#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "font_shoujiti.h"
#include "zh_front_20.h"


TFT_eSPI tft = TFT_eSPI(128,160);  // 调用库，在User Setup.h中定义引脚


void setup(void) 
{
  //常用的API函数
  tft.init();//初始化函数
  tft.setRotation(0);//屏幕反转位置,0~3(4是镜像,要自己添加相关代码)
  tft.fillScreen(TFT_WHITE);//颜色填充,相当于清屏  
  tft.setTextColor(TFT_GREEN,16);//设置字体颜色,前一个参数为字体颜色,后一个参数为背景颜色,可以多次出现实现不同颜色文字的出现
  //tft.fillCircle(64,64,61,TFT_BLUE);//绘制一个圆面
  //tft.drawLine(20,20,40,40,TFT_BLACK);//绘制一条线
  //tft.drawCentreString("LINGSOU",64,120,4);//显示字符串
  tft.loadFont(font_shoujiti);//加载字体
  //tft.drawRightString("LINGSOU",64,120,4);//显示字符串
  //tft.drawString("lingsou",30,60);//显示字符串
  tft.drawString("作者",20,30);//显示中文字符

}

void loop()
{

}
