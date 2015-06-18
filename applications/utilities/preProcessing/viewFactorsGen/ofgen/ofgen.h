/*
 * ofgen.h
 *
 *  Created on: Mar 11, 2015
 *      Author: TWT GmbH
 */

#ifndef OFGEN_H_
#define OFGEN_H_

#define GLM_PRECISION_LOWP_FLOAT
//#define GLM_PRECISION_HIGHP_FLOAT

#include <vector>

namespace ofgen
{
#ifdef GLM_PRECISION_LOWP_FLOAT
	typedef float real;
#else
	typedef double real;
#endif

	struct Point
	{
		ofgen::real coords[3];
	};

	typedef int Label;

	typedef std::vector<ofgen::Label> Labels;

} /* namespace ofgen */

#endif /* OFGEN_H_ */
