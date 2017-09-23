/**
 * \file preset.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the Preset class.
 *
 * \dep
 * - preset.h
 * - unordered_map
 * - QDebug
 * - QDir
 * - QFile
 * - QVariant
 * - QStringRef
 * - QXmlStreamWriter
 * - QXmlStreamReader
 * - functions.h
 */

#include "preset.h"

#include <unordered_map>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVariant>
#include <QStringRef>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "functions.h"


namespace Qync {


	namespace PresetDetail {
		/*
		 * These aid with loading from and saving to XML. It is not necessary to
		 * implement any added features as properties, but doing so means that the
		 * XML reading/writing code will automatically handle them without having
		 * to write any XML parsing code (which can be a bit verbose).
		 */

		/* data structure containing the getter and setter for a property
		 * getters and setters are member functions of Preset. */
		template<typename T>
		struct PresetProperty {
			using Type = T;
			using Getter = const T & (Preset::*) (void) const;
			using Setter = bool (Preset::*)(const T &);

			const Getter getter;
			const Setter setter;
		};

		/* Alias for a set of properties of a given type. it's a map of
		 * property-name => PresetProperty, with property-name a std::string
		 *
		 * so for any given property "myproperty" the value contains the getter
		 * and setter for "myproperty". So properties["myproperty"].setter(value)
		 * sets the value of "myproperty" and properties["myproperties"].getter()
		 * retrieves the value. The actual call syntax is more complicated because
		 * it needs an object to operate on.
		 */
		template<typename T>
		using PresetProperties = std::unordered_map<std::string, PresetProperty<T>>;

		/* the boolean properties for Preset objects */
		static PresetProperties<bool> booleanPresetProperties = {
		  {"preserveTime", {&Preset::preserveTime, &Preset::setPreserveTime}},
		  {"preservePermissions", {&Preset::preservePermissions, &Preset::setPreservePermissions}},
		  {"preserveOwner", {&Preset::preserveOwner, &Preset::setPreserveOwner}},
		  {"preserveGroup", {&Preset::preserveGroup, &Preset::setPreserveGroup}},

		  {"windowsCompatability", {&Preset::windowsCompatability, &Preset::setWindowsCompatability}},
		  {"honourDeletions", {&Preset::honourDeletions, &Preset::setHonourDeletions}},

		  {"alwaysCompareChecksums", {&Preset::alwaysCompareChecksums, &Preset::setAlwaysCompareChecksums}},
		  {"preserveDevices", {&Preset::preserveDevices, &Preset::setPreserveDevices}},
		  {"keepPartialTransfers", {&Preset::keepPartialTransfers, &Preset::setKeepPartialTransfers}},
		  {"copySymlinksAsSymlinks", {&Preset::copySymlinksAsSymlinks, &Preset::setCopySymlinksAsSymlinks}},
		  {"makeBackups", {&Preset::makeBackups, &Preset::setMakeBackups}},

		  {"useTransferCompression", {&Preset::useTransferCompression, &Preset::setUseTransferCompression}},
		  {"onlyUpdateExistingEntries", {&Preset::onlyUpdateExistingEntries, &Preset::setOnlyUpdateExistingEntries}},
		  {"dontUpdateExistingEntries", {&Preset::dontUpdateExistingEntries, &Preset::setDontUpdateExistingEntries}},
		  {"dontMapUsersAndGroups", {&Preset::dontMapUsersAndGroups, &Preset::setDontMapUsersAndGroups}},
		  {"copyHardlinksAsHardlinks", {&Preset::copyHardlinksAsHardlinks, &Preset::setCopyHardlinksAsHardlinks}},
		  {"showItemisedChanges", {&Preset::showItemisedChanges, &Preset::setShowItemisedChanges}},
		};

		/* the string properties for Preset objects */
		static PresetProperties<QString> stringPresetProperties = {
		  {"logFile", {&Preset::logFile, &Preset::setLogFile}},
		};

	}  // namespace PresetDetail


