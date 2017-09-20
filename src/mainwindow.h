/**
 * \file QyncMainWindow.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.5
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

	class MainWindow
	  : public QMainWindow {
		Q_OBJECT

	public:
		explicit MainWindow(void);
		virtual ~MainWindow(void);

	public Q_SLOTS:
		/**
		 * \brief Refresh the presets combo box.
		 *
		 * The list of presets in the combo box is discarded and it is
		 * repopulated with the set of presets queried from the manager.
		 */
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

		/**
		 * \brief Save the current settings to the currently-selected preset.
		 *
		 * The current preset is retrieved from the manager and updated with
		 * the settings set in all the widgets in the window.
		 */
		void saveSettingsToCurrentPreset(void);

		/**
		 * \brief Remove the currently selected preset.
		 *
		 * The current preset is removed from the manager. This triggers a
		 * refresh of the presets list.
		 */
		void removeCurrentPreset(void);

		/**
		 * \brief Create a new preset.
		 *
		 * A dialogue box is presented for the user to enter a name for the
		 * new preset. If the dialogue is not cancelled, a new preset is
		 * created using the current settings and added to the manager.
		 */
		void newPresetFromSettings(void);

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

		/**
		 * \brief Show the preferences dialogue.
		 */
		void showPreferences(void);

		/**
		 * \brief Start a simulation based on the current settings.
		 *
		 * The manager is requested to start a simulation with the current
		 * settings.
		 */
		void simulate(void);

		/**
		 * \brief Start rsync based on the current settings.
		 *
		 * The manager is requested to start a full execution of rsync with the
		 * current settings.
		 */
		void execute(void);

		/**
		 * \brief Show the about Qync dialogue.
		 */
		void about(void);

		/**
		 * \brief Show the about rsync dialogue.
		 *
		 * The dialogue content is retrieved from
		 * \ref QyncManager::rsyncVersionText()
		 */
		void aboutRsync(void);


	private Q_SLOTS:
		/**
		 * \brief Show the details of a preset selected from the menu.
		 *
		 * The preset is determined by examining the action that sent the
		 * signal. The index of the preset is stored in the action's data. The
		 * preset is fetched from the manager and the widgets in the window
		 * are updated to reflect the settings in the preset.
		 */
		void showPresetFromMenu(void);

		/**
		 * \brief Show the details of a preset from the manager.
		 *
		 * \param index is the index of the preset to show.
		 *
		 * The preset is fetched from the manager and the widgets in the window
		 * are updated to reflect the settings in the preset.
		 */
		void showPreset(int index);

		/**
		 * \brief (Re)read the preferences from the manager.
		 *
		 * This slot essentially makes the visual appearance of the GUI match
		 * the settings stored in the manager's preferences object.
		 */
		void readPreferences(void);

		/**
		 * \brief Ensure that the check boxes for only syncing existing
		 * entries and ignoring existing entries are kept mutually exclusive.
		 */
		//		void syncIgnoreExistingToOnlyExisting(void);
		//		void syncOnlyExistingToIgnoreExisting(void);


	protected:
		/**
		 * \brief Disconnect the manager's signals from the window's slots.
		 */
		void disconnectApplication(void);

		/**
		 * \brief Connect the manager's signals to the window's slots.
		 */
		void connectApplication(void);

	private:
		/**
		 * \brief Fill a preset with the current settings.
		 */
		void fillPreset(Preset &) const;

		std::unique_ptr<Ui::MainWindow> m_ui;
		std::unique_ptr<PreferencesDialogue> m_prefsWindow;
	};

}  // namespace Qync

#endif  // QYNCMAINWINDOW_H
