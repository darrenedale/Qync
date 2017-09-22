/**
 * \file application.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Declaration of the Application class.
 *
 * \dep
 * - memory
 * - QApplication
 * - QList
 * - QString
 */

#ifndef QYNC_APPLICATION_H
#define QYNC_APPLICATION_H

#include <memory>

#include <QApplication>
#include <QList>
#include <QString>

namespace Qync {

	class Preset;
	class Process;
	class Preferences;

	class Application
	: public QApplication {
		Q_OBJECT

	public:
		using PresetList = std::vector<std::unique_ptr<Preset>>;

		Application(int & argc, char ** argv);
		~Application(void);

		static int exec(void);

		QString configurationPath(void);
		QString presetsPath(void);
		QString rsyncVersionText(void);

		inline QString buildId(void) const {
			return property("BuildId").toString();
		}

		inline QString releaseDate(void) const {
			return property("ReleaseDate").toString();
		}

		inline QString websiteUrl(void) const {
			return property("ApplicationWebsite").toString();
		}

		const Preferences * preferences(void) const;
		bool setPreferences(Preferences * prefs);

		inline int presetCount(void) const {
			return static_cast<int>(m_presets.size());
		}

		const PresetList & presets(void) const {
			return m_presets;
		}

		Preset & preset(int index) const;
		bool removePreset(int index);
		bool addPreset(Preset * preset);

		void clearPresets(void);

		Process * simulate(int i) const;
		Process * simulate(const Preset & preset) const;
		Process * synchronise(int i) const;
		Process * synchronise(const Preset & preset) const;

		QString lastError(void) const;

	public Q_SLOTS:
		bool loadPresets(void);
		bool loadPresets(const QString & path);

	Q_SIGNALS:
		//		void presetAdded(Preset * preset);
		void presetRemoved(void);
		void presetsChanged(void);
		void preferencesChanged(void);
		void processStarted(Process * process) const;

	protected:
		/**
		 * \brief Set the text that describes the last error.
		 *
		 * \param err is the error description.
		 *
		 * Methods that indicate success or failure must use this method if
		 * they indicate failure so that the end user can be presented with a
		 * description of what went wrong. They need not reset it if they
		 * succeed as long as the clearly indicate success.
		 */
		void setLastError(const QString & err) const;

	private:
		QString m_rsyncVersionText;
		QString m_configPath;
		QString m_presetsPath;

		PresetList m_presets;

		std::unique_ptr<Preferences> m_prefs;

		mutable QString m_lastError;
	};

}  // namespace Qync

#define qyncApp (static_cast<Qync::Application *>(QApplication::instance()))

#endif  // QYNC_APPLICATION_H
