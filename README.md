# OV7670-ESP32-v2
PlatformIO＋ESP32＋ArduinoFrameworkでOV7670(non-FIFO)を動かすライブラリ、の補足
概要
[kobatanさんのOV7670-ESP32プロジェクト](https://github.com/kobatan/OV7670-ESP32)について、
OV7670::init()の不具合を修正するため、派生クラスOV7670v2を作成しました。  
libフォルダに下記のように配置してください。使い方はサンプルのmain.cppを参考にしてください。
```
|--lib
|  |
|  |--OV7670-ESP32  ←kobatanさんのライブラリ
|  |  |- I2Scamera.c  ★
|  |  |- I2Scamera.h
|  |  |- OV7670.cpp  ★
|  |  |- OV7670.h
|  |
|  |--OV7670-ESP32-v2  ←jsdiyのライブラリ
|  |  |- OV7670v2.cpp
|  |  |- OV7670v2.hpp
|  |  |--examples
|  |     |- main.cpp
|  |
```
# 推奨
★印のファイルについて、警告を解消するため下記の変更を推奨します。
## I2Scamera.c
- #include "esp32-hal.h" を追加。
  - 警告[implicit declaration of function 'millis']への対応のため。
## OV7670.cpp
- rdReg()内、  
	Wire.requestFrom(OV7670_ADDR, 1, true); を、  
	Wire.requestFrom(OV7670_ADDR, 1, 1); とする。
 - 曖昧なオーバーロードを解消するため。
- ほか、必要であれば全てのSerial.printf()をコメントアウト。
