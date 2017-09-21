/**
 * \file preset.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Implementation of the Preset class.
 *
 * \dep
 * - Preset.h
 * - QDebug
 * - QDir
 * - QFile
 * - QVariant
 * - QMetaProperty
 * - QStringRef
 * - QXmlStreamWriter
 * - QXmlStreamReader
 * - functions.h
 */

#include "preset.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVariant>
#include <QMetaProperty>
#include <QStringRef>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "functions.h"


namespace Qync {


	/**
	 * \class Preset
	 * \author Darren Edale
	 * \date September 2017
	 * \version 0.9.6
	 *
	 * \brief A class to represent a preset for the rsync process.
	 *
	 * This class basically stores all the settings that the user is able to modify
	 * for the rsync process. Most are implemented as Qt properties for flexibility
	 * and to enable the easy extensibility of the XML read and write code. At
	 * present, the following settings can be manipulated:
	 * - whether or not timestamps are preserved (preserveTime(), rsync -t)
	 * - whether or not permissions are preserved (preservePermissions(), rsync -p)
	 * - whether or not ownership is preserved (preserveOwner(), rsync -o)
	 * - whether or not group ownership is preserved (preserveGroup(), rsync -g)
	 * - whether or not destination entries should be deleted (honourDeletions(),
	 *   rsync --delete)
	 * - whether or not checksums should be used (alwaysCompareChecksums(), rsync
	 *   -c)
	 * - whether or not device files are preserved (preserveDevices(), rsync
	 *   --devices)
	 * - whether or not partial transfers should be kept (keepPartialTransfers(),
	 *   rsync --partial)
	 * - whether or not symlinks should be copied as symlinks
	 *   (copySymlinksAsSymlinks(), rsync -l)
	 * - whether or not backups should be made (makeBackups(), rsync -b)
	 * - whether or not transfer compression should be used
	 *   (useTransferCompression(), rsync -z)
	 * - whether or not only existing entries on the destination should be updated
	 *   (onlyUpdateExistingEntries(), rsync --existing)
	 * - whether or not only entries that don't yet exist on the destination should
	 *   be updated (dontUpdateExistingEntries(), rsync --ignore-existing)
	 * - whether or not user and group id mapping should be suppressed
	 *   (dontMapUsersAndGroups(), rsync --numeric-ids)
	 * - whether or not hardlinks should be copied as hardlinks
	 *   (copyHardlinksAsHardlinks(), rsync -H)
	 * - whether or not an itemised list of changes should be generated
	 *   (showItemisedChanges(), rsync -i)
	 * - the log file for the standard output of the rsync command (logFile()).
	 * - the source and destination for the rsync process (source(), destination())
	 *
	 * In addition, it provides (protected) methods to write and read the preset to
	 * and from an XML stream (parseXml(), emitXml()), and to write and read that
	 * stream to and from a file (load(), loadFrom(), save(), saveAs(),
	 * saveCopyAs()).
	 */


	/**
	 * \brief Create a new Preset.
	 *
	 * \param name is the name for the new preset.
	 */
	Preset::Preset(const QString & name) {
		setDefaultProperties();
		setName(name);
	}


	/**
	 * \brief Load a Preset from a file.
	 *
	 * \param fileName is the path to the file to load.
	 *
	 * If the file is a valid Preset file, the file is loaded and a new
	 * Preset object is returned. If not, no Preset is returned. If
	 * successful, the returned preset is owned by the calling code, and it
	 * is responsible for the destruction of the preset at the appropriate
	 * time.
	 *
	 * \return A new preset, or \b null if one could not be loaded.
	 */
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


	/**
	 * \brief Save the preset to its internally stored file name.
	 *
	 * \return \b true if the preset file was saved, \b false otherwise.
	 */


	/**
	 * \brief Save the preset to a new file.
	 *
	 * \param fileName is the name of the file to save to.
	 *
	 * If successful, the internally stored name of the file is set to
	 * the provided file name.
	 *
	 * \return \b true if the preset file was saved, \b false otherwise.
	 */
	bool Preset::saveAs(const QString & fileName) {
		if(saveCopyAs(fileName)) {
			m_fileName = fileName;
			return true;
		}

		return false;
	}


	/**
	 * \brief Save a copy of the preset to a new file.
	 *
	 * \param fileName is the name of the file to save to.
	 *
	 * The internally stored name of the file is never altered by this
	 * method.
	 *
	 * \return \b true if the preset file was saved, \b false otherwise.
	 */
	bool Preset::saveCopyAs(const QString & fileName) const {
		QFile file(fileName);

		if(file.open(QIODevice::WriteOnly)) {
			QXmlStreamWriter xml(&file);
			return emitXml(xml);
		}

		return false;
	}


