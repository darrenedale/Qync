/**
 * @file functions.cpp
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Qync free-standing functions implementation file.
 */

#include "functions.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>

/**
 * @namespace Qync
 *
 * @brief Namespace for functions and types for the Qync application.
 *
 * The Qync namespace is reserved for all functions and typedefs available to
 * the Qync application and its classes. Its main purpose is to avoid pollution
 * of the global namespace.
 */
namespace Qync {

	/**
	 * @brief Completely parses an XML element.
	 *
	 * @param xml is the stream from which to parse the XML element.
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
	void parseUnknownElementXml(QXmlStreamReader & xml) {
		Q_ASSERT_X(xml.isStartElement(), __PRETTY_FUNCTION__, "stream is not at the start of an element");

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement()) {
				break;
			}

			if(xml.isCharacters()) {
				continue;
			}

			if(xml.isStartElement()) {
				parseUnknownElementXml(xml);
			}
		}
	}

    /**
     * @brief Convert the text representation of a boolean value to an
     * actual @b bool.
     *
     * @param boolString is the boolean text to convert.
     *
     * This is a helper method to parse the content of a boolean text from
     * the preferences file. A valid @b bool is always returned - callers
     * must provide an @b ok parameter and check its content when the method
     * returns if they need to check whether the returned value represents
     * the text provided or is just a default value because the text was not
     * valid.
     *
     * @return An optional boolean. If set, it is set to the boolean value
     * parsed from the string; if not set the provided string could not be
     * parsed.
     */
    std::optional<bool> parseBooleanText(const QString & boolString)
    {
        if(0 == QString::compare("true", boolString.trimmed(), Qt::CaseInsensitive)) {
            return true;
        }
        else if(0 == QString::compare("false", boolString.trimmed(), Qt::CaseInsensitive)) {
            return false;
        }

        return {};
    }
}  // namespace Qync
