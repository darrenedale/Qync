/**
 * \file QyncPreferences.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the QyncPreferences class.
 *
 * \dep
 * - QyncPreferences.h
 * - common.h
 * - QDir
 * - QDebug
 * - QFileInfo
 * - QProcess
 *
 * \todo
 * Nothing.
 */

#include "QyncPreferences.h"
#include "common.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>


QyncPreferences::QyncPreferences( const QString & fileName )
:	m_fileName(fileName) {
	setDefaults();
	load();
}


void QyncPreferences::setDefaults( void ) {
#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX) || defined(Q_OS_DARWIN)
	/* Q_OS_SOLARIS too? */
	QProcess p;
	p.start("which rsync");
	p.waitForFinished();
	QString myPath(p.readAllStandardOutput().trimmed());
	setRsyncPath(myPath);
#elif defined(Q_OS_WIN32)
	/* rsync binary is provided with qync */
	setRsyncPath(QDir::currentPath() + "/bin/rsync.exe");
#else
	qDebug() << "unsupported platform - default rsync binary unknown";
	setRsyncPath(QString());
#endif
}


bool QyncPreferences::parseBooleanText( const QString & b, bool * ok ) {
	if("true" == b.trimmed().toLower()) {
		if(ok) *ok = true;
		return true;
	}
	else if("false" == b.trimmed().toLower()) {
		if(ok) *ok = true;
		return false;
	}

	if(ok) *ok = false;
	return false;
}


bool QyncPreferences::parseXmlStream( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement() && "qyncpreferences" == xml.name());
	setDefaults();

	while(!xml.atEnd()) {
		xml.readNext();

		if(xml.isEndElement())
			break;

		if(xml.isCharacters()) {
			if(!xml.isWhitespace())
				qDebug() << "QyncPreferences::parseXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

			/* ignore extraneous characters */
			continue;
		}

		parseXmlElement(xml);
	}

	return true;
}


bool QyncPreferences::parseXmlElement( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement());

	if("corepreferences" == xml.name()) return parseCorePreferencesXml(xml);
	Qync::parseUnknownElementXml(xml);
	return true;
}


bool QyncPreferences::parseCorePreferencesXml( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement() && "corepreferences" == xml.name());

	while(!xml.atEnd()) {
		xml.readNext();

		if(xml.isEndElement())
			break;

		if(xml.isCharacters()) {
			if(!xml.isWhitespace())
				qDebug() << "QyncPreferences::parseCorePreferencesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

			/* ignore extraneous characters */
			continue;
		}

		if("rsyncpath" == xml.name()) {
			setRsyncPath(xml.readElementText());
		}
		else
			Qync::parseUnknownElementXml(xml);
	}

	return true;
}


bool QyncPreferences::emitXmlStream( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("qyncpreferences");
	emitXmlElements(xml);
	xml.writeEndElement();
	return true;
}


bool QyncPreferences::emitXmlElements( QXmlStreamWriter & xml ) const {
	return emitCorePreferencesXml(xml);
}


bool QyncPreferences::emitCorePreferencesXml( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("corepreferences");
	xml.writeStartElement("rsyncpath");
	xml.writeCharacters(rsyncPath());
	xml.writeEndElement();
	xml.writeEndElement();
	return true;
}


bool QyncPreferences::save( void ) const {
	if(saveCopyAs(m_fileName)) {
		m_modified = false;
		return true;
	}

	return false;
}


bool QyncPreferences::saveAs( const QString & fileName ) {
	if(saveCopyAs(fileName)) {
		m_fileName = fileName;
		m_modified = false;
		return true;
	}

	return false;
}


bool QyncPreferences::saveCopyAs( const QString & fileName ) const {
	QFile file(fileName);
	file.open(QIODevice::WriteOnly);

	if(file.isOpen()) {
		QXmlStreamWriter xml(&file);
		return emitXmlStream(xml);
	}

	return false;
}

bool QyncPreferences::load( void ) {
	if(loadFrom(m_fileName)) {
		m_modified = false;
		return true;
	}

	return false;
}

bool QyncPreferences::loadFrom( const QString & fileName ) {
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);

	if(file.isOpen()) {
		QXmlStreamReader xml(&file);

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isStartElement()) {
				if("qyncpreferences" == xml.name() && parseXmlStream(xml)) {
					m_modified = false;
					m_fileName = fileName;
					return true;
				}
				else
					xml.readElementText();
			}
		}
	}

	return false;
}


QString QyncPreferences::rsyncPath( void ) const {
    return m_rsyncBinary;
}


bool QyncPreferences::setRsyncPath( const QString & path ) {
    m_rsyncBinary = path;
	return true;
}

bool QyncPreferences::rsyncPathIsValid( void ) const {
	QFileInfo f(rsyncPath());
	return f.exists() && f.isFile() && f.isExecutable();
}
