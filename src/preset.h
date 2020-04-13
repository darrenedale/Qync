/**
 * @file preset.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the Preset class.
 *
 * In an ideal world the implementation here would be de-coupled from Qt.
 * Moving to std::string would be simple, but the use of Qt's XML reading
 * and writing makes it harder. It would be possible to use a different
 * XML handling library, but this would just remove one dependency for
 * another, which is of little utility since the UI uses Qt anyway. The
 * other alternatives are to:
 * - implement our own XML parser
 * - use a different format for storing presets for which it is simpler
 *   to implement a parser
 * The former option would be time-consuming and error prone for little
 * gain, since the Qt XML parsing code works well; the latter is a better
 * option since it would enable removal of the Qt dependency without
 * introducing another and the data to store is not particularly complex.
 * INI style files would be possible.
 */

#ifndef QYNC_PRESET_H
#define QYNC_PRESET_H

#include <QtCore/QString>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Qync {

	class Preset final {
	public:
		explicit Preset(const QString & = QStringLiteral());
		Preset(const Preset &) = delete;
		Preset(Preset &&) = default;

		void operator=(const Preset &) = delete;
		Preset & operator=(Preset &&) = default;

		~Preset();

		void setDefaults();
		bool load(const QString &);

		inline bool save() const {
			return saveCopyAs(m_fileName);
		}

		bool saveAs(const QString &);
		bool saveCopyAs(const QString &) const;

		void setName(const QString &);

		[[nodiscard]] inline const QString & name() const {
			return m_name;
		}

		void setFileName(const QString &);

		[[nodiscard]] inline const QString & fileName() const {
			return m_fileName;
		}

		bool setSource(const QString &);
		bool setDestination(const QString &);
		bool setPreserveTime(const bool &);
		bool setPreservePermissions(const bool &);
		bool setPreserveOwner(const bool &);
		bool setPreserveGroup(const bool &);
		bool setWindowsCompatability(const bool &);
		bool setHonourDeletions(const bool &);
		bool setAlwaysCompareChecksums(const bool &);
		bool setIgnoreTimes(const bool &);
		bool setPreserveDevices(const bool &);
		bool setKeepPartialTransfers(const bool &);
		bool setCopySymlinksAsSymlinks(const bool &);
		bool setMakeBackups(const bool &);
		bool setUseTransferCompression(const bool &);
		bool setOnlyUpdateExistingEntries(const bool &);
		bool setDontUpdateExistingEntries(const bool &);
		bool setDontMapUsersAndGroups(const bool &);
		bool setCopyHardlinksAsHardlinks(const bool &);
		bool setShowItemisedChanges(const bool &);
		bool setLogFile(const QString &);

		[[nodiscard]] inline const QString & source() const {
			return m_source;
		}

		[[nodiscard]] inline const QString & destination() const {
			return m_dest;
		}

		[[nodiscard]] inline const bool & preserveTime() const {
			return m_preserveTime;
		}

		[[nodiscard]] inline const bool & preservePermissions() const {
			return m_preservePerms;
		}

		[[nodiscard]] inline const bool & preserveOwner() const {
			return m_preserveOwner;
		}

		[[nodiscard]] inline const bool & preserveGroup() const {
			return m_preserveGroup;
		}

		[[nodiscard]] inline const bool & windowsCompatability() const {
			return m_windowsCompatability;
		}

		[[nodiscard]] inline const bool & honourDeletions() const {
			return m_deleteOnDestination;
		}

		[[nodiscard]] inline const bool & alwaysCompareChecksums() const {
			return m_alwaysChecksum;
		}

		[[nodiscard]] inline const bool & ignoreTimes() const {
			return m_ignoreTimes;
		}

		[[nodiscard]] inline const bool & preserveDevices() const {
			return m_preserveDevices;
		}

		[[nodiscard]] inline const bool & keepPartialTransfers() const {
			return m_keepParitalTransfers;
		}

		[[nodiscard]] inline const bool & copySymlinksAsSymlinks() const {
			return m_symlinksAsSymlinks;
		}

		[[nodiscard]] inline const bool & makeBackups() const {
			return m_makeBackups;
		}

		[[nodiscard]] inline const bool & useTransferCompression() const {
			return m_compressInTransit;
		}

		[[nodiscard]] inline const bool & onlyUpdateExistingEntries() const {
			return m_onlyUpdateExisting;
		}

		[[nodiscard]] inline const bool & dontUpdateExistingEntries() const {
			return m_dontUpdateExisting;
		}

		[[nodiscard]] inline const bool & dontMapUsersAndGroups() const {
			return m_dontMapUidGid;
		}

		[[nodiscard]] inline const bool & copyHardlinksAsHardlinks() const {
			return m_copyHardlinksAsHardlinks;
		}

		[[nodiscard]] inline const bool & showItemisedChanges() const {
			return m_showItemisedChanges;
		}

		inline const QString & logFile() const {
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
		bool m_ignoreTimes;
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
