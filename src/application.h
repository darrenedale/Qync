/**
 * \file application.h
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Declaration of the Application class.
 *
 * \dep
 * - memory
 * - vector
 * - QApplication
 * - QString
 */

#ifndef QYNC_APPLICATION_H
#define QYNC_APPLICATION_H

#include <memory>
#include <vector>

#include <QApplication>
#include <QString>

#include "guipreferences.h"

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

		inline GuiPreferences & preferences(void) {
			return m_prefs;
		}

		inline int presetCount(void) const {
			return static_cast<int>(presets().size());
		}

		const PresetList & presets(void) const {
			return m_presets;
		}

		Preset & preset(int index);
		bool removePreset(int index);
		Preset & addPreset(const QString & name);
		bool loadPreset(const QString & fileName);

		void clearPresets(void);

		QString lastError(void) const;

		bool loadPresets(void);
		bool loadPresets(const QString & path);

	Q_SIGNALS:
		void presetsChanged(void);
		void preferencesChanged(void);

	protected:
		void setLastError(const QString & err) const;

	private:
		QString m_rsyncVersionText;
		QString m_configPath;
		QString m_presetsPath;

		PresetList m_presets;
		GuiPreferences m_prefs;

		mutable QString m_lastError;
	};

}  // namespace Qync

#define qyncApp (static_cast<Qync::Application *>(QApplication::instance()))

#endif  // QYNC_APPLICATION_H
