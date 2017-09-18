/**
 * \file common.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Defines some macros, types and functions that are available throughout
 * the application.
 *
 * All common functions are defined in the Qync namespace.
 *
 * \dep
 * - QXmlStreamReader
 *
 * \todo
 * Nothing.
 */

#ifndef COMMON_H
#define COMMON_H

#include <QXmlStreamReader>

//#define QYNC_DEBUG

#define QYNC_APP_NAME "Qync"
#define QYNC_VERSION_STRING "0.9.5"
#define QYNC_VERSION_DATE "13th December, 2013"

#define QYNC_URL "http://www.equituk.net/"

/**
 * \namespace Qync
 *
 * \brief Namespace for functions and types for the Qync application.
 *
 * The Qync namespace is reserved for all functions and typedefs available to
 * the Qync application and its classes. Its main purpose is to avoid pollution
 * of the global namespace.
 */
namespace Qync {

	/**
	 * \brief Completely parses an XML element.
	 *
	 * \param xml is the stream from which to parse the XML element.
	 *
	 * Given an XML stream positioned on a start element, this function will
	 * read and discard the entire content of the element, leaving the stream
	 * positioned at the end of the element.
	 *
	 * This function expects the stream to be positioned at the start of the element
	 * to discard. If it is not positioned at the start of an element, the
	 * application will fail.
	 *
	 * This is a convenience function for XML parsers to use to discard elements
	 * they don't recognise without each class's XML stream parser having to
	 * reimplement its own means of ignoring such elements.
	 */
	void parseUnknownElementXml( QXmlStreamReader & xml );
};

#endif // COMMON_H
