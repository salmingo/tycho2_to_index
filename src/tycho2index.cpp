/**
 Name        : tycho2_to_index.cpp
 Author      : Xiaomeng Lu
 Version     : 0.1
 Date        : 2020年7月3日
 Description : 参照Astrometry.net中Shape-Index算法, 由Tycho2星表生成索引文件
 - 从星表中提取J2000坐标和自行
 - 遍历以参考星为中心, 建立索引
 - 以BINTABLE格式, 在FITS文件中存储索引、坐标和自行
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "build_index.h"
#include "FITSHandler.hpp"
#include "ADefine.h"
using namespace AstroUtil;

/*!
 * @brief 对tycho2星表排序, 并存储为DAT文件
 * @param pathroot 根路径
 * @note
 * - 星表按照第一优先级赤纬、第二优先级赤经排序. 赤经与赤纬步长=2.5度
 * - 文件由2部分组成
 *   1: 索引
 *   2: 数据
 */
void save_tycho2(const char* pathroot) {
	char filepath[100];
	HFITS hfit;

	sprintf (filepath, "%s/tycho2.fit", pathroot);
	remove(filepath);

	if (!hfit(filepath, 2)) {
		printf ("failed to create file: %s\n", filepath);
		return ;
	}
	// 转换数据格式
	int n = stars.size();
	int i, ir, id;
	int zd = int(180.001 / 2.5);
	int zr = int(360.001 / 2.5);
	int m  = zd * zr;
	uint32_t* head  = new uint32_t[m];
	uint32_t* count = new uint32_t[n];
	uint32_t* ra    = new uint32_t[n];
	uint32_t* spd   = new uint32_t[n];
	int16_t*  mag   = new int16_t[n];
	double spd;

	memset(count, 0, sizeof(uint32_t) * m);
	for (i = 0; i < n; ++i) {
		id = int((spd = stars[i].dc + 90.0) / 2.5);
		ir = int(stars[i].ra / 2.5);
		++count[id * 144 + ir];
		ra[i]  = uint32_t(stars[i].ra * 3600000.0);
		dc[i]  = uint32_t(spd * 3600000.0);
		mag[i] = int16_t(stars[i].mag * 1000.0);
	}
	head[0] = 1;
	for (i = 1; i < m; ++i) head[i] = head[i - 1] + count[i - 1];

	// HDU 1
	fits_create_img(hfit(), BYTE_IMG, 0, NULL, hfit.Status());
	fits_write_comment(hfit(),
			"simplified binary TYCHO2 catalog, just stores RA/DEC and Magnitude, expected to be used for celestial fix. "
			"including all-sky stars from tycho2 and its' supplementary catalog. "
			"RA is stored as 32-bit unsigned integer in milli arcsecs. "
			"DEC is transformed to distance from SOUTH Pole and then stored as 32-bit unsigned ingeter in milli arcsecs. "
			"Magnitude refers to V band from BT and VT, is stored as 16-bit integer in milli mag.",
			hfit.Status());
	fits_write_comment(hfit(),
			"RA is stored as 32-bit unsigned integer in milli arcsecs. ",
			hfit.Status());
	fits_write_comment(hfit(),
			"DEC is transformed to distance from SOUTH Pole and then stored as 32-bit unsigned ingeter in milli arcsecs. ",
			hfit.Status());
	fits_write_comment(hfit(),
			"Magnitude refers to V band from BT and VT, is stored as 16-bit integer in milli mag.",
			hfit.Status());
	// HDU 2
	char *ttype2[] = {"head index", "count"};
	char *tform2[] = {"1J", "1J"};
	fits_create_tbl(hfit(), BINARY_TBL, m, 2, ttype2, tform2, NULL, NULL, hfit.Status());
	fits_write_comment(hfit(),
			"quick index for sky quads. quads are divided by step 2.5 degrees on both RA and DEC. "
			"quads count is 72 * 144 = 10368. first order is DEC and second is RA. ",
			hfit.Status());
	fits_write_col(hfit(), TUINT, 1, 1, 1, m, head,  hfit.Status());
	fits_write_col(hfit(), TUINT, 2, 1, 1, m, count, hfit.Status());

	// HDU 3
	char *ttype3[] = {"RA", "DEC", "Mag"};
	char *tform3[] = {"1J", "1J", "1I"};
	char *tunit3[] = {"mas", "mas", "millimag"};
	fits_create_tbl(hfit(), BINARY_TBL, n, 3, ttype3, tform3, tunit3, NULL, hfit.Status());
	fits_write_col(hfit(), TUINT,  1, 1, 1, n, ra,  hfit.Status());
	fits_write_col(hfit(), TUINT,  2, 1, 1, n, dc,  hfit.Status());
	fits_write_col(hfit(), TSHORT, 3, 1, 1, n, mag, hfit.Status());

	// 处理结果
	if (hfit.Success()) printf ("catalog tycho2 is saved to [%s]\n", filepath);
	else printf ("save_tycho2() failed: %s\n", hfit.GetError());

	// 释放资源
	delete []head;
	delete []count;
	delete []ra;
	delete []dc;
	delete []mag;
}

/*!
 * @brief 由Tycho2原始星表生成用于星图匹配的索引文件
 * @param argc 命令行参量数量
 * @param argv 命令行参数
 * @return
 * 处理结果
 * @note
 * 命令行参数:
 * - 视场
 * - 最暗星等
 */
int main(int argc, char **argv) {
	double fov = atof(argv[1]);

	return 0;
}
