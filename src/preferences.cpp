/**
 * \file preferences.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the Preferences class.
 *
 * \dep
 * - preferences.h
 * - QDebug
 * - QXmlStreamWriter
 * - QXmlStreamReader
 * - QDir
 * - QFileInfo
 * - QProcess
 * - functions.h
 */

#include "preferences.h"

#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

#include "functions.h"


namespace Qync {


	/**
	 * \class Preferences
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
	 *
	 * \brief Base class representing the core Qync preferences.
	 *
	 * This class represents the core preferences for a Qync application. At present
	 * it has only one setting, which is the path of the \b rsync executable file.
	 * This is set using setRsyncPath(), read using rsyncPath() and its validity can
	 * be assessed using rsyncPathIsValid().
	 *
	 * In addition to managing this setting, the class provides the core loading -
	 * load() and loadFrom() - and saving - save(), saveAs() and saveCopyAs() -
	 * features that will work for all subclasses and need not be reimplemented.
	 * These methods make use of the XML read and write virtual methods described
	 * below.
	 *
	 * The base class features of the class mainly revolve around the actual reading
	 * and writing of XML streams. The parseXmlStream() virtual method reads
	 * settings from an XML stream and the emitXmlStream() virtual method writes
	 * settings to an XML stream. It is recommended that these method are not
	 * reimplemented in subclasses to ensure that settings files from subclasses
	 * remain compatible with those of the base class. Instead, subclasses should
	 * reimplement the virtual emitXmlElements() and parseXmlElement() methods to
	 * add their own extensions to the XML file. The method documentation provides
	 * simple details of how to achieve this while remaining compatible with the
	 * base class settings. The setDefaults() virtual method can also be
	 * reimplemented to provide defaults for extended settings. Reimplementations
	 * should call their direct base class implementation to ensure that defaults
	 * for core settings are also set.
	 */


	/**
	 * \brief Create a new Preferences object.
	 *
	 * The preferences will be set to default and there will be no file
	 * name, so load() and save() will fail (use loadFrom() or saveAs()
	 * to set the file name).
	 */
	Preferences::Preferences(void)
	: m_fileName(),
	  m_rsyncBinary() {
		setDefaults();
	}


	/**
	 * \brief Create a new Preferences object.
	 *
	 * \param fileName is the path to the file from which to load the
	 * preferences.
	 *
	 * The preferences will be loaded from the provided file name, if
	 * it is a valid preferences file.
	 */
	Preferences::Preferences(const QString & fileName)
	: Preferences() {
		m_fileName = fileName;
		load();
	}


	/**
	 * \brief Set the default values for all settings.
	 *
	 * By default, the rsync path is set to \b /usr/bin/rsync.
	 *
	 * Reimplementations should call this base class method to ensure that
	 * defaults for core settings are also set.
	 */
	void Preferences::setDefaults(void) {
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
		qCritical() << __PRETTY_FUNCTION__ << "unsupported platform - default rsync binary unknown";
		setRsyncPath({});
#endif
	}


	/**
	 * \brief Convert the text representation of a boolean value to an
	 * actual \b bool.
	 *
	 * \param b is the boolean text to convert.
	 * \param ok is a pointer to a boolean value that will be set to \b true
	 * if the conversion is successful, \b false if not.
	 *
	 * This is a helper method to parse the content of a boolean text from
	 * the preferences file. A valid \b bool is always returned - callers
	 * must provide an \b ok parameter and check its content when the method
	 * returns if they need to check whether the returned value represents
	 * the text provided or is just a default value because the text was not
	 * valid.
	 *
	 * \return The \b bool value.
	 */
	bool Preferences::parseBooleanText(const QString & b, bool * ok) {
		if(0 == QString::compare("true", b.trimmed(), Qt::CaseInsensitive)) {
			if(ok) {
				*ok = true;
			}

			return true;
		}
		else if(0 == QString::compare("false", b.trimmed(), Qt::CaseInsensitive)) {
			if(ok) {
				*ok = true;
			}

			return false;
		}

		if(ok) {
			*ok = false;
		}

		return false;
	}


	/**
	 * \brief Read all settings from an XML stream.
	 *
	 * \param xml is the stream from which to read.
	 *
	 * \return \b true if the settings were read, \b false otherwise.
	 */
	bool Preferences::parseXmlStream(QXmlStreamReader & xml) {
		Q_ASSERT_X(xml.isStartElement() && "qyncpreferences" == xml.name(), __PRETTY_FUNCTION__, "no XML start element or element is not qyncpreferences");
		setDefaults();

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement()) {
				break;
			}

			if(xml.isCharacters()) {
				if(!xml.isWhitespace()) {
					qWarning() << __PRETTY_FUNCTION__ << "Preferences::parseXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();
				}

				/* ignore extraneous characters */
				continue;
			}

