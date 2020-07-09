/**
 * @file build_index.h 由tycho2星表生成星图匹配索引
 */

#ifndef BUILD_INDEX_H_
#define BUILD_INDEX_H_

#include <string>
#include <vector>
#include "ATimeSpace.h"

struct CatStar {
	double ra, dc;		// J2000坐标, 量纲: 角度
	float mag;			// 星等
};
typedef std::vector<CatStar> CatStarVec;
extern CatStarVec stars;

/*!
 * @brief 解析Tycho2中tyc2.dat.xx
 * @param line 一行数据
 * @return
 * 解析结果
 */
bool resolve_cat(const std::string &line, CatStar &star);
/*!
 * @brief 解析Tycho2的补充星表
 * @param line 一行数据
 * @return
 * 解析结果
 * @note
 * - 将坐标转换至J2000
 */
bool resolve_suppl(const std::string &line, CatStar &star);
/*!
 * @brief 赤道坐标历元转换: J1991.25=>J2000
 * @param ats   算法接口
 * @param star  星数据
 */
void to_J2000(AstroUtil::ATimeSpace& ats, CatStar& star);
/*!
 * @brief 加载原始星表
 * @param pathroot  根路径
 */
void load_catalog(const char *pathroot);

#endif /* BUILD_INDEX_H_ */
