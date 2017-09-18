/**
 * \file Preset.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the Preset class.
 *
 * \dep
 * - Preset.h
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

#include "Preset.h"
#include "common.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVariant>
#include <QMetaProperty>
#include <QStringRef>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


namespace Qync {


	Preset::Preset(const QString & name) {
		setDefaultProperties();
		setName(name);
	}


	Preset * Preset::load(const QString & fileName) {
		/* parse the preset from the file */
		Preset * ret = new Preset();
		QFile file(fileName);
		file.open(QIODevice::ReadOnly);

		if(file.isOpen()) {
			ret->setFileName(fileName);
			QXmlStreamReader xml(&file);

			while(!xml.atEnd()) {
				xml.readNext();

				if(xml.isStartElement()) {
					if("qyncpreset" == xml.name() && ret->parseXml(xml))
						return ret;
					else
						xml.readElementText();
				}
			}

			qDebug() << "file" << fileName << "does not contain a valid qync preset";
		}
		else
			qDebug() << "failed to open file" << fileName << "for reading";

		delete ret;
		return 0;
	}


	bool Preset::save(void) const {
		return saveCopyAs(m_fileName);
	}


	bool Preset::saveAs(const QString & fileName) {
		if(saveCopyAs(fileName)) {
			m_fileName = fileName;
			return true;
		}

		return false;
	}


	bool Preset::saveCopyAs(const QString & fileName) const {
		QFile file(fileName);

		if(file.open(QIODevice::WriteOnly)) {
			QXmlStreamWriter xml(&file);
			return emitXml(xml);
		}

		return false;
	}


	bool Preset::emitXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("qyncpreset");
		emitNameXml(xml);
		emitSourceXml(xml);
		emitDestinationXml(xml);
		emitPropertiesXml(xml);
		xml.writeEndElement();
		return true;
	}


	bool Preset::emitNameXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("name");
		xml.writeCharacters(name());
		xml.writeEndElement();
		return true;
	}


	bool Preset::emitSourceXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("source");
		xml.writeCharacters(source());
		xml.writeEndElement();
		return true;
	}


	bool Preset::emitDestinationXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("destination");
		xml.writeCharacters(destination());
		xml.writeEndElement();
		return true;
	}


	bool Preset::emitPropertiesXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("properties");

		const QMetaObject * mo = metaObject();

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
			xml.writeEndElement(); /* property */
		}

		xml.writeEndElement(); /* properties */
		return true;
	}


	bool Preset::parseXml(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement() && "qyncpreset" == xml.name());
		setDefaultProperties();

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement())
				break;

			if(xml.isCharacters()) {
				if(!xml.isWhitespace())
					qDebug() << "Preset::parseXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

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


	bool Preset::parsePropertiesXml(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement() && "properties" == xml.name());

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement())
				break;

			if(xml.isCharacters()) {
				if(!xml.isWhitespace())
					qDebug() << "Preset::parsePropertiesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

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


	bool Preset::parsePropertyXml(QXmlStreamReader & xml) {
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
				qDebug() << "Preset::parsePropertyXml() - invalid value" << value << "for boolean property" << name << "at line" << xml.lineNumber() << "column" << xml.columnNumber();
				return false;
			}
		}
		else if("string" == type)
			setProperty(name.toLatin1(), value);

		return true;
	}


	void Preset::setDefaultProperties(void) {
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


	void Preset::setName(const QString & name) {
		m_name = name;
	}


	QString Preset::name(void) const {
		return m_name;
	}


	void Preset::setFileName(const QString & fileName) {
		m_fileName = fileName;
	}


	QString Preset::fileName(void) const {
		return m_fileName;
	}


	bool Preset::setSource(const QString & source) {
		m_source = source;
		return true;
	}


	bool Preset::setDestination(const QString & dest) {
		m_dest = dest;
		return true;
	}

	bool Preset::setPreserveTime(bool preserve) {
		m_preserveTime = preserve;
		return true;
	}


	bool Preset::setPreservePermissions(bool preserve) {
		m_preservePerms = preserve;
		return true;
	}


	bool Preset::setPreserveOwner(bool preserve) {
		m_preserveOwner = preserve;
		return true;
	}


	bool Preset::setPreserveGroup(bool preserve) {
		m_preserveGroup = preserve;
		return true;
	}


	bool Preset::setWindowsCompatability(bool compatible) {
		m_windowsCompatability = compatible;
		return true;
	}


	bool Preset::setHonourDeletions(bool honour) {
		this->m_deleteOnDestination = honour;
		return true;
	}


	bool Preset::setAlwaysCompareChecksums(bool compare) {
		m_alwaysChecksum = compare;
		return true;
	}


	bool Preset::setPreserveDevices(bool preserve) {
		m_preserveDevices = preserve;
		return true;
	}


	bool Preset::setKeepPartialTransfers(bool partial) {
		m_keepParitalTransfers = partial;
		return true;
	}


	bool Preset::setCopySymlinksAsSymlinks(bool copyAsLinks) {
		m_symlinksAsSymlinks = copyAsLinks;
		return true;
	}


	bool Preset::setMakeBackups(bool backups) {
		m_makeBackups = backups;
		return true;
	}


	bool Preset::setUseTransferCompression(bool compress) {
		m_compressInTransit = compress;
		return true;
	}


	bool Preset::setOnlyUpdateExistingEntries(bool existingOnly) {
		m_onlyUpdateExisting = existingOnly;

		if(existingOnly) {
			m_dontUpdateExisting = false;
		}

		return true;
	}


	bool Preset::setDontUpdateExistingEntries(bool noExisting) {
		m_dontUpdateExisting = noExisting;

		if(noExisting) {
			m_onlyUpdateExisting = false;
		}

		return true;
	}


	bool Preset::setDontMapUsersAndGroups(bool dontMap) {
		m_dontMapUidGid = dontMap;
		return true;
	}


	bool Preset::setCopyHardlinksAsHardlinks(bool copyAsLinks) {
		m_copyHardlinksAsHardlinks = copyAsLinks;
		return true;
	}


	bool Preset::setShowItemisedChanges(bool show) {
		m_showItemisedChanges = show;
		return true;
	}


	bool Preset::setLogFile(const QString & fileName) {
		m_logFile = fileName;
		return true;
	}


	QString Preset::source(void) const {
		return m_source;
	}


	QString Preset::destination(void) const {
		return m_dest;
	}


	bool Preset::preserveTime(void) const {
		return m_preserveTime;
	}


	bool Preset::preservePermissions(void) const {
		return m_preservePerms;
	}


	bool Preset::preserveOwner(void) const {
		return m_preserveOwner;
	}


	bool Preset::preserveGroup(void) const {
		return m_preserveGroup;
	}


	bool Preset::windowsCompatability(void) const {
		return m_windowsCompatability;
	}


	bool Preset::honourDeletions(void) const {
		return m_deleteOnDestination;
	}


	bool Preset::alwaysCompareChecksums(void) const {
		return m_alwaysChecksum;
	}


	bool Preset::preserveDevices(void) const {
		return m_preserveDevices;
	}


	bool Preset::keepPartialTransfers(void) const {
		return m_keepParitalTransfers;
	}


	bool Preset::copySymlinksAsSymlinks(void) const {
		return m_symlinksAsSymlinks;
	}


	bool Preset::makeBackups(void) const {
		return m_makeBackups;
	}


	bool Preset::useTransferCompression(void) const {
		return m_compressInTransit;
	}


	bool Preset::onlyUpdateExistingEntries(void) const {
		return m_onlyUpdateExisting;
	}


	bool Preset::dontUpdateExistingEntries(void) const {
		return m_dontUpdateExisting;
	}


	bool Preset::dontMapUsersAndGroups(void) const {
		return m_dontMapUidGid;
	}


	bool Preset::copyHardlinksAsHardlinks(void) const {
		return m_copyHardlinksAsHardlinks;
	}


	bool Preset::showItemisedChanges(void) const {
		return m_showItemisedChanges;
	}


	QString Preset::logFile(void) const {
		return m_logFile;
	}

}  // namespace Qync