	/**
	 * \brief Write the preset to an XML stream.
	 *
	 * \param xml is the stream to write to.
	 *
	 * \return \b true if the preset was written to the stream, \b false
	 * otherwise.
	 */
	bool Preset::emitXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("qyncpreset");
		emitNameXml(xml);
		emitSourceXml(xml);
		emitDestinationXml(xml);
		emitPropertiesXml(xml);
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the name of the preset to an XML stream.
	 *
	 * \param xml is the stream to write to.
	 *
	 * \return \b true if the name of the preset was written to the stream,
	 * \b false otherwise.
	 */
	bool Preset::emitNameXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("name");
		xml.writeCharacters(name());
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the preset source to an XML stream.
	 *
	 * \param xml is the stream to write to.
	 *
	 * \return \b true if the preset source was written to the stream,
	 * \b false otherwise.
	 */
	bool Preset::emitSourceXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("source");
		xml.writeCharacters(source());
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the preset destination to an XML stream.
	 *
	 * \param xml is the stream to write to.
	 *
	 * \return \b true if the preset destination was written to the stream,
	 * \b false otherwise.
	 */
	bool Preset::emitDestinationXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("destination");
		xml.writeCharacters(destination());
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the preset properties to an XML stream.
	 *
	 * \param xml is the stream to write to.
	 *
	 * Most of the settings in a preset are implemented using Qt's
	 * properties system. This method automatically writes all properties
	 * to the stream.
	 *
	 * \return \b true if the preset properties were written to the stream,
	 * \b false otherwise.
	 */
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


	/**
	 * \brief Read the preset from an XML stream.
	 *
	 * \param xml is the stream to read from.
	 *
	 * \return \c true if the preset was read from the stream, \b false
	 * otherwise.
	 */
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


	/**
	 * \brief Read the properties for the preset from an XML stream.
	 *
	 * \param xml is the stream to read from.
	 *
	 * \return \c true if the properties for the preset were read from the
	 * stream, \b false otherwise.
	 */
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


	/**
	 * \brief Read an individual property for the preset from an XML stream.
	 *
	 * \param xml is the stream to read from.
	 *
	 * \return \c true if the property for the preset was read from the
	 * stream, \b false otherwise.
	 */
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


	/**
	 * \brief Set all settings to default values.
	 */
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


	/**
	 * \brief Set the name of the preset.
	 *
	 * \param name is the new name for the preset.
	 *
	 * Presets do not need to have unique names.
	 */
	void Preset::setName(const QString & name) {
		m_name = name;
	}


	/**
	 * \brief Get the name of the preset.
	 *
	 * \return The name of the preset.
	 */


	/**
	 * \brief Set the file name of the preset.
	 *
	 * \param fileName is the new path to the file for the preset.
	 *
	 * Setting the file name does not automatically save the preset.
	 */
	void Preset::setFileName(const QString & fileName) {
		m_fileName = fileName;
	}


	/**
	 * \brief Get the file name of the preset.
	 *
	 * \return The current path to the file for the preset.
	 */


	/**
	 * \brief Set the source file or directory.
	 *
	 * \param source is the path to the source file or directory.
	 *
	 * \return \b true if the source was set, \b false otherwise.
	 */
	bool Preset::setSource(const QString & source) {
		m_source = source;
		return true;
	}


	/**
	 * \brief Set the destination file or directory.
	 *
	 * \param dest is the path to the destination file or directory.
	 *
	 * \return \b true if the destination was set, \b false otherwise.
	 */
	bool Preset::setDestination(const QString & dest) {
		m_dest = dest;
		return true;
	}


