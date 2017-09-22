/**
 * \file preset.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.7
 *
 * \brief Declaration of the Preset class.
 *
 * \dep
 * - QObject
 * - QString
 */

#ifndef QYNC_PRESET_H
#define QYNC_PRESET_H

#include <QObject>
#include <QString>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Qync {

	class Preset {
	public:
		explicit Preset(const QString & name = QString());
		Preset(const Preset & other) = default;
		Preset(Preset && other) = default;

		Preset & operator=(const Preset & other) = default;
		Preset & operator=(Preset && other) = default;

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
		bool setPreserveTime(const bool & preserve);
		bool setPreservePermissions(const bool & preserve);
		bool setPreserveOwner(const bool & preserve);
		bool setPreserveGroup(const bool & preserve);
		bool setWindowsCompatability(const bool & compatible);
		bool setHonourDeletions(const bool & honour);
		bool setAlwaysCompareChecksums(const bool & checksums);
		bool setPreserveDevices(const bool & preserve);
		bool setKeepPartialTransfers(const bool & keep);
		bool setCopySymlinksAsSymlinks(const bool & links);
		bool setMakeBackups(const bool & backup);
		bool setUseTransferCompression(const bool & compress);
		bool setOnlyUpdateExistingEntries(const bool & onlyExisting);
		bool setDontUpdateExistingEntries(const bool & noExisting);
		bool setDontMapUsersAndGroups(const bool & dontMap);
		bool setCopyHardlinksAsHardlinks(const bool & links);
		bool setShowItemisedChanges(const bool & itemise);
		bool setLogFile(const QString & fileName);

		inline QString source(void) const {
			return m_source;
		}

		inline QString destination(void) const {
			return m_dest;
		}

		inline const bool & preserveTime(void) const {
			return m_preserveTime;
		}

		inline const bool & preservePermissions(void) const {
			return m_preservePerms;
		}

		inline const bool & preserveOwner(void) const {
			return m_preserveOwner;
		}

		inline const bool & preserveGroup(void) const {
			return m_preserveGroup;
		}

		inline const bool & windowsCompatability(void) const {
			return m_windowsCompatability;
		}

		inline const bool & honourDeletions(void) const {
			return m_deleteOnDestination;
		}

		inline const bool & alwaysCompareChecksums(void) const {
			return m_alwaysChecksum;
		}

		inline const bool & preserveDevices(void) const {
			return m_preserveDevices;
		}

		inline const bool & keepPartialTransfers(void) const {
			return m_keepParitalTransfers;
		}

		inline const bool & copySymlinksAsSymlinks(void) const {
			return m_symlinksAsSymlinks;
		}

		inline const bool & makeBackups(void) const {
			return m_makeBackups;
		}

		inline const bool & useTransferCompression(void) const {
			return m_compressInTransit;
		}

		inline const bool & onlyUpdateExistingEntries(void) const {
			return m_onlyUpdateExisting;
		}

		inline const bool & dontUpdateExistingEntries(void) const {
			return m_dontUpdateExisting;
		}

		inline const bool & dontMapUsersAndGroups(void) const {
			return m_dontMapUidGid;
		}

		inline const bool & copyHardlinksAsHardlinks(void) const {
			return m_copyHardlinksAsHardlinks;
		}

		inline const bool & showItemisedChanges(void) const {
			return m_showItemisedChanges;
		}

		inline const QString & logFile(void) const {
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

		//		mutable bool m_modified;
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
