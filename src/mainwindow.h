/**
 * \file mainwindow.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the MainWindow class.
 *
 * \dep
 * - memory
 * - QMainWindow
 */

#ifndef QYNC_MAINWINDOW_H
#define QYNC_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include "types.h"

namespace Qync {

	namespace Ui {
		class MainWindow;
	}

	class Application;
	class Preset;
	class PreferencesDialogue;
	class AboutDialogue;
	class Process;

	class MainWindow
	: public QMainWindow {
		Q_OBJECT

	public:
		MainWindow();
		virtual ~MainWindow();

		void showNotification(const QString &, const QString &, NotificationType = NotificationType::Information);

	public Q_SLOTS:
		//		void refreshPresets();
		void switchSourceAndDestination();

		void saveSettingsToCurrentPreset();
		void removeCurrentPreset();
		void newPresetFromSettings();
		void newPreset(bool fill = false);
		void importPreset();
		void exportPreset();

		void chooseLogFile();

		void useSimpleUi(bool);
		void showPreferences();

		void simulate();
		void synchronise();

		void about();
		void aboutRsync();

	private Q_SLOTS:
		void showPreset(const Preset &);
		void onPreferencesChanged();

	protected:
		void disconnectApplication();
		void connectApplication();

	private:
		void fillPreset(Preset &) const;
		bool runProcess(std::shared_ptr<Process> &);

		std::unique_ptr<Ui::MainWindow> m_ui;
		std::unique_ptr<PreferencesDialogue> m_prefsWindow;
		std::unique_ptr<AboutDialogue> m_aboutDialogue;
	};

}  // namespace Qync

#endif  // QYNC_MAINWINDOW_H
