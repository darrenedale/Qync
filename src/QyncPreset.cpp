/**
 * \file QyncPreset.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the QyncPreset class.
 *
 * \dep
 * - QyncPreset.h
 * - common.h
 * - QDebug
 * - QDir
 * - QFile
 * - QVariant
 * - QMetaProperty
 * - QStringRef
 * - QXmlStreamWriter
 * - QXmlStreamReader
 *
 * \todo
 * Nothing.
 */

#include "QyncPreset.h"
#include "common.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVariant>
#include <QMetaProperty>
#include <QStringRef>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


QyncPreset::QyncPreset( const QString & name ) {
	setDefaultProperties();
	setName(name);
}


QyncPreset * QyncPreset::load( const QString & fileName ) {
	/* parse the preset from the file */
	QyncPreset * ret = new QyncPreset();
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);

	if(file.isOpen()) {
		ret->setFileName(fileName);
		QXmlStreamReader xml(&file);

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isStartElement()) {
				if("qyncpreset" == xml.name() && ret->parseXml(xml)) return ret;
				else xml.readElementText();
			}
		}

		qDebug() << "file" << fileName << "does not contain a valid qync preset";
	}
	else
		qDebug() << "failed to open file" << fileName << "for reading";

	delete ret;
	return 0;
}


bool QyncPreset::save( void ) const {
	return saveCopyAs(m_fileName);
}


bool QyncPreset::saveAs( const QString & fileName ) {
	if(saveCopyAs(fileName)) {
		m_fileName = fileName;
		return true;
	}

	return false;
}


bool QyncPreset::saveCopyAs( const QString & fileName ) const {
	QFile file(fileName);

	if(file.open(QIODevice::WriteOnly)) {
		QXmlStreamWriter xml(&file);
		return emitXml(xml);
	}

	return false;
}


bool QyncPreset::emitXml( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("qyncpreset");
	emitNameXml(xml);
	emitSourceXml(xml);
	emitDestinationXml(xml);
	emitPropertiesXml(xml);
	xml.writeEndElement();
	return true;
}


bool QyncPreset::emitNameXml( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("name");
	xml.writeCharacters(name());
	xml.writeEndElement();
	return true;
}


bool QyncPreset::emitSourceXml( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("source");
	xml.writeCharacters(source());
	xml.writeEndElement();
	return true;
}


bool QyncPreset::emitDestinationXml( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("destination");
	xml.writeCharacters(destination());
	xml.writeEndElement();
	return true;
}


bool QyncPreset::emitPropertiesXml( QXmlStreamWriter & xml ) const {
	xml.writeStartElement("properties");

	const QMetaObject* mo = metaObject();

	/* write all properties to XML */
	for(int i = mo->propertyOffset(); i < mo->propertyCount(); i++) {
		QMetaProperty mp = mo->property(i);
		xml.writeStartElement("property");
		xml.writeAttribute("name", QString::fromLatin1(mp.name()));
		QString value;

		switch(mp.type()) {
			case QVariant::Bool:
				xml.writeAttribute("type", "boolean");
				value = (property(mp.name()).toBool() ? "true" : "false");
				break;

			case QVariant::String:
				xml.writeAttribute("type", "string");
				value = property(mp.name()).toString();
				break;

			default:
				xml.writeAttribute("type", "unknown");
				value = QString();
				break;
		}

		xml.writeCharacters(value);
		xml.writeEndElement();	/* property */
	}

	xml.writeEndElement();	/* properties */
	return true;
}


bool QyncPreset::parseXml( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement() && "qyncpreset" == xml.name());
	setDefaultProperties();

	while(!xml.atEnd()) {
		xml.readNext();

		if(xml.isEndElement())
			break;

		if(xml.isCharacters()) {
			if(!xml.isWhitespace())
				qDebug() << "QyncPreset::parseXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

			/* ignore extraneous characters */
			continue;
		}

		if("name" == xml.name())
			setName(xml.readElementText().trimmed());
		else if("source" == xml.name())
			setSource(xml.readElementText());
		else if("destination" == xml.name())
			setDestination(xml.readElementText());
		else if("properties" == xml.name())
			parsePropertiesXml(xml);
		else
			Qync::parseUnknownElementXml(xml);
	}

	return true;
}


bool QyncPreset::parsePropertiesXml( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement() && "properties" == xml.name());

	while(!xml.atEnd()) {
		xml.readNext();

		if(xml.isEndElement())
			break;

		if(xml.isCharacters()) {
			if(!xml.isWhitespace())
				qDebug() << "QyncPreset::parsePropertiesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

			/* ignore extraneous characters */
			continue;
		}

		if("property" == xml.name())
			parsePropertyXml(xml);
		else
			Qync::parseUnknownElementXml(xml);
	}

	return true;
}


bool QyncPreset::parsePropertyXml( QXmlStreamReader & xml ) {
	Q_ASSERT(xml.isStartElement() && "property" == xml.name());

	QString name = xml.attributes().value("name").toString();
	QString type = xml.attributes().value("type").toString();
	QString value = xml.readElementText();

	if("boolean" == type) {
		if("true" == value)
			setProperty(name.toLatin1(), true);
		else if("false" == value)
			setProperty(name.toLatin1(), false);
		else {
			qDebug() << "QyncPreset::parsePropertyXml() - invalid value" << value << "for boolean property" << name << "at line" << xml.lineNumber()  << "column" << xml.columnNumber();
			return false;
		}
	}
	else if("string" == type)
		setProperty(name.toLatin1(), value);

	return true;
}