	/**
	 * \brief Set whether or not file timestamps should be preserved.
	 *
	 * \param preserve indicates whether the timestamps should be preserved.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setPreserveTime(bool preserve) {
		m_preserveTime = preserve;
		return true;
	}


	/**
	 * \brief Set whether or not file permissions should be preserved.
	 *
	 * \param preserve indicates whether the permissions should be
	 * preserved.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setPreservePermissions(bool preserve) {
		m_preservePerms = preserve;
		return true;
	}


	/**
	 * \brief Set whether or not file ownership should be preserved.
	 *
	 * \param preserve indicates whether the ownership should be preserved.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setPreserveOwner(bool preserve) {
		m_preserveOwner = preserve;
		return true;
	}


	/**
	 * \brief Set whether or not file group ownership should be preserved.
	 *
	 * \param preserve indicates whether the group ownership should be
	 * preserved.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setPreserveGroup(bool preserve) {
		m_preserveGroup = preserve;
		return true;
	}


	/**
	 * \brief Set whether or not Windows compatability should be used.
	 *
	 * \param compatible indicates whether Windows compatability should be
	 * used.
	 *
	 * Windows compatability means accommodating some imprecision in the
	 * way certain filestytems on the Windows platform record file
	 * modification timestamps.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setWindowsCompatability(bool compatible) {
		m_windowsCompatability = compatible;
		return true;
	}


	/**
	 * \brief Set whether or not deletions in the source directory tree
	 * should be honoured.
	 *
	 * \param honour indicates whether deletions should be honoured.
	 *
	 * Honouring deletions means that files or directories in the source
	 * tree that have been removed since the last synchronisation will also
	 * be removed from the destination tree.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setHonourDeletions(bool honour) {
		this->m_deleteOnDestination = honour;
		return true;
	}


	/**
	 * \brief Set whether or not to use checksums to compare source and
	 * destination items.
	 *
	 * \param checksums indicates whether checksums should be used.
	 *
	 * Using checksums means that the size and modification timestamp of
	 * files is not used as the sole indicator of whether a file needs to be
	 * updated in the destination tree.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setAlwaysCompareChecksums(bool compare) {
		m_alwaysChecksum = compare;
		return true;
	}


	/**
	 * \brief Set whether or not device files should be preserved as such.
	 *
	 * \param preserve indicates whether the device files should be
	 * preserved.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setPreserveDevices(bool preserve) {
		m_preserveDevices = preserve;
		return true;
	}


	/**
	 * \brief Set whether or not partially-transferred items should be kept.
	 *
	 * \param keep indicates whether partially-transferred items should be
	 * kept.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setKeepPartialTransfers(bool partial) {
		m_keepParitalTransfers = partial;
		return true;
	}


	/**
	 * \brief Set whether or not symbolic links should be kept as such.
	 *
	 * \param links indicates whether symbolic links should be kept as such.
	 *
	 * If this setting is set, symbolic links in the source tree will be
	 * copied as symbolic links; if not, symbolic links in the source tree
	 * will be followed and the original file will be copied.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setCopySymlinksAsSymlinks(bool copyAsLinks) {
		m_symlinksAsSymlinks = copyAsLinks;
		return true;
	}


	/**
	 * \brief Set whether or not backups of the destination tree should be
	 * made.
	 *
	 * \param backup indicates whether backups should be made.
	 *
	 * If this setting is set, backups of the pre-synchronisation entries in
	 * the destination tree will be made; if not, destination entries will
	 * simply be overwritten.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setMakeBackups(bool backups) {
		m_makeBackups = backups;
		return true;
	}


	/**
	 * \brief Set whether or not to use transfer compression.
	 *
	 * \param compress indicates whether or not to use compression.
	 *
	 * If compression is used, any data transferred to the destination will
	 * be compressed in transit. This is useful for large data sets or when
	 * either the source or destination is remote.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setUseTransferCompression(bool compress) {
		m_compressInTransit = compress;
		return true;
	}


	/**
	 * \brief Set whether or not only existing destination entries should
	 * be updated.
	 *
	 * \param onlyExisting indicates whether only existing entries are
	 * updated.
	 *
	 * If this is set, any entries in the source tree that do not already
	 * exist in the destination tree will be ignored; if this is not set,
	 * any entries in the source tree that do not already exist in the
	 * destination tree will result in new entries being created in the
	 * destination tree.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setOnlyUpdateExistingEntries(bool existingOnly) {
		m_onlyUpdateExisting = existingOnly;

		if(existingOnly) {
			m_dontUpdateExisting = false;
		}

		return true;
	}


	/**
	 * \brief Set whether or not to ignore entries that already exist on
	 * the destination.
	 *
	 * \param noExisting indicates whether existing entries are ignored.
	 *
	 * If this is set, any entries in the source tree that already exist
	 * in the destination tree will be ignored; if this is not set, any
	 * entries in the source tree that already exist in the destination
	 * tree will be updated in the destination tree if the entry in the
	 * source tree is different.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setDontUpdateExistingEntries(bool noExisting) {
		m_dontUpdateExisting = noExisting;

		if(noExisting) {
			m_onlyUpdateExisting = false;
		}

		return true;
	}


	/**
	 * \brief Set whether or not to use UID and GID to username and group
	 * name mapping.
	 *
	 * \param dontMap indicates whether to suppress UID and GID mapping.
	 *
	 * Ordinarily, UID and GID values will be mapped to usernames and
	 * group names so that the username and group name of destination
	 * entries matches that of source entries. This can be suppressed so
	 * that the original UID and GID are used on the destination entries.
	 * This makes little difference unless either the source or destination
	 * is remote and therefore potentially uses different UIDs and GIDs for
	 * the same username and group name.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setDontMapUsersAndGroups(bool dontMap) {
		m_dontMapUidGid = dontMap;
		return true;
	}


	/**
	 * \brief Set whether or not hard links should be kept as such.
	 *
	 * \param links indicates whether hard links should be kept as such.
	 *
	 * If this setting is set, hard links in the source tree will be copied
	 * as hard links; if not, hard links in the source tree will be followed
	 * and the original file will be copied.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setCopyHardlinksAsHardlinks(bool copyAsLinks) {
		m_copyHardlinksAsHardlinks = copyAsLinks;
		return true;
	}


	/**
	 * \brief Set whether or not an itemised list of changes should be
	 * generated.
	 *
	 * \param itemise indicates whether an itemised list should be
	 * generated.
	 *
	 * \return \b true if the setting was set, \b false otherwise.
	 */
	bool Preset::setShowItemisedChanges(bool show) {
		m_showItemisedChanges = show;
		return true;
	}


