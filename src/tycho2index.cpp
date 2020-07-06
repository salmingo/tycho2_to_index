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

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <boost/algorithm/string/trim.hpp>
#include "FITSHandler.hpp"
#include "ADefine.h"
#include "ATimeSpace.h"

using namespace std;
using namespace boost::algorithm;
using namespace AstroUtil;

struct CatStar {
	double ra, dc;		// J2000坐标, 量纲: 角度
	float pmra, pmdc;	// 自行, 量纲: 角秒/年
	float mag;			// 星等
};
typedef vector<CatStar> CatStarVec;

/*!
 * @brief 解析Tycho2中tyc2.dat.xx
 * @param line 一行数据
 * @return
 * 解析结果
 */
bool resolve_cat(const string &line, CatStar &star) {
	if (line[13] != ' ') return false;
	string ra_str   = line.substr(15, 12); trim(ra_str);
	string dc_str   = line.substr(28, 12); trim(dc_str);
	string pmra_str = line.substr(41, 7);  trim(pmra_str);
	string pmdc_str = line.substr(49, 7);  trim(pmdc_str);
	string bt_str   = line.substr(110, 6); trim(bt_str);
	string vt_str   = line.substr(123, 6); trim(vt_str);

	star.ra = stod(ra_str);
	star.dc = stod(dc_str);
	star.pmra = pmra_str.empty() ? 0.0 : stod(pmra_str) * 0.001;
	star.pmdc = pmdc_str.empty() ? 0.0 : stod(pmdc_str) * 0.001;
	if (bt_str.size() || vt_str.size()) {
		if (bt_str.size() && vt_str.size()) {
			double vt = stod(vt_str);
			star.mag = vt - 0.09 * (stod(bt_str) - vt);
		}
		else {
			star.mag = bt_str.size() ? stod(bt_str) : stod(vt_str);
		}
	}
	else return false;
	return true;
}

/*!
 * @brief 解析Tycho2的补充星表
 * @param line 一行数据
 * @return
 * 解析结果
 * @note
 * - 将坐标转换至J2000
 */
bool resolve_suppl(const string &line, CatStar &star) {
	string ra_str   = line.substr(15, 12); trim(ra_str);
	string dc_str   = line.substr(28, 12); trim(dc_str);
	string pmra_str = line.substr(41, 7);  trim(pmra_str);
	string pmdc_str = line.substr(49, 7);  trim(pmdc_str);
	string bt_str   = line.substr(83, 6);  trim(bt_str);
	string vt_str   = line.substr(96, 6);  trim(vt_str);

	star.ra = stod(ra_str);
	star.dc = stod(dc_str);
	star.pmra = pmra_str.empty() ? 0.0 : stod(pmra_str) * 0.001;
	star.pmdc = pmdc_str.empty() ? 0.0 : stod(pmdc_str) * 0.001;
	if (bt_str.size() || vt_str.size()) {
		if (bt_str.size() && vt_str.size()) {
			double vt = stod(vt_str);
			star.mag = vt - 0.09 * (stod(bt_str) - vt);
		}
		else {
			star.mag = bt_str.size() ? stod(bt_str) : stod(vt_str);
		}
	}
	else return false;
	return true;
}

void to_J2000(ATimeSpace& ats, CatStar& star) {
	double ra1, dc1, ra2, dc2;
	double t = ats.Epoch() - 2000.0;
	ra1 = star.ra + star.pmra * t * AS2D / cos(star.dc * D2R);
	dc1 = star.dc + star.pmdc * t * AS2D;
	ats.EqReTransfer(ra1 * D2R, dc1 * D2R, ra2, dc2);
	star.ra = ra2 * R2D;
	star.dc = dc2 * R2D;
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
 * -
 */
int main(int argc, char **argv) {
	/* 加载原始星表 */
	const char pathcat[] = "/Users/lxm/Catalogue/tycho2";
	const int szline(220);
	char filepath[50], line[szline];
	CatStarVec stars;

	for (int i = 0; i < 20; ++i) {
		sprintf (filepath, "%s/tyc2.dat.%02d", pathcat, i);
		printf ("%s\n", filepath);

		FILE *fp = fopen(filepath, "r");
		while (!feof(fp)) {
			if (NULL == fgets(line, szline, fp)) continue;

			CatStar star;
			if (resolve_cat(line, star)) stars.push_back(star);
		}
		fclose(fp);
	}

	ATimeSpace ats;
	ats.SetEpoch(1991.25);

	for (int i = 1; i <= 2; ++i) {
		sprintf (filepath, "%s/suppl_%d.dat", pathcat, i);
		printf ("%s\n", filepath);

		FILE *fp = fopen(filepath, "r");
		while (!feof(fp)) {
			if (NULL == fgets(line, szline, fp)) continue;

			CatStar star;
			if (resolve_suppl(line, star)) {
				to_J2000(ats, star);
				stars.push_back(star);
			}
		}
		fclose(fp);
	}
	printf ("%lu stars are found\n", stars.size());

	/* 生成索引星表 */

	return 0;
}