void QyncPreset::setDefaultProperties( void ) {
	m_name = QString();

	m_source = QString();
	m_dest = QString();

	m_preserveTime = true;
	m_preservePerms = true;
	m_preserveOwner = true;
	m_preserveGroup = true;

	m_windowsCompatability = false;
	m_deleteOnDestination = false;

	m_alwaysChecksum = false;
	m_preserveDevices = false;
	m_keepParitalTransfers = false;
	m_symlinksAsSymlinks = false;
	m_makeBackups = false;

	m_compressInTransit = false;
	m_onlyUpdateExisting = false;
	m_dontMapUidGid = false;
	m_copyHardlinksAsHardlinks = false;
	m_showItemisedChanges = false;

	m_logFile = QString();
}


void QyncPreset::setName( const QString & name ) {
	m_name = name;
}


QString QyncPreset::name( void ) const {
	return m_name;
}


void QyncPreset::setFileName( const QString & fileName ) {
	m_fileName = fileName;
}


QString QyncPreset::fileName( void ) const {
	return m_fileName;
}


bool QyncPreset::setSource( const QString & source ) {
	m_source = source;
	return true;
}


bool QyncPreset::setDestination( const QString & dest ) {
	m_dest = dest;
	return true;
}

bool QyncPreset::setPreserveTime( bool preserve ) {
	m_preserveTime = preserve;
	return true;
}


bool QyncPreset::setPreservePermissions( bool preserve ) {
	m_preservePerms = preserve;
	return true;
}


bool QyncPreset::setPreserveOwner( bool preserve ) {
	m_preserveOwner = preserve;
	return true;
}


bool QyncPreset::setPreserveGroup( bool preserve ) {
	m_preserveGroup = preserve;
	return true;
}


bool QyncPreset::setWindowsCompatability( bool compatible ) {
	m_windowsCompatability = compatible;
	return true;
}


bool QyncPreset::setHonourDeletions( bool honour ) {
	this->m_deleteOnDestination = honour;
	return true;
}


bool QyncPreset::setAlwaysCompareChecksums( bool compare ) {
	m_alwaysChecksum = compare;
	return true;
}


bool QyncPreset::setPreserveDevices( bool preserve ) {
	m_preserveDevices = preserve;
	return true;
}


bool QyncPreset::setKeepPartialTransfers( bool partial ) {
	m_keepParitalTransfers = partial;
	return true;
}


bool QyncPreset::setCopySymlinksAsSymlinks( bool copyAsLinks ) {
	m_symlinksAsSymlinks = copyAsLinks;
	return true;
}


bool QyncPreset::setMakeBackups( bool backups ) {
	m_makeBackups = backups;
	return true;
}


bool QyncPreset::setUseTransferCompression( bool compress ) {
	m_compressInTransit = compress;
	return true;
}


bool QyncPreset::setOnlyUpdateExistingEntries( bool existingOnly ) {
	m_onlyUpdateExisting = existingOnly;

	if(existingOnly) {
		m_dontUpdateExisting = false;
	}

	return true;
}


bool QyncPreset::setDontUpdateExistingEntries( bool noExisting ) {
	m_dontUpdateExisting = noExisting;

	if(noExisting) {
		m_onlyUpdateExisting = false;
	}

	return true;
}


bool QyncPreset::setDontMapUsersAndGroups( bool dontMap ) {
	m_dontMapUidGid = dontMap;
	return true;
}


bool QyncPreset::setCopyHardlinksAsHardlinks( bool copyAsLinks ) {
	m_copyHardlinksAsHardlinks = copyAsLinks;
	return true;
}


bool QyncPreset::setShowItemisedChanges( bool show ) {
	m_showItemisedChanges = show;
	return true;
}


bool QyncPreset::setLogFile( const QString & fileName ) {
	m_logFile = fileName;
	return true;
}


QString QyncPreset::source( void ) const {
	return m_source;
}


QString QyncPreset::destination( void ) const {
	return m_dest;
}


bool QyncPreset::preserveTime( void ) const {
	return m_preserveTime;
}


bool QyncPreset::preservePermissions( void ) const {
	return m_preservePerms;
}


bool QyncPreset::preserveOwner( void ) const {
	return m_preserveOwner;
}


bool QyncPreset::preserveGroup( void ) const {
	return m_preserveGroup;
}


bool QyncPreset::windowsCompatability( void ) const {
	return m_windowsCompatability;
}


bool QyncPreset::honourDeletions( void ) const {
	return m_deleteOnDestination;
}


bool QyncPreset::alwaysCompareChecksums( void ) const {
	return m_alwaysChecksum;
}


bool QyncPreset::preserveDevices( void ) const {
	return m_preserveDevices;
}


bool QyncPreset::keepPartialTransfers( void ) const {
	return m_keepParitalTransfers;
}


bool QyncPreset::copySymlinksAsSymlinks( void ) const {
	return m_symlinksAsSymlinks;
}


bool QyncPreset::makeBackups( void ) const {
	return m_makeBackups;
}


bool QyncPreset::useTransferCompression( void ) const {
	return m_compressInTransit;
}


bool QyncPreset::onlyUpdateExistingEntries( void ) const {
	return m_onlyUpdateExisting;
}


bool QyncPreset::dontUpdateExistingEntries( void ) const {
	return m_dontUpdateExisting;
}


bool QyncPreset::dontMapUsersAndGroups( void ) const {
	return m_dontMapUidGid;
}


bool QyncPreset::copyHardlinksAsHardlinks( void ) const {
	return m_copyHardlinksAsHardlinks;
}


bool QyncPreset::showItemisedChanges( void ) const {
	return m_showItemisedChanges;
}


QString QyncPreset::logFile( void ) const {
	return m_logFile;
}
