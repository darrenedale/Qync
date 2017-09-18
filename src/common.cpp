/**
 * \file common.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Qync common functions implementation file.
 *
 * \dep
 * - common.h
 *
 * \todo
 * Nothing.
 */

#include "common.h"

#include <QDebug>


void Qync::parseUnknownElementXml( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement());

	qDebug() << "discarding unrecognised XML element" << xml.name();

	while(!xml.atEnd()) {
		xml.readNext();
		if(xml.isEndElement()) break;
		if(xml.isCharacters()) continue;
		if(xml.isStartElement()) parseUnknownElementXml(xml);
	}
}
