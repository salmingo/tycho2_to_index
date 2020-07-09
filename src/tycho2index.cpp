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
#include "build_index.h"

/*!
 * @brief 由Tycho2原始星表生成用于星图匹配的索引文件
 * @param argc 命令行参量数量
 * @param argv 命令行参数
 * @return
 * 处理结果
 * @note
 * 命令行参数:
 * - 视场
 * -
 */
int main(int argc, char **argv) {
	/* 加载原始星表 */
	const char pathcat[] = "/Users/lxm/Catalogue/tycho2";
	load_catalog(pathcat);
	/* 生成索引星表 */

	return 0;
}
