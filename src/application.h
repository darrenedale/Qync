/**
 * \file application.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
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
#include "mainwindow.h"

namespace Qync {

	class Preset;
	class Process;
	class Preferences;

	class Application
	: public QApplication {
		Q_OBJECT

	public:
		using PresetList = std::vector<std::unique_ptr<Preset>>;

		enum class NotificationType {
			Information = 0,
			Warning,
			Error
		};

		Application(int & argc, char ** argv);
		~Application();

		static int exec();

		QString configurationPath();
		QString presetsPath();
		QString rsyncVersionText();

		inline QString buildId() const {
			return property("BuildId").toString();
		}

		inline QString releaseDate() const {
			return property("ReleaseDate").toString();
		}

		inline QString websiteUrl() const {
			return property("ApplicationWebsite").toString();
		}

		inline GuiPreferences & preferences() {
			return m_prefs;
		}

		inline int presetCount() const {
			return static_cast<int>(presets().size());
		}

		const PresetList & presets() const {
			return m_presets;
		}

		Preset & preset(int index);
		bool removePreset(int index);
		Preset & addPreset(const QString & name);
		bool loadPreset(const QString & fileName);

		void clearPresets();

		QString lastError() const;

		bool loadPresets();

		MainWindow * mainWindow() const {
			return m_mainWindow.get();
		}

	Q_SIGNALS:
		void presetsChanged();
		void preferencesChanged();

	protected:
		void setLastError(const QString & err) const;

	private:
		QString m_rsyncVersionText;
		QString m_configPath;
		QString m_presetsPath;

		PresetList m_presets;
		GuiPreferences m_prefs;

		std::unique_ptr<MainWindow> m_mainWindow;

		mutable QString m_lastError;
	};

}  // namespace Qync

#define qyncApp (dynamic_cast<Qync::Application *>(QApplication::instance()))

#endif  // QYNC_APPLICATION_H