	/**
	 * \brief Set the log file.
	 *
	 * \param fileName is the path to the new log file.
	 *
	 * The log file can be set to a \b null or empty string to suppress
	 * capturing of the rsync command's standard output to a log file.
	 *
	 * \return \b true if the log file was set, \c false otherwise.
	 */
	bool Preset::setLogFile(const QString & fileName) {
		m_logFile = fileName;
		return true;
	}


	/**
	 * \fn Preset::source(void)
	 * \brief Get the source location.
	 *
	 * \return The source.
	 */


	/**
	 * \fn Preset::destination(void)
	 * \brief Get the destination location.
	 *
	 * \return The destination.
	 */


	/**
	 * \fn Preset::preserveTime(void)
	 * \brief Get whether file timestamps should be preserved.
	 *
	 * \return \b true if timestamps should be preserved, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::preservePermissions(void)
	 * \brief Get whether file permissions should be preserved.
	 *
	 * \return \b true if permissions should be preserved, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::preserveOwner(void)
	 * \brief Get whether file ownership should be preserved.
	 *
	 * \return \b true if ownership should be preserved, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::preserveGroup(void)
	 * \brief Get whether file group ownership should be preserved.
	 *
	 * \return \b true if group ownership should be preserved, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::windowsCompatability(void)
	 * \brief Get whether Windows compatability should be used.
	 *
	 * \return \b true if Windows compatability should be used, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::honourDeletions(void)
	 * \brief Get whether source tree deletions should be honoured.
	 *
	 * \return \b true if source tree deletions should be honoured, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::alwaysCompareChecksums(void)
	 * \brief Get whether entry checksums should always be used.
	 *
	 * \return \b true if entry checksums should always be used, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::preserveDevices(void)
	 * \brief Get whether or device files should be preserved.
	 *
	 * \return \b true if device files should be preserved, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::keepPartialTransfers(void)
	 * \brief Get whether partial transfers should be kept.
	 *
	 * \return \b true if partial transfers should be kept, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::copySymlinksAsSymlinks(void)
	 * \brief Get whether symbolic links should be copied as symbolic
	 * links.
	 *
	 * \return \b true if symbolic links should be copied as symbolic links,
	 * \b false otherwise.
	 */


	/**
	 * \fn Preset::makeBackups(void)
	 * \brief Get whether destination tree backups should be made.
	 *
	 * \return \b true if destination tree backups should be made, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::useTransferCompression(void)
	 * \brief Get whether transfer compression should be used.
	 *
	 * \return \b true if transfer compression should be used, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::onlyUpdateExistingEntries(void)
	 * \brief Get whether only existing destination entries should be
	 * updated.
	 *
	 * \return \b true if only existing destination should be updated,
	 * \b false otherwise.
	 */


	/**
	 * \fn Preset::dontUpdateExistingEntries(void)
	 * \brief Get whether entries that already exist on the destination
	 * should be ignored.
	 *
	 * \return \b true if entries that already exist on the destination
	 * will be ignored.
	 */


	/**
	 * \fn Preset::dontMapUsersAndGroups(void)
	 * \brief Get whether UID and GID mapping should be suppressed.
	 *
	 * \return \b true if UID and GID mapping should be suppressed, \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::copyHardlinksAsHardlinks(void)
	 * \brief Get whether hard links should be copied as hard links.
	 *
	 * \return \b true if hard links should be copied as hard links,
	 * \b false otherwise.
	 */


	/**
	 * \fn Preset::showItemisedChanges(void)
	 * \brief Get whether a list of itemised changes should be generated.
	 *
	 * \return \b true if a list of itemised changes should be generated,
	 * \b false
	 * otherwise.
	 */


	/**
	 * \fn Preset::logFile(void)
	 * \brief Get the log file.
	 *
	 * \return The path to the log file.
	 */


}  // namespace Qync
