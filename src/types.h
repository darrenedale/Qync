/**
 * @file types.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declarations of various types used throughout the application.
 */

#ifndef QYNC_TYPES_H
#define QYNC_TYPES_H

namespace Qync {
	enum class NotificationType : unsigned char {
		Information = 0,
		Warning = 1,
		Error = 2,
	};
}

#endif  // TYPES_H
