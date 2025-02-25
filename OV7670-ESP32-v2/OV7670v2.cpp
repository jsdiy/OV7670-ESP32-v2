//OV7670クラスにおける解像度設定に関する不具合と対策
//『昼夜逆転』工作室	http://jsdiy.starfree.jp
//jsdiy	https://github.com/jsdiy/
//	2024/04 - 2025/02	@jsdiy
#include <Arduino.h>
#include "OV7670v2.hpp"

/*	OV7670クラスにおける解像度設定に関する不具合と対策
状態：
	init()の中でreset()が呼ばれている。
	reset()の中でsetResolution()が呼ばれ、その中でsetPCLK()が呼ばれている。
	reset()の最後でもsetPCLK()が呼ばれている。(*)
不具合：
	・(*)のsetPCLK()により、setResolution()の設定が不適切に更新されている。
	・setResolution()の中で呼ばれているsetPCLK(pre,pll)の引数pllが適切ではない。
		→pllをDBLV_CLK_x4としているが、画像が乱れるのでDBLV_BYPASSとしたい。
解決策：
	・init()完了後、適切なsetPCLK(pre,pll)を実行する。
		→preはsetResolution()から持ってくる。pllはDBLV_BYPASSとする。
*/
//修正版関数
esp_err_t	OV7670v2::init(const camera_config_t *value, uint8_t res, uint8_t colmode)
{
	esp_err_t retCode = OV7670::init(value, res, colmode);

	uint8_t pclkdiv = 1;
	if (res == QQVGA || res == QQCIF || res == QCIF) { pclkdiv = 1; }
	if (res == QVGA || res == CIF) { pclkdiv = 2; }
	if (res == VGA) { pclkdiv = 4; }
	setPCLK(pclkdiv, DBLV_BYPASS);

	return retCode;
}

/*	setBright(),getBright()の値について
レジスタにセットする値は0x00-0xFFで、デフォルト値は0x00.
ただし、レジスタ値と明るさはリニアに対応していない。下記のようになっている。
	レジスタ値：	暗← 0xFF...0x80,0x00...0x7F →明
そこで、設定値と明るさがリニアに対応するよう、レジスタ値と設定値を相互変換する。
	設定値：		暗← 0x00...0x7F,0x80...0xFF →明

ところで、基底クラスのsetBright(),getBright()はレジスタ値を符号あり整数で扱っているが、
レジスタ値は単なるbyte値と考え、正と見るか負と見るかはアプリ側の解釈によるものとしたい。
よって、OV7670v2クラスではsetBright()の引数およびgetBright()の戻り値を符号なし整数とした関数を定義する。
*/
//取得値と明るさがリニアに対応（暗:0x00...0xFF:明）
uint8_t	OV7670v2::getBright(void)
{
	int8_t ival = OV7670::getBright();
	auto uval = static_cast<uint8_t>(ival);
	uval = (0x00 <= uval && uval <= 0x7F) ? (0x80 + uval) : (0xFF - uval);
	return uval;
}

//設定値と明るさがリニアに対応（暗:0x00...0xFF:明）
void	OV7670v2::setBright(uint8_t val)
{
	uint8_t& uval = val;	//読み替え
	uval = (0x00 <= uval && uval <= 0x7F) ? (0xFF - uval) : (uval - 0x80);
	auto ival = static_cast<int8_t>(uval);
	OV7670::setBright(ival);
}
