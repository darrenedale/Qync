/**
 * \file mainwindow.h
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
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
		MainWindow(void);
		virtual ~MainWindow(void);

	public Q_SLOTS:
		//		void refreshPresets(void);
		void switchSourceAndDestination(void);

		void saveSettingsToCurrentPreset(void);
		void removeCurrentPreset(void);
		void newPresetFromSettings(void);
		void newPreset(bool fill = false);
		void importPreset(void);
		void exportPreset(void);

		void chooseLogFile(void);

		void useSimpleUi(bool);
		void showPreferences(void);

		void simulate(void);
		void synchronise(void);

		void about(void);
		void aboutRsync(void);

	private Q_SLOTS:
		void showPreset(const Preset & preset);
		void onPreferencesChanged(void);

	protected:
		void disconnectApplication(void);
		void connectApplication(void);

	private:
		static const int UpdateEverything;
		static const int OnlyUpdateExisting;
		static const int DontUpdateExisting;

		void fillPreset(Preset &) const;
		bool runProcess(std::shared_ptr<Process> &);

		std::unique_ptr<Ui::MainWindow> m_ui;
		std::unique_ptr<PreferencesDialogue> m_prefsWindow;
		std::unique_ptr<AboutDialogue> m_aboutDialogue;
	};

}  // namespace Qync

#endif  // QYNC_MAINWINDOW_H
