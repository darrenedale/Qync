/**
 * \file QyncPreset.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the QyncPreset class.
 *
 * \dep
 * - QObject
 * - QString
 * - QXmlStreamReader
 * - QXmlStreamWriter
 *
 * \todo
 * Nothing.
 */

#ifndef QYNC_PRESET_H
#define QYNC_PRESET_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Qync {

	/**
 * \class Preset
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
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
	class Preset
	  : public QObject {
		Q_OBJECT
		Q_PROPERTY(bool preserveTime READ preserveTime WRITE setPreserveTime)
		Q_PROPERTY(bool preservePermissions READ preservePermissions WRITE setPreservePermissions)
		Q_PROPERTY(bool preserveOwner READ preserveOwner WRITE setPreserveOwner)
		Q_PROPERTY(bool preserveGroup READ preserveGroup WRITE setPreserveGroup)

		Q_PROPERTY(bool windowsCompatability READ windowsCompatability WRITE setWindowsCompatability)
		Q_PROPERTY(bool honourDeletions READ honourDeletions WRITE setHonourDeletions)

		Q_PROPERTY(bool alwaysCompareChecksums READ alwaysCompareChecksums WRITE setAlwaysCompareChecksums)
		Q_PROPERTY(bool preserveDevices READ preserveDevices WRITE setPreserveDevices)
		Q_PROPERTY(bool keepPartialTransfers READ keepPartialTransfers WRITE setKeepPartialTransfers)
		Q_PROPERTY(bool copySymlinksAsSymlinks READ copySymlinksAsSymlinks WRITE setCopySymlinksAsSymlinks)
		Q_PROPERTY(bool makeBackups READ makeBackups WRITE setMakeBackups)

		Q_PROPERTY(bool useTransferCompression READ useTransferCompression WRITE setUseTransferCompression)
		Q_PROPERTY(bool onlyUpdateExistingEntries READ onlyUpdateExistingEntries WRITE setOnlyUpdateExistingEntries)
		Q_PROPERTY(bool dontUpdateExistingEntries READ dontUpdateExistingEntries WRITE setDontUpdateExistingEntries)
		Q_PROPERTY(bool dontMapUsersAndGroups READ dontMapUsersAndGroups WRITE setDontMapUsersAndGroups)
		Q_PROPERTY(bool copyHardlinksAsHardlinks READ copyHardlinksAsHardlinks WRITE setCopyHardlinksAsHardlinks)
		Q_PROPERTY(bool showItemisedChanges READ showItemisedChanges WRITE setShowItemisedChanges)

		Q_PROPERTY(QString logFile READ logFile WRITE setLogFile)

	private:
		mutable bool m_modified;
		QString m_fileName;

		QString m_name;

		QString m_source;
		QString m_dest;

		bool m_preserveTime;
		bool m_preservePerms;
		bool m_preserveOwner;
		bool m_preserveGroup;

		bool m_windowsCompatability;
		bool m_deleteOnDestination;

		bool m_alwaysChecksum;
		bool m_preserveDevices;
		bool m_keepParitalTransfers;
		bool m_symlinksAsSymlinks;
		bool m_makeBackups;

		bool m_compressInTransit;
		bool m_onlyUpdateExisting;
		bool m_dontUpdateExisting;
		bool m_dontMapUidGid;
		bool m_copyHardlinksAsHardlinks;
		bool m_showItemisedChanges;

		QString m_logFile;

		/**
		 * \brief Set all settings to default values.
		 */
		void setDefaultProperties(void);

	protected:
		/**
		 * \brief Write the preset to an XML stream.
		 *
		 * \param xml is the stream to write to.
		 *
		 * \return \b true if the preset was written to the stream, \b false
		 * otherwise.
		 */
		bool emitXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write the name of the preset to an XML stream.
		 *
		 * \param xml is the stream to write to.
		 *
		 * \return \b true if the name of the preset was written to the stream,
		 * \b false otherwise.
		 */
		bool emitNameXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write the preset source to an XML stream.
		 *
		 * \param xml is the stream to write to.
		 *
		 * \return \b true if the preset source was written to the stream,
		 * \b false otherwise.
		 */
		bool emitSourceXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write the preset destination to an XML stream.
		 *
		 * \param xml is the stream to write to.
		 *
		 * \return \b true if the preset destination was written to the stream,
		 * \b false otherwise.
		 */
		bool emitDestinationXml(QXmlStreamWriter & xml) const;

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
		bool emitPropertiesXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Read the preset from an XML stream.
		 *
		 * \param xml is the stream to read from.
		 *
		 * \return \c true if the preset was read from the stream, \b false
		 * otherwise.
		 */
		bool parseXml(QXmlStreamReader & xml);

		/**
		 * \brief Read the properties for the preset from an XML stream.
		 *
		 * \param xml is the stream to read from.
		 *
		 * \return \c true if the properties for the preset were read from the
		 * stream, \b false otherwise.
		 */
		bool parsePropertiesXml(QXmlStreamReader & xml);

		/**
		 * \brief Read an individual property for the preset from an XML stream.
		 *
		 * \param xml is the stream to read from.
		 *
		 * \return \c true if the property for the preset was read from the
		 * stream, \b false otherwise.
		 */
		bool parsePropertyXml(QXmlStreamReader & xml);

	public:
		/**
		 * \brief Create a new Preset.
		 *
		 * \param name is the name for the new preset.
		 */
		explicit Preset(const QString & name = QString());

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
		static Preset * load(const QString & fileName);

		/**
		 * \brief Save the preset to its internally stored file name.
		 *
		 * \return \b true if the preset file was saved, \b false otherwise.
		 */
		bool save(void) const;

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
		bool saveAs(const QString & fileName);

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
		bool saveCopyAs(const QString & fileName) const;

		/**
		 * \brief Set the name of the preset.
		 *
		 * \param name is the new name for the preset.
		 *
		 * Presets do not need to have unique names.
		 */
		void setName(const QString & name);

		/**
		 * \brief Get the name of the preset.
		 *
		 * \return The name of the preset.
		 */
		QString name(void) const;

		/**
		 * \brief Set the file name of the preset.
		 *
		 * \param fileName is the new path to the file for the preset.
		 *
		 * Setting the file name does not automatically save the preset.
		 */
		void setFileName(const QString & fileName);

		/**
		 * \brief Get the file name of the preset.
		 *
		 * \return The current path to the file for the preset.
		 */
		QString fileName(void) const;

		/**
		 * \brief Set the source file or directory.
		 *
		 * \param source is the path to the source file or directory.
		 *
		 * \return \b true if the source was set, \b false otherwise.
		 */
		bool setSource(const QString & source);

		/**
		 * \brief Set the destination file or directory.
		 *
		 * \param dest is the path to the destination file or directory.
		 *
		 * \return \b true if the destination was set, \b false otherwise.
		 */
		bool setDestination(const QString & dest);

		/**
		 * \brief Set whether or not file timestamps should be preserved.
		 *
		 * \param preserve indicates whether the timestamps should be preserved.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setPreserveTime(bool preserve);

		/**
		 * \brief Set whether or not file permissions should be preserved.
		 *
		 * \param preserve indicates whether the permissions should be
		 * preserved.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setPreservePermissions(bool preserve);

		/**
		 * \brief Set whether or not file ownership should be preserved.
		 *
		 * \param preserve indicates whether the ownership should be preserved.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setPreserveOwner(bool preserve);

		/**
		 * \brief Set whether or not file group ownership should be preserved.
		 *
		 * \param preserve indicates whether the group ownership should be
		 * preserved.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setPreserveGroup(bool preserve);

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
		bool setWindowsCompatability(bool compatible);

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
		bool setHonourDeletions(bool honour);

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
		bool setAlwaysCompareChecksums(bool checksums);

		/**
		 * \brief Set whether or not device files should be preserved as such.
		 *
		 * \param preserve indicates whether the device files should be
		 * preserved.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setPreserveDevices(bool preserve);

		/**
		 * \brief Set whether or not partially-transferred items should be kept.
		 *
		 * \param keep indicates whether partially-transferred items should be
		 * kept.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setKeepPartialTransfers(bool keep);

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
		bool setCopySymlinksAsSymlinks(bool links);

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
		bool setMakeBackups(bool backup);

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
		bool setUseTransferCompression(bool compress);

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
		bool setOnlyUpdateExistingEntries(bool onlyExisting);

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
		bool setDontUpdateExistingEntries(bool noExisting);

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
		bool setDontMapUsersAndGroups(bool dontMap);

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
		bool setCopyHardlinksAsHardlinks(bool links);

		/**
		 * \brief Set whether or not an itemised list of changes should be
		 * generated.
		 *
		 * \param itemise indicates whether an itemised list should be
		 * generated.
		 *
		 * \return \b true if the setting was set, \b false otherwise.
		 */
		bool setShowItemisedChanges(bool itemise);

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
		bool setLogFile(const QString & fileName);

		/**
		 * \brief Get the source location.
		 *
		 * \return The source.
		 */
		QString source(void) const;

		/**
		 * \brief Get the destination location.
		 *
		 * \return The destination.
		 */
		QString destination(void) const;

		/**
		 * \brief Get whether file timestamps should be preserved.
		 *
		 * \return \b true if timestamps should be preserved, \b false
		 * otherwise.
		 */
		bool preserveTime(void) const;

		/**
		 * \brief Get whether file permissions should be preserved.
		 *
		 * \return \b true if permissions should be preserved, \b false
		 * otherwise.
		 */
		bool preservePermissions(void) const;

		/**
		 * \brief Get whether file ownership should be preserved.
		 *
		 * \return \b true if ownership should be preserved, \b false
		 * otherwise.
		 */
		bool preserveOwner(void) const;

		/**
		 * \brief Get whether file group ownership should be preserved.
		 *
		 * \return \b true if group ownership should be preserved, \b false
		 * otherwise.
		 */
		bool preserveGroup(void) const;

		/**
		 * \brief Get whether Windows compatability should be used.
		 *
		 * \return \b true if Windows compatability should be used, \b false
		 * otherwise.
		 */
		bool windowsCompatability(void) const;

		/**
		 * \brief Get whether source tree deletions should be honoured.
		 *
		 * \return \b true if source tree deletions should be honoured, \b false
		 * otherwise.
		 */
		bool honourDeletions(void) const;

		/**
		 * \brief Get whether entry checksums should always be used.
		 *
		 * \return \b true if entry checksums should always be used, \b false
		 * otherwise.
		 */
		bool alwaysCompareChecksums(void) const;

		/**
		 * \brief Get whether or device files should be preserved.
		 *
		 * \return \b true if device files should be preserved, \b false
		 * otherwise.
		 */
		bool preserveDevices(void) const;

		/**
		 * \brief Get whether partial transfers should be kept.
		 *
		 * \return \b true if partial transfers should be kept, \b false
		 * otherwise.
		 */
		bool keepPartialTransfers(void) const;

		/**
		 * \brief Get whether symbolic links should be copied as symbolic
		 * links.
		 *
		 * \return \b true if symbolic links should be copied as symbolic links,
		 * \b false otherwise.
		 */
		bool copySymlinksAsSymlinks(void) const;

		/**
		 * \brief Get whether destination tree backups should be made.
		 *
		 * \return \b true if destination tree backups should be made, \b false
		 * otherwise.
		 */
		bool makeBackups(void) const;

		/**
		 * \brief Get whether transfer compression should be used.
		 *
		 * \return \b true if transfer compression should be used, \b false
		 * otherwise.
		 */
		bool useTransferCompression(void) const;

		/**
		 * \brief Get whether only existing destination entries should be
		 * updated.
		 *
		 * \return \b true if only existing destination should be updated,
		 * \b false otherwise.
		 */
		bool onlyUpdateExistingEntries(void) const;

		/**
		 * \brief Get whether entries that already exist on the destination
		 * should be ignored.
		 *
		 * \return \b true if entries that already exist on the destination
		 * will be ignored.
		 */
		bool dontUpdateExistingEntries(void) const;

		/**
		 * \brief Get whether UID and GID mapping should be suppressed.
		 *
		 * \return \b true if UID and GID mapping should be suppressed, \b false
		 * otherwise.
		 */
		bool dontMapUsersAndGroups(void) const;

		/**
		 * \brief Get whether hard links should be copied as hard links.
		 *
		 * \return \b true if hard links should be copied as hard links,
		 * \b false otherwise.
		 */
		bool copyHardlinksAsHardlinks(void) const;

		/**
		 * \brief Get whether a list of itemised changes should be generated.
		 *
		 * \return \b true if a list of itemised changes should be generated,
		 * \b false
		 * otherwise.
		 */
		bool showItemisedChanges(void) const;

		/**
		 * \brief Get the log file.
		 *
		 * \return The path to the log file.
		 */
		QString logFile(void) const;
	};

}  // namespace Qync

#endif  // QYNC_PRESET_H
