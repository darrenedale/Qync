/**
 * \file preferences.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the Preferences class.
 *
 * \dep
 * - QString
 */

#ifndef QYNC_PREFERENCES_H
#define QYNC_PREFERENCES_H

#include <QString>

#if __cplusplus >= 201703L

#include <optional>

namespace Qync {
	using std::optional;
}

#else

#include <equit/optional>

namespace Qync {
	using Equit::optional;
}

#endif

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Qync {

	class Preferences {
	public:
		Preferences();
		explicit Preferences(const QString & fileName);
		virtual ~Preferences() = default;

		bool save() const;
		bool saveAs(const QString & fileName);
		bool saveCopyAs(const QString & fileName) const;

		bool load();
		bool loadFrom(const QString & fileName);

		inline QString rsyncPath() const {
			return m_rsyncBinary;
		}

		inline bool setRsyncPath(const QString & path) {
			m_rsyncBinary = path;
			return true;
		}

		bool rsyncPathIsValid() const;

	protected:
		static optional<bool> parseBooleanText(const QString & boolString);
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
