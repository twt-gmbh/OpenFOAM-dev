/*
 * VisibilityIO.h
 *
 *  Created on: Apr 13, 2015
 *      Author: TWT GmbH
 */

#ifndef INCLUDE_OFGEN_VISIBILITYIO_H_
#define INCLUDE_OFGEN_VISIBILITYIO_H_

#include <ostream>
#include <istream>
#include <stdexcept>
#include <vector>

#include <ofgen/ofgen.h>

namespace ofgen
{

	/**
	 * Visible labels (usually some kind of face id) for an eye point
	 */
	struct VisibleLabels
	{
		Point eye;
		std::vector<unsigned int> labels;
	};

	/**
	 * All visible labels of the scene
	 */
	typedef std::vector<VisibleLabels> Visibility;

	/**
	 * Helper to read and write visibilities
	 */
	class VisibilityIO
	{
	public:
		VisibilityIO();
		virtual ~VisibilityIO();

		/**
		 * Write to binary (!) output stream
		 * @param
		 * @param
		 */
		static void write( const Visibility &, std::ostream & );

		/**
		 * Read from binary (!) input stream
		 * @param
		 * @return
		 */
		static Visibility read( std::istream & ) throw (const std::runtime_error &);

	protected:
		/**
		 * Beginning of file
		 */
		struct FileHeader
		{
			FileHeader() :
					magic
						{ "ofgenvis 1.3\0\0\0" }, version( 0 ), nPoints( 0 )
			{
			}
			const char magic[16];
			float version;
			unsigned int nPoints;
		};

		/**
		 * Beginning of each eye point
		 */
		struct PointHeader
		{
			PointHeader() :
					iPoint( 0 ), nLabels( 0 )
			{
			}
			/**
			 * Only for consistency checking
			 */
			unsigned int iPoint;
			/**
			 * Only for consistency checking
			 */
			Point point;
			/**
			 * The actual data
			 */
			unsigned int nLabels;
		};

	private:
		/**
		 * Version to be included in file headers
		 */
		static const float version;
		/**
		 * Minimum backwards compatible version
		 */
		static const float minVersion;
	};

} /* namespace ofgen */

#endif /* INCLUDE_OFGEN_VISIBILITYIO_H_ */