			parseXmlElement(xml);
		}

		return true;
	}


	/**
	 * \brief Parse an arbitrary XML element from a stream.
	 *
	 * \param xml is the stream from which to read.
	 *
	 * This is a virtual method. Subclasses should reimplement this method
	 * to ensure their own extensions to the settings file are parsed. The
	 * reimplementation should pass all unrecognised XML elements up the
	 * class hierarchy by calling parseXmlElement() in their direct parent
	 * class.
	 *
	 * \return \b true if the settings were read, \b false otherwise.
	 */
	bool Preferences::parseXmlElement(QXmlStreamReader & xml) {
		Q_ASSERT_X(xml.isStartElement(), __PRETTY_FUNCTION__, "no XML start element");

		if("corepreferences" == xml.name()) {
			return parseCorePreferencesXml(xml);
		}

		Qync::parseUnknownElementXml(xml);
		return true;
	}


	/**
	 * \brief Read all the core settings from an XML stream.
	 *
	 * \param xml is the stream from which to read.
	 *
	 * \return \b true if the settings were read, \b false otherwise.
	 */
	bool Preferences::parseCorePreferencesXml(QXmlStreamReader & xml) {
		Q_ASSERT_X(xml.isStartElement() && "corepreferences" == xml.name(), __PRETTY_FUNCTION__, "no XML start element or element is not corepreferences");

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement()) {
				break;
			}

			if(xml.isCharacters()) {
				if(!xml.isWhitespace()) {
					qWarning() << __PRETTY_FUNCTION__ << "Preferences::parseCorePreferencesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();
				}

				/* ignore extraneous characters */
				continue;
			}

			if("rsyncpath" == xml.name()) {
				setRsyncPath(xml.readElementText());
			}
			else {
				Qync::parseUnknownElementXml(xml);
			}
		}

		return true;
	}


	/**
	 * \brief Write all settings to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * \return \b true if the settings were written, \b false otherwise.
	 */
	bool Preferences::emitXmlStream(QXmlStreamWriter & xml) const {
		xml.writeStartElement("qyncpreferences");
		emitXmlElements(xml);
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write all settings to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * This is a virtual method. Subclasses should reimplement this method
	 * to ensure their own extensions to the settings file are written. The
	 * reimplementation should pass the call back up the class hierarchy
	 * at some point in their execution by including a call to the
	 * emitXmlElements() from the subclass's direct base class to ensure
	 * that all base class settings are also written to the stream.
	 *
	 * \return \b true if the settings were written, \b false otherwise.
	 */
	bool Preferences::emitXmlElements(QXmlStreamWriter & xml) const {
		return emitCorePreferencesXml(xml);
	}


	/**
	 * \brief Write all the core settings to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * \return \b true if the settings were written, \b false otherwise.
	 */
	bool Preferences::emitCorePreferencesXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("corepreferences");
		xml.writeStartElement("rsyncpath");
		xml.writeCharacters(rsyncPath());
		xml.writeEndElement();
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Save the settings to the internally stored file.
	 *
	 * The settings are saved tothe file set in the constructor or
	 * using loadFrom() or saveAs().
	 *
	 * \return \b true if the settings were saved, \b false otherwise.
	 */
	bool Preferences::save(void) const {
		if(saveCopyAs(m_fileName)) {
			return true;
		}

		return false;
	}


	/**
	 * \brief Save the settings to a named file.
	 *
	 * \param fileName is the path of the file to save.
	 *
	 * If the save is successful, the internally stored file name is set
	 * to the name of the file.
	 *
	 * \return \b true if the settings were saved, \b false otherwise.
	 */
	bool Preferences::saveAs(const QString & fileName) {
		if(saveCopyAs(fileName)) {
			m_fileName = fileName;
			return true;
		}

		return false;
	}


	/**
	 * \brief Save a copy of the settings to a named file.
	 *
	 * \param fileName is the path of the file to save.
	 *
	 * Regardless of whether the save is successful or not, the internally
	 * stored file name is never altered by this method.
	 *
	 * \return \b true if the settings were saved, \b false otherwise.
	 */
	bool Preferences::saveCopyAs(const QString & fileName) const {
		QFile file(fileName);

		if(!file.open(QIODevice::WriteOnly)) {
			return false;
		}

		QXmlStreamWriter xml(&file);
		xml.setAutoFormatting(true);
		return emitXmlStream(xml);
	}


	/**
	 * \brief (Re)load the settings from the internally stored file.
	 *
	 * The settings are loaded from the file set in the constructor or
	 * using loadFrom() or saveAs().
	 *
	 * \return \b true if the settings were loaded, \b false otherwise.
	 */
	bool Preferences::load(void) {
		if(m_fileName.isEmpty()) {
			qWarning() << __PRETTY_FUNCTION__ << "can't load prefs with no file name";
			return false;
		}

		return loadFrom(m_fileName);
	}


	/**
	 * \brief Load settings from a named file.
	 *
	 * \param fileName is the path of the file to load.
	 *
	 * If the load is successful, the internally stored file name is set
	 * to the name of the file.
	 *
	 * \return \b true if the settings were loaded, \b false otherwise.
	 */
	bool Preferences::loadFrom(const QString & fileName) {
		if(fileName.isEmpty()) {
			qCritical() << __PRETTY_FUNCTION__ << "empty file name provided";
			return false;
		}

		QFile file(fileName);

		if(!file.open(QIODevice::ReadOnly)) {
			qCritical() << __PRETTY_FUNCTION__ << "failed to open preferences file" << fileName << "for reading";
			return false;
		}

		QXmlStreamReader xml(&file);

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isStartElement()) {
				if(0 == QString::compare("qyncpreferences", xml.name(), Qt::CaseInsensitive) && parseXmlStream(xml)) {
					m_fileName = fileName;
					return true;
				}

				xml.readElementText();
			}
		}

		return false;
	}


	/**
	 * \brief Check whether the path to the rsync command is valid.
	 *
	 * The path is valid if it identifies a valid, executable file that
	 * the current user has access to.
	 *
	 * \return \b true if the path is valid, \b false otherwise.
	 */
	bool Preferences::rsyncPathIsValid(void) const {
		QFileInfo f(rsyncPath());
		return f.exists() && f.isFile() && f.isExecutable();
	}


	/**
	 * \fn Preferences::~Preferences(void)
	 * \brief Destroy the Preferences object.
	 */


	/**
	 *\brief Get the path to the rsync command.
	 *
	 * \return The path to the rsync command.
	 */


	/**
	 *\brief Set the path to the rsync command.
	 *
	 * \param path is the path to the rsync command.
	 *
	 * \return \b true if the path was set, \b false otherwise.
	 */


}  // namespace Qync