	/**
	 * \class Preset
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
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
	 * stream to and from a file (load(), save(), saveAs(), saveCopyAs()).
	 *
	 * A Preset object is just a container for settings. It does not \i do anything
	 * itself. An object of this class is provided to a Process object in order for
	 * the Process to set up the rsync command. A set of Preset objects is kept by
	 * the Application instance as the master set of presets available during a
	 * session.
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
		QFile file(fileName);

		if(!file.open(QIODevice::ReadOnly)) {
			qCritical() << __PRETTY_FUNCTION__ << "failed to open file" << fileName << "for reading";
			return nullptr;
		}

		Preset * ret = new Preset;
		ret->setFileName(fileName);
		QXmlStreamReader xml(&file);

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isStartElement()) {
				if("qyncpreset" == xml.name() && ret->parseXml(xml)) {
					return ret;
				}
				else {
					xml.readElementText();
				}
			}
		}

		qCritical() << __PRETTY_FUNCTION__ << "file" << fileName << "does not contain a valid qync preset";
		delete ret;
		return nullptr;
	}


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
		if(!saveCopyAs(fileName)) {
			return false;
		}

		m_fileName = fileName;
		return true;
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

		if(!file.open(QIODevice::WriteOnly)) {
			return false;
		}

		QXmlStreamWriter xml(&file);
		xml.setAutoFormatting(true);
		return emitXml(xml);
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
	 * Most of the settings in a preset are implemented using a basic properties
	 * system. The properties are defined in a map in the PresetDetail private
	 * namespace. Each property has a name, a getter and a setter. This function
	 * iterates over the defined properties and emits a &lt;property&gt; element
	 * to the XML stream for each with the appropriate name and type attributes,
	 * and the value set according to the value provided by the property's getter.
	 *
	 * \return \b true if the properties were written to the stream, \b false
	 * otherwise.
	 */
	bool Preset::emitPropertiesXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("properties");

		for(const auto & propertyDef : PresetDetail::booleanPresetProperties) {
			xml.writeStartElement("property");
			xml.writeAttribute("name", QString::fromStdString(propertyDef.first));
			xml.writeAttribute("type", "boolean");
			xml.writeCharacters((this->*(propertyDef.second.getter))() ? "true" : "false");
			xml.writeEndElement(); /* property */
		}

		for(const auto & propertyDef : PresetDetail::stringPresetProperties) {
			xml.writeStartElement("property");
			xml.writeAttribute("name", QString::fromStdString(propertyDef.first));
			xml.writeAttribute("type", "string");
			xml.writeCharacters((this->*(propertyDef.second.getter))());
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
		Q_ASSERT_X(xml.isStartElement() && "qyncpreset" == xml.name(), __PRETTY_FUNCTION__, "XML stream is not at the start of a \"qyncpreset\" element");
		setDefaultProperties();

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement()) {
				break;
			}

			if(xml.isCharacters()) {
				if(!xml.isWhitespace()) {
					qWarning() << __PRETTY_FUNCTION__ << "Preset::parseXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();
				}

				/* ignore extraneous characters */
				continue;
			}

