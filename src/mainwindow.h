/**
 * \file QyncMainWindow.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Declaration of the QyncMainWindow class.
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

	class Preset;
	class PreferencesDialogue;
	class AboutDialogue;

	class MainWindow
	  : public QMainWindow {
		Q_OBJECT

	public:
		explicit MainWindow(void);
		virtual ~MainWindow(void);

	public Q_SLOTS:
		void refreshPresets(void);

		/**
		 * \brief Choose a local directory for the rsync source.
		 *
		 * A local directory browser is presented for the user to to choose a
		 * source for rsync. If the user does not cancel the dialogue, the
		 * chosen directory is set as the text in the source line edit.
		 */
		void chooseSourceDirectory(void);

		/**
		 * \brief Choose a local directory for the rsync destination.
		 *
		 * A local directory browser is presented for the user to to choose a
		 * destination for rsync. If the user does not cancel the dialogue, the
		 * chosen directory is set as the text in the destination line edit.
		 */
		void chooseDestinationDirectory(void);

		/**
		 * \brief Choose a local file for the rsync source.
		 *
		 * A local file browser is presented for the user to to choose a source
		 * for rsync. If the user does not cancel the dialogue, the chosen file
		 * is set as the text in the source line edit.
		 */
		void chooseSourceFile(void);

		/**
		 * \brief Choose a local file for the rsync destination.
		 *
		 * A local file browser is presented for the user to to choose a
		 * destination for rsync. If the user does not cancel the dialogue, the
		 * chosen file is set as the text in the destination line edit.
		 */
		void chooseDestinationFile(void);

		/**
		 * \brief Choose a local file for the rsycn output log.
		 *
		 * A local file browser is presented for the user to to choose a log
		 * file. If the user does not cancel the dialogue, the chosen file is
		 * set as the text in the log file line edit.
		 */
		void chooseLogFile(void);

		/**
		 * \brief Switch the content of the source and destination line edits.
		 */
		void switchSourceAndDestination(void);

		void saveSettingsToCurrentPreset(void);

		void removeCurrentPreset(void);
		void newPresetFromSettings(void);
		void newPreset(bool fill = false);

		/**
		 * \brief Import a preset from a file.
		 *
		 * A file dialogue is presented for the user to choose a file from which
		 * to import a preset. If the user does not cancel the dialogue and the
		 * file s/he chooses is a valid preset file, the preset is added to
		 * the manager. Any failures to do so, other than the user cancelling
		 * the dialogue, are reported to the user.
		 */
		void importPreset(void);

		/**
		 * \brief Export a preset to a file.
		 *
		 * A file dialogue is presented for the user to choose a file to which
		 * to export a preset. If the user does not cancel the dialogue, the
		 * current settings are saved to the selected file. Any failures to do
		 * so, other than the user cancelling the dialogue, are reported to the
		 * user.
		 *
		 * \note It is the current settings \b not the current preset that is
		 * exported. The two could be different if the user has selected a
		 * preset and altered the settings without saving the settings back to
		 * selected preset.
		 */
		void exportPreset(void);

		void showPreferences(void);

		void simulate(void);
		void synchronise(void);

		void about(void);
		void aboutRsync(void);

	private Q_SLOTS:
		void showPresetFromMenu(void);
		void showPreset(int index);
		void readPreferences(void);

	protected:
		void disconnectApplication(void);
		void connectApplication(void);

	private:
		static const int UpdateEverything;
		static const int OnlyUpdateExisting;
		static const int DontUpdateExisting;

		void fillPreset(Preset &) const;

		std::unique_ptr<Ui::MainWindow> m_ui;
		std::unique_ptr<PreferencesDialogue> m_prefsWindow;
		std::unique_ptr<AboutDialogue> m_aboutDialogue;
	};

}  // namespace Qync

#endif  // QYNCMAINWINDOW_H
