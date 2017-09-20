/**
 * \file preset.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.5
 *
 * \brief Declaration of the Preset class.
 *
 * \dep
 * - QObject
 * - QString
 * - QXmlStreamReader
 * - QXmlStreamWriter
 */

#ifndef QYNC_PRESET_H
#define QYNC_PRESET_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Qync {

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

	public:
		explicit Preset(const QString & name = QString());

		static Preset * load(const QString & fileName);

		inline bool save(void) const {
			return saveCopyAs(m_fileName);
		}

		bool saveAs(const QString & fileName);
		bool saveCopyAs(const QString & fileName) const;

		void setName(const QString & name);

		inline QString name(void) const {
			return m_name;
		}

		void setFileName(const QString & fileName);

		inline QString fileName(void) const {
			return m_fileName;
		}

		bool setSource(const QString & source);
		bool setDestination(const QString & dest);
		bool setPreserveTime(bool preserve);
		bool setPreservePermissions(bool preserve);
		bool setPreserveOwner(bool preserve);
		bool setPreserveGroup(bool preserve);
		bool setWindowsCompatability(bool compatible);
		bool setHonourDeletions(bool honour);
		bool setAlwaysCompareChecksums(bool checksums);
		bool setPreserveDevices(bool preserve);
		bool setKeepPartialTransfers(bool keep);
		bool setCopySymlinksAsSymlinks(bool links);
		bool setMakeBackups(bool backup);
		bool setUseTransferCompression(bool compress);
		bool setOnlyUpdateExistingEntries(bool onlyExisting);
		bool setDontUpdateExistingEntries(bool noExisting);
		bool setDontMapUsersAndGroups(bool dontMap);
		bool setCopyHardlinksAsHardlinks(bool links);
		bool setShowItemisedChanges(bool itemise);
		bool setLogFile(const QString & fileName);

		inline QString source(void) const {
			return m_source;
		}

		inline QString destination(void) const {
			return m_dest;
		}

		inline bool preserveTime(void) const {
			return m_preserveTime;
		}

		inline bool preservePermissions(void) const {
			return m_preservePerms;
		}

		inline bool preserveOwner(void) const {
			return m_preserveOwner;
		}

		inline bool preserveGroup(void) const {
			return m_preserveGroup;
		}

		inline bool windowsCompatability(void) const {
			return m_windowsCompatability;
		}

		inline bool honourDeletions(void) const {
			return m_deleteOnDestination;
		}

		inline bool alwaysCompareChecksums(void) const {
			return m_alwaysChecksum;
		}

		inline bool preserveDevices(void) const {
			return m_preserveDevices;
		}

		inline bool keepPartialTransfers(void) const {
			return m_keepParitalTransfers;
		}

		inline bool copySymlinksAsSymlinks(void) const {
			return m_symlinksAsSymlinks;
		}

		inline bool makeBackups(void) const {
			return m_makeBackups;
		}

		inline bool useTransferCompression(void) const {
			return m_compressInTransit;
		}

		inline bool onlyUpdateExistingEntries(void) const {
			return m_onlyUpdateExisting;
		}

		inline bool dontUpdateExistingEntries(void) const {
			return m_dontUpdateExisting;
		}

		inline bool dontMapUsersAndGroups(void) const {
			return m_dontMapUidGid;
		}

		inline bool copyHardlinksAsHardlinks(void) const {
			return m_copyHardlinksAsHardlinks;
		}

		inline bool showItemisedChanges(void) const {
			return m_showItemisedChanges;
		}

		inline QString logFile(void) const {
			return m_logFile;
		}

	protected:
		bool emitXml(QXmlStreamWriter & xml) const;
		bool emitNameXml(QXmlStreamWriter & xml) const;
		bool emitSourceXml(QXmlStreamWriter & xml) const;
		bool emitDestinationXml(QXmlStreamWriter & xml) const;
		bool emitPropertiesXml(QXmlStreamWriter & xml) const;

		bool parseXml(QXmlStreamReader & xml);
		bool parsePropertiesXml(QXmlStreamReader & xml);
		bool parsePropertyXml(QXmlStreamReader & xml);

	private:
		void setDefaultProperties(void);

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
	};

}  // namespace Qync

#endif  // QYNC_PRESET_H
