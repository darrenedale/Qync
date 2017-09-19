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

#ifndef QYNC_COMMON_H
#define QYNC_COMMON_H

class QXmlStreamReader;

namespace Qync {
	void parseUnknownElementXml(QXmlStreamReader & xml);
};

#endif  // QYNC_COMMON_H
