/**
 * \file Preferences.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Declaration of the Preferences class.
 *
 * \dep
 * - QString
 * - QXmlStreamWriter
 * - QXmlStreamReader
 */

#ifndef QYNC_PREFERENCES_H
#define QYNC_PREFERENCES_H

#include <QString>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Qync {

	class Preferences {
	public:
		explicit Preferences(const QString & fileName = QString());
		virtual ~Preferences(void) = default;

		bool save(void) const;
		bool saveAs(const QString & fileName);
		bool saveCopyAs(const QString & fileName) const;

		bool load(void);
		bool loadFrom(const QString & fileName);

		inline QString rsyncPath(void) const {
			return m_rsyncBinary;
		}

		inline bool setRsyncPath(const QString & path) {
			m_rsyncBinary = path;
			return true;
		}

		bool rsyncPathIsValid(void) const;

	protected:
		static bool parseBooleanText(const QString & b, bool * ok = nullptr);
		virtual void setDefaults(void);
		virtual bool parseXmlStream(QXmlStreamReader & xml);
		virtual bool parseXmlElement(QXmlStreamReader & xml);
		bool parseCorePreferencesXml(QXmlStreamReader & xml);
		virtual bool emitXmlStream(QXmlStreamWriter & xml) const;
		virtual bool emitXmlElements(QXmlStreamWriter & xml) const;
		bool emitCorePreferencesXml(QXmlStreamWriter & xml) const;

	private:
		mutable bool m_modified;
		QString m_fileName;
		QString m_rsyncBinary;
	};

}  // namespace Qync

#endif  // QYNC_PREFERENCES_H
