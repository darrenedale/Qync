/**
 * \file functions.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.7
 *
 * \brief Qync free-standing functions implementation file.
 *
 * \dep
 * - functions.h
 * - QDebug
 * - QXmlStreamReader
 */

#include "functions.h"

#include <QDebug>
#include <QXmlStreamReader>

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
	void parseUnknownElementXml(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement());

		qDebug() << "discarding unrecognised XML element" << xml.name();

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


}  // namespace Qync
