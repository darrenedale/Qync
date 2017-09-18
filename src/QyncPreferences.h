/**
 * \file QyncPreferences.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the QyncPreferences class.
 *
 * \dep
 * - QString
 * - QXmlStreamWriter
 * - QXmlStreamReader
 *
 * \todo
 * Nothing.
 */

#ifndef QYNCPREFERENCES_H
#define QYNCPREFERENCES_H

#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


/**
 * \class QyncPreferences
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
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
class QyncPreferences {
	private:
		mutable bool m_modified;
		QString m_fileName;
		QString m_rsyncBinary;

	protected:
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
		static bool parseBooleanText( const QString & b, bool * ok = 0 );

		/**
		 * \brief Set the default values for all settings.
		 *
		 * By default, the rsync path is set to \b /usr/bin/rsync.
		 *
		 * Reimplementations should call this base class method to ensure that
		 * defaults for core settings are also set.
		 */
		virtual void setDefaults( void );

		/**
		 * \brief Read all settings from an XML stream.
		 *
		 * \param xml is the stream from which to read.
		 *
		 * \return \b true if the settings were read, \b false otherwise.
		 */
		virtual bool parseXmlStream( QXmlStreamReader & xml );

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
		virtual bool parseXmlElement( QXmlStreamReader & xml );

		/**
		 * \brief Read all the core settings from an XML stream.
		 *
		 * \param xml is the stream from which to read.
		 *
		 * \return \b true if the settings were read, \b false otherwise.
		 */
		bool parseCorePreferencesXml( QXmlStreamReader & xml );

		/**
		 * \brief Write all settings to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * \return \b true if the settings were written, \b false otherwise.
		 */
		virtual bool emitXmlStream( QXmlStreamWriter & xml ) const;

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
		virtual bool emitXmlElements( QXmlStreamWriter & xml ) const;

		/**
		 * \brief Write all the core settings to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * \return \b true if the settings were written, \b false otherwise.
		 */
		bool emitCorePreferencesXml( QXmlStreamWriter & xml ) const;

	public:
		/**
		 * \brief Create a new QuncPreferences object.
		 *
		 * \param fileName is the path to the file from which to load the
		 * preferences.
		 *
		 * The file name is optional. If it is not provided, is not a valid
		 * preferences file path or cannot be loaded for some other reason
		 * (e.g. the file is not readable), the preferences will be set to
		 * defaults.
		 */
		explicit QyncPreferences( const QString & fileName = QString() );

		/**
		 * \brief Destroy the QyncPreferences object.
		 */
		virtual ~QyncPreferences( void ) {}

		/**
		 * \brief Save the settings to the internally stored file.
		 *
		 * The settings are saved tothe file set in the constructor or
		 * using loadFrom() or saveAs().
		 *
		 * \return \b true if the settings were saved, \b false otherwise.
		 */
		bool save( void ) const;

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
		bool saveAs( const QString & fileName );

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
		bool saveCopyAs( const QString & fileName ) const;

		/**
		 * \brief (Re)load the settings from the internally stored file.
		 *
		 * The settings are loaded from the file set in the constructor or
		 * using loadFrom() or saveAs().
		 *
		 * \return \b true if the settings were loaded, \b false otherwise.
		 */
		bool load( void );

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
		bool loadFrom( const QString & fileName );

		/**
		 *\brief Get the path to the rsync command.
		 *
		 * \return The path to the rsync command.
		 */
		QString rsyncPath( void ) const;

		/**
		 *\brief Set the path to the rsync command.
		 *
		 * \param path is the path to the rsync command.
		 *
		 * \return \b true if the path was set, \b false otherwise.
		 */
		bool setRsyncPath( const QString & path );

		/**
		 * \brief Check whether the path to the rsync command is valid.
		 *
		 * The path is valid if it identifies a valid, executable file that
		 * the current user has access to.
		 *
		 * \return \b true if the path is valid, \b false otherwise.
		 */
		bool rsyncPathIsValid( void ) const;

};

#endif // QYNCPREFERENCES_H
