/**
 * \file QyncMainWindow.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the QyncMainWindow class.
 *
 * \dep
 * - QMainWindow
 * - QyncProcess.h
 *
 * \todo
 * Nothing.
 */

#ifndef QYNC_MAINWINDOW_H
#define QYNC_MAINWINDOW_H

#include <QMainWindow>

#include "Process.h"

class QComboBox;
class QCheckBox;
class QLineEdit;
class QToolBar;
class QMenu;
class QToolButton;
class QAction;

namespace Qync {

	class Manager;
	class Preset;
	class PreferencesDialogue;

	/**
 * \class MainWindow
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief The main window of the Qync GUI.
 *
 * The main window is the hub of user interaction with the Qync application. It
 * provides the user with the ability to manipulate the list of presets stored
 * in the manager, customise presets either for one-off rsync processes or to
 * alter the saved preset, and to launch simulations or synchronisations. It
 * also provides access to the preferences window.
 *
 * The largest part of the main window is dedicated to customising preset
 * settings. Two tabs - basic and advanced settings - group the preset settings
 * into the most commonly used and less frequently used settings. The window
 * has a full menu and two toolbars from which options can be chosen.
 *
 * The presets toolbar and presets menu both provide facilities to add, update
 * and remove presets from the set stored in the manager. The menu also has
 * options to import and export presets to and from external files.
 *
 * The synchronise toolbar provides options to execute the current settings
 * either as a dry-run (simulate) or as a full rsync session (synchronise).
 * These options are replicated in the File menu.
 *
 * The core functionality exposed to the user derives from a QyncManager
 * object that is stored in the main window object. The main window does not
 * take ownership of the manager and the creator of the manager is responsible
 * for its timely destruction. The manager that the window is using can
 * be retrieved using the manager() method. It can be set using the protected
 * setManager() method, which will cause the old manager to be deleted.
 * Subclasses can also connect and disconnect the manager's signals and slots
 * from the window's using the connectManager() and disconnectManager()
 * methods if signal processing needs to be suspended for an internal operation.
 *
 * Most of the class's methods are public slots that correspond to the options
 * available in the GUI. chooseDestinationFile(), chooseDestinationDirectory(),
 * chooseSourceFile() and chooseSourceDirectory() are invoked when the user
 * clicks one of the source or destination chooser buttons to enable to user
 * to choose the appropriate file or directory. Similarly, the
 * saveSettingsToCurrentPreset(), removeCurrentPreset() and
 * newPresetFromSettings() slots are invoked when the user clicks the respective
 * toolbar button  or chooses the respective menu item. The import and export
 * preset menu items are connected to the importPreset() and exportPreset()
 * slots, the simulate and synchronise toolbar buttons and menu items are
 * connected to the simulate() and execute() slots, and the about and about
 * rsync menu items are connected to the about() and aboutRsync() slots.
 * Finally, the preferences item in the file menu calls the showPreferences()
 * slot.
 *
 * The manager provides some useful signals that the window makes use of. It
 * emits the presetsChanged() signal which is connected to the refreshPresets()
 * slot so that the combo box showing the list of presets is always
 * synchronised with the set of presets stored in the manager. It also emits
 * the preferencesChanged() signal, which is connected to the readPreferences()
 * slot to ensure that the GUI is always styled according to the user's
 * preferences.
 */
	class MainWindow
	  : public QMainWindow {
		Q_OBJECT

	private:
		Manager * m_manager;
		Preset * m_tempPreset;
		PreferencesDialogue * m_prefsWindow;

		QMenu * m_presetsMenu;

		QToolBar * m_presetsToolbar;
		QToolBar * m_synchroniseToolbar;

		QComboBox * m_preset;

		QCheckBox * m_preserveTime;
		QCheckBox * m_preservePerms;
		QCheckBox * m_preserveOwner;
		QCheckBox * m_preserveGroup;

		QCheckBox * m_windowsCompatible;
		QCheckBox * m_deleteOnServer;

		QLineEdit * m_source;
		QLineEdit * m_dest;

		QToolButton * m_chooseSourceDir;
		QToolButton * m_chooseDestinationDir;
		QToolButton * m_chooseSourceFile;
		QToolButton * m_chooseDestinationFile;

		QCheckBox * m_alwaysChecksum;
		QCheckBox * m_preserveDevices;
		QCheckBox * m_keepParitalTransfers;
		QCheckBox * m_symlinksAsSymlinks;
		QCheckBox * m_makeBackups;

		QCheckBox * m_compressInTransit;
		QCheckBox * m_onlyUpdateExisting;
		QCheckBox * m_dontUpdateExisting;
		QCheckBox * m_dontMapUidGid;
		QCheckBox * m_copyHardlinksAsHardlinks;
		QCheckBox * m_showItemisedChanges;

		QLineEdit * m_logFile;
		QToolButton * m_chooseLogFile;

		QAction * m_menuFileChooseSource;
		QAction * m_menuFileChooseDest;
		QAction * m_menuFileSwitchSourceAndDest;
		QAction * m_menuFileSimulate;
		QAction * m_menuFileExecute;
		QAction * m_menuFileQuit;

		QAction * m_savePresetAction;
		QAction * m_savePresetAsAction;
		QAction * m_removePresetAction;

		/**
   * \brief Create all the window's widgets.
   */
		void createWidgets(void);

		/**
   * \brief Fill a preset with the current settings.
   */
		void fillPreset(Preset *) const;

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
		void syncIgnoreExistingToOnlyExisting(void);
		void syncOnlyExistingToIgnoreExisting(void);

	protected:
		/**
   * \brief Set the manager used by the main window.
   *
   * \param manager is the manager for the main window.
   *
   * The manager is used to provide the application functionality
   * controlled by the window. The window does not take ownership of the
   * manager as a manager may be used by more than one interface. The
   * creator of the manager is responsible for its timely destruction.
   */
		bool setManager(Manager * manager);

		/**
   * \brief Disconnect the manager's signals from the window's slots.
   */
		void disconnectManager(void);

		/**
   * \brief Connect the manager's signals to the window's slots.
   */
		void connectManager(void);

	public:
		/**
   * \brief Create a new QyncMainWindow.
   *
   * \param manager is the manager for the main window.
   *
   * The manager is used to provide the application functionality
   * controlled by the window. The window does not take ownership of the
   * manager as a manager may be used by more than one interface. The
   * creator of the manager is responsible for its timely destruction.
   */
		explicit MainWindow(Manager * manager = nullptr);

		/**
   * \brief Destroy the QyncMainWindow.
   */
		virtual ~MainWindow(void);

		/**
   * \brief Retrieve the main window's application manager.
   *
   * The manager is used to provide the application functionality
   * controlled by the window. The manager may be used by more than one
   * interface so you may not take the manager, set the window's manager
   * to \b null and then delete the manager you took. The original creator
   * of the manager is responsible for its timely destruction.
   *
   * \return The manager.
   */
		Manager * manager(void) const;

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
	};

}  // namespace Qync

#endif  // QYNCMAINWINDOW_H
