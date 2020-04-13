/**
 * @file preferences.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the Preferences class.
 */

#ifndef QYNC_PREFERENCES_H
#define QYNC_PREFERENCES_H

#include <QtCore/QString>
#include <optional>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Qync {

	class Preferences {
	public:
		Preferences();
		explicit Preferences(QString);
		virtual ~Preferences() = default;

		bool save() const;
		bool saveAs(const QString &);
		bool saveCopyAs(const QString &) const;

		bool load();
		bool loadFrom(const QString & fileName);

		[[nodiscard]] inline QString rsyncPath() const {
			return m_rsyncBinary;
		}

		inline void setRsyncPath(const QString & path) {
			m_rsyncBinary = path;
		}

		[[nodiscard]] bool rsyncPathIsValid() const;

	protected:
		virtual void setDefaults();
		virtual bool parseXmlStream(QXmlStreamReader & xml);
		virtual bool parseXmlElement(QXmlStreamReader & xml);
		bool parseCorePreferencesXml(QXmlStreamReader & xml);
		virtual bool emitXmlStream(QXmlStreamWriter & xml) const;
		virtual bool emitXmlElements(QXmlStreamWriter & xml) const;
		bool emitCorePreferencesXml(QXmlStreamWriter & xml) const;

	private:
		QString m_fileName;
		QString m_rsyncBinary;
	};

}  // namespace Qync

#endif  // QYNC_PREFERENCES_H
