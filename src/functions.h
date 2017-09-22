/**
 * \file functions.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.7
 *
 * \brief Defines free-standing functions that are available throughout
 * the application.
 *
 * All functions are defined in the Qync namespace.
 *
 * \dep
 * - QXmlStreamReader
 */

#ifndef QYNC_FUNCTIONS_H
#define QYNC_FUNCTIONS_H

class QXmlStreamReader;

namespace Qync {
	void parseUnknownElementXml(QXmlStreamReader & xml);
};

#endif  // QYNC_FUNCTIONS_H
