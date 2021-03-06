/**
 * @file build_index.cpp 由tycho2星表生成星图匹配索引
 */
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include "ADefine.h"
#include "build_index.h"

using namespace std;
using namespace AstroUtil;
using namespace boost::algorithm;

CatStarVec stars;

bool resolve_cat(const string &line, CatStar &star) {
	string ra_str, dc_str, pmra_str, pmdc_str, bt_str, vt_str;

	if (line[13] == ' ') {
		ra_str   = line.substr(15, 12); trim(ra_str);
		dc_str   = line.substr(28, 12); trim(dc_str);
	}
	else {
		ra_str   = line.substr(152, 12); trim(ra_str);
		dc_str   = line.substr(165, 12); trim(dc_str);
	}
	pmra_str = line.substr(41, 7);  trim(pmra_str);
	pmdc_str = line.substr(49, 7);  trim(pmdc_str);
	bt_str   = line.substr(110, 6); trim(bt_str);
	vt_str   = line.substr(123, 6); trim(vt_str);
	star.ra  = int(stod(ra_str) * D2MAS);
	star.spd = int((stod(dc_str) + 90.0) * D2MAS);
	if (pmra_str.size()) star.pmra = short(stod(pmra_str));
	if (pmdc_str.size()) star.pmdc = short(stod(pmdc_str));
	if (bt_str.size() || vt_str.size()) {
		if (bt_str.size() && vt_str.size()) {
			double vt = stod(vt_str);
			star.mag = short((vt - 0.09 * (stod(bt_str) - vt)) * 1000.0);
		}
		else {
			star.mag = short((bt_str.size() ? stod(bt_str) : stod(vt_str)) * 1000.0);
		}
		return true;
	}
	return false;
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

	star.ra  = int(stod(ra_str) * D2MAS);
	star.spd = int((stod(dc_str) + 90.0) * D2MAS);
	if (pmra_str.size()) star.pmra = short(stod(pmra_str));
	if (pmdc_str.size()) star.pmdc = short(stod(pmdc_str));
	if (bt_str.size() || vt_str.size()) {
		if (bt_str.size() && vt_str.size()) {
			double vt = stod(vt_str);
			star.mag = short((vt - 0.09 * (stod(bt_str) - vt)) * 1000.0);
		}
		else {
			star.mag = short((bt_str.size() ? stod(bt_str) : stod(vt_str)) * 1000.0);
		}
		return true;
	}
	return false;
}

void to_J2000(ATimeSpace& ats, CatStar& star) {
	double ra, dc;
	double t = 2000.0 - ats.Epoch();
	ra = (star.ra + star.pmra * t / cos((star.spd * MAS2D - 90.0) * D2R)) * MAS2D;
	dc = (star.spd + star.pmdc * t) * MAS2D - 90.0;
	ats.EqReTransfer(ra * D2R, dc * D2R, ra, dc);
	star.ra  = int(ra * R2D * D2MAS);
	star.spd = int((dc * R2D + 90.0) * D2MAS);
}

void load_catalog(const char *pathroot) {
	const int szline(220);
	char filepath[50], line[szline];

	for (int i = 0; i < 20; ++i) {
		sprintf (filepath, "%s/tyc2.dat.%02d", pathroot, i);
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
		sprintf (filepath, "%s/suppl_%d.dat", pathroot, i);
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
}

void sort_catalog() {
	sort(stars.begin(), stars.end(), [](CatStar& x1, CatStar& x2) {
		double scale = 0.4 * MAS2D;
		int id1 = int(x1.spd * scale);
		int id2 = int(x2.spd * scale);
		int ir1 = int(x1.ra * scale);
		int ir2 = int(x2.ra * scale);
		return (id1 < id2 || (id1 == id2 && ir1 < ir2));
	});
}
