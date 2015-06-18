/*
 * VisibilityIO.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: TWT GmbH
 */

#include <sstream>
#include <iterator>
#include <string>
#include <assert.h>

#include <ofgen/VisibilityIO.h>

#ifndef LOGI
#define LOGI(...)
#endif

namespace ofgen
{

	VisibilityIO::VisibilityIO()
	{
	}

	VisibilityIO::~VisibilityIO()
	{
	}

	void VisibilityIO::write( const Visibility& visibility,
														std::ostream &ofsVisibility )
	{
		assert( ofsVisibility.good() );
		unsigned int nPoints = visibility.size();

		FileHeader fileHeader;
		fileHeader.version = version;
		fileHeader.nPoints = nPoints;

		ofsVisibility.write( reinterpret_cast< const char * >( &fileHeader ), sizeof(FileHeader) );

		PointHeader pointHeader;
		for (pointHeader.iPoint = 0; pointHeader.iPoint < nPoints;
				pointHeader.iPoint++)
		{
			const VisibleLabels &visible = visibility[pointHeader.iPoint];
			pointHeader.point = visible.eye;
			pointHeader.nLabels = visible.labels.size();
			ofsVisibility.write( reinterpret_cast<const char *>( &pointHeader ), sizeof(PointHeader) );
			ofsVisibility.write( reinterpret_cast<const char *>( &visible.labels[0] ),
													 sizeof(unsigned int) * pointHeader.nLabels );

			if (!ofsVisibility.good())
			{
				break;
			}
		}

		// write version again at the end for consistency checking
		if (ofsVisibility.good())
		{
			ofsVisibility.write( reinterpret_cast<const char *>( &fileHeader ), sizeof(FileHeader) );
		}
	}

	Visibility VisibilityIO::read( std::istream &ifsVisibility )
			throw (const std::runtime_error &)
	{
		assert( ifsVisibility.good() );

		FileHeader fileHeader;
		ifsVisibility.read( reinterpret_cast<char *>( &fileHeader ), sizeof(FileHeader) );

		if (!ifsVisibility.good())
		{
			throw std::runtime_error( "Error reading header" );
		}
		if (fileHeader.version < minVersion || fileHeader.version > version)
		{
			throw std::runtime_error(
					std::string( "Incompatible version: " )
							+ std::to_string( fileHeader.version )
							+ " - expected at least version "
							+ std::to_string( minVersion ) );
		}

		LOGI( "Visibility file version is %f, reading visibility for %d points",
				fileHeader.version, fileHeader.nPoints );

		Visibility visibility( fileHeader.nPoints );
		PointHeader pointHeader;

		for (unsigned int iPoint = 0; iPoint < fileHeader.nPoints; iPoint++)
		{
			if (ifsVisibility.eof())
			{
				throw std::runtime_error(
						"Reached end-of-file before all visibilities could be read" );
			}

			ifsVisibility.read( reinterpret_cast< char * >( &pointHeader ), sizeof(PointHeader) );
			if (pointHeader.iPoint != iPoint)
			{
				throw std::runtime_error(
						std::string( "Wrong eye point index: " )
								+ std::to_string( pointHeader.iPoint ) + " instead of "
								+ std::to_string( iPoint ) );
			}

			VisibleLabels &visible = visibility[iPoint];
			visible.eye = pointHeader.point;
			visible.labels.resize( pointHeader.nLabels );
			ifsVisibility.read( reinterpret_cast<char *>( &visible.labels[0] ),
													sizeof(unsigned int) * pointHeader.nLabels );
		}

		FileHeader check;
		ifsVisibility.read( reinterpret_cast<char *>( &check ), sizeof(FileHeader) );
		if (check.version != fileHeader.version
				|| check.nPoints != fileHeader.nPoints)
		{
			throw std::runtime_error( "Checksum mismatch" );
		}

		return visibility;
	}

	const float VisibilityIO::version = 1.30f;
	const float VisibilityIO::minVersion = 1.30f;

} /* namespace ofgen */
