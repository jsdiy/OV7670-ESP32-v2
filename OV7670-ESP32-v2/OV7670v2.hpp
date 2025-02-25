//OV7670クラスにおける解像度設定に関する不具合と対策
//『昼夜逆転』工作室	http://jsdiy.starfree.jp
//jsdiy	https://github.com/jsdiy/
//	2024/04 - 2025/02	@jsdiy
#pragma	once

#include <Arduino.h>
#include "OV7670.h"

class	OV7670v2	: public	OV7670
{
public:
	OV7670v2(void) {}

	//基底クラスの同名関数を隠蔽
	esp_err_t	init(const camera_config_t *value, uint8_t res, uint8_t colmode);
	uint8_t	getBright(void);
	void	setBright(uint8_t val);
};
