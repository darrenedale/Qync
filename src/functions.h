/**
 * @file functions.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Defines free-standing functions that are available throughout
 * the application.
 *
 * All functions are defined in the Qync namespace.
 */

#ifndef QYNC_FUNCTIONS_H
#define QYNC_FUNCTIONS_H

#include <optional>

class QXmlStreamReader;
class QString;

namespace Qync {
	void parseUnknownElementXml(QXmlStreamReader &);
    std::optional<bool> parseBooleanText(const QString &);
};

#endif  // QYNC_FUNCTIONS_H
