//『昼夜逆転』工作室	http://jsdiy.starfree.jp
//https://github.com/jsdiy/

#include <Arduino.h>
#include "OV7670v2.hpp"

OV7670v2	camera;

void	setup(void)
{
	camera.init();
}

void	loop(void)
{
	uint8_t brVal = camera.getBright();
	brVal += 16;
	camera.setBright(brVal);

	//camera.getFrame(buf);
	//...
}
