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

///*!
// * @brief 对tycho2星表排序, 并存储为DAT文件
// * @param pathroot 根路径
// * @note
// * - 星表按照第一优先级赤纬、第二优先级赤经排序. 赤经与赤纬步长=2.5度
// * - 文件由2部分组成
// *   1: 索引
// *   2: 数据
// */
//void save_tycho2_data(const char* pathroot) {
//	// 转换数据格式
//	int n = stars.size();
//	int i, ir, id;
//	int zd = int(180.001 / 2.5);
//	int zr = int(360.001 / 2.5);
//	int m  = zd * zr;
//	double scale = 0.4 * MAS2D;
//	struct quick_index {
//		uint32_t head, count;
//	};
//	quick_index *index = new quick_index[m];
//
//	memset(index, 0, sizeof(quick_index) * m);
//	for (i = 0; i < n; ++i) {
//		id = int(stars[i].spd * scale);
//		ir = int(stars[i].ra * scale);
//		++index[id * zr + ir].count;
//	}
//	for (i = 1; i < m; ++i) index[i].head = index[i-1].head + index[i-1].count;
//	for (i = 0; i < m; ++i) printf ("%-7d  |  %4d ++ ", index[i].head, index[i].count);
//	printf ("\n");
//	// 输出星表
//	char filepath[100];
//	sprintf (filepath, "%s/tycho2.dat", pathroot);
//	FILE *fp = fopen(filepath, "wb");
//	fwrite(index, sizeof(quick_index), m, fp);
//	fwrite(stars.data(), sizeof(CatStar), n, fp);
//	fclose(fp);
//	// 释放资源
//	delete []index;
//}
//
///*!
// * @brief 由Tycho2原始星表生成用于星图匹配的索引文件
// * @param argc 命令行参量数量
// * @param argv 命令行参数
// * @return
// * 处理结果
// * @note
// * 命令行参数:
// * - 视场
// * - 最暗星等
// */
//int main(int argc, char **argv) {
//	load_catalog(".");
//	printf ("load_catalog() over\n");
//	sort_catalog();
//	printf ("sort_catalog() over\n");
//	save_tycho2_data(".");
//	printf ("save_tycho2() over\n");
//	return 0;
//}


/**
 * @brief
 * 由tycho2星表生成星表索引
 */
int main(int argc, char** argv) {
	return 0;
}