			if(0 == QString::compare("name", xml.name(), Qt::CaseInsensitive)) {
				setName(xml.readElementText().trimmed());
			}
			else if(0 == QString::compare("source", xml.name(), Qt::CaseInsensitive)) {
				setSource(xml.readElementText());
			}
			else if(0 == QString::compare("destination", xml.name(), Qt::CaseInsensitive)) {
				setDestination(xml.readElementText());
			}
			else if(0 == QString::compare("properties", xml.name(), Qt::CaseInsensitive)) {
				parsePropertiesXml(xml);
			}
			else {
				Qync::parseUnknownElementXml(xml);
			}
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
		Q_ASSERT_X(xml.isStartElement() && "properties" == xml.name(), __PRETTY_FUNCTION__, "XML stream is not at the start of a \"properties\" element");

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement()) {
				break;
			}

			if(xml.isCharacters()) {
				if(!xml.isWhitespace()) {
					qWarning() << __PRETTY_FUNCTION__ << "Preset::parsePropertiesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();
				}

				/* ignore extraneous characters */
				continue;
			}

			if(0 == QString::compare("property", xml.name(), Qt::CaseInsensitive)) {
				parsePropertyXml(xml);
			}
			else {
				Qync::parseUnknownElementXml(xml);
			}
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
		Q_ASSERT_X(xml.isStartElement() && "property" == xml.name(), __PRETTY_FUNCTION__, "XML stream is not at the start of a \"property\" element");

		auto propName = xml.attributes().value("name");
		auto propType = xml.attributes().value("type");
		auto propValueString = xml.readElementText();

		if(0 == QString::compare("boolean", propType, Qt::CaseInsensitive)) {
			auto propertyDef = PresetDetail::booleanPresetProperties.find(propName.toString().toStdString());

			if(propertyDef == PresetDetail::booleanPresetProperties.end()) {
				qWarning() << __PRETTY_FUNCTION__ << "unrecognised boolean property" << propName << "found at line" << xml.lineNumber();
				return false;
			}

			bool propValue;

			if(0 == QString::compare("true", propValueString, Qt::CaseInsensitive)) {
				propValue = true;
			}
			else if(0 == QString::compare("false", propValueString, Qt::CaseInsensitive)) {
				propValue = false;
			}
			else {
				qWarning() << __PRETTY_FUNCTION__ << "Preset::parsePropertyXml() - invalid value" << propValueString << "for boolean property" << propName << "at line" << xml.lineNumber() << "column" << xml.columnNumber();
				return false;
			}

			(this->*(propertyDef->second.setter))(propValue);
		}
		else if(0 == QString::compare("string", propType, Qt::CaseInsensitive)) {
			auto propertyDef = PresetDetail::stringPresetProperties.find(propName.toString().toStdString());

			if(propertyDef == PresetDetail::stringPresetProperties.end()) {
				qWarning() << __PRETTY_FUNCTION__ << "unrecognised string property" << propName << "found at line" << xml.lineNumber();
				return false;
			}

			(this->*(propertyDef->second.setter))(propValueString);
		}
		else {
			qWarning() << __PRETTY_FUNCTION__ << "unrecognised property type" << propType << "for property" << propName << "found at line" << xml.lineNumber();
			return false;
		}

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
	bool Preset::setPreserveTime(const bool & preserve) {
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
	bool Preset::setPreservePermissions(const bool & preserve) {
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
	bool Preset::setPreserveOwner(const bool & preserve) {
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
	bool Preset::setPreserveGroup(const bool & preserve) {
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
	bool Preset::setWindowsCompatability(const bool & compatible) {
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
	bool Preset::setHonourDeletions(const bool & honour) {
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
	bool Preset::setAlwaysCompareChecksums(const bool & compare) {
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
	bool Preset::setPreserveDevices(const bool & preserve) {
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
	bool Preset::setKeepPartialTransfers(const bool & partial) {
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
	bool Preset::setCopySymlinksAsSymlinks(const bool & copyAsLinks) {
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
	bool Preset::setMakeBackups(const bool & backups) {
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
	bool Preset::setUseTransferCompression(const bool & compress) {
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
	bool Preset::setOnlyUpdateExistingEntries(const bool & existingOnly) {
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
	bool Preset::setDontUpdateExistingEntries(const bool & noExisting) {
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
	bool Preset::setDontMapUsersAndGroups(const bool & dontMap) {
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
	bool Preset::setCopyHardlinksAsHardlinks(const bool & copyAsLinks) {
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
	bool Preset::setShowItemisedChanges(const bool & show) {
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
	 * \fn Preset::name(void)
	 * \brief Get the name of the preset.
	 *
	 * \return The name of the preset.
	 */


	/**
	 * \fn Preset::fileName(void)
	 * \brief Get the file name of the preset.
	 *
	 * \return The current path to the file for the preset.
	 */


	/**
	 * \fn Preset::save(void)
	 * \brief Save the preset to its internally stored file name.
	 *
	 * \return \b true if the preset file was saved, \b false otherwise.
	 */


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
