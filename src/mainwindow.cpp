/**
 * \file mainwindow.cpp
 * \author Darren Edale
 * \date September, 2017
 * \version 1.1.0
 *
 * \brief Implementation of the MainWindow class.
 *
 * \dep
 * - mainwindow.h
 * - mainwindow.ui
 * - QDebug
 * - QDir
 * - QString
 * - QIcon
 * - QMessageBox
 * - QFileDialog
 * - QInputDialog
 * - QStandardPaths
 * - application.h
 * - guipreferences.h
 * - preset.h
 * - process.h
 * - processdialogue.h
 * - preferencesdialogue.h
 * - aboutdialogue.h
 * - functions.h
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QString>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QStandardPaths>
#include <QStringBuilder>
#include <QPropertyAnimation>
#include "application.h"
#include "guipreferences.h"
#include "preset.h"
#include "process.h"
#include "processdialogue.h"
#include "preferencesdialogue.h"
#include "aboutdialogue.h"
#include "functions.h"
#include "types.h"

namespace Qync {

	/**
	 * \class MainWindow
	 * \author Darren Edale
	 * \date October 2017
	 * \version 1.1.0
	 *
	 * \brief The main window of the Qync GUI.
	 *
	 * The main window is the centre of user interaction with the Qync application.
	 * It consists of two user interface styles:
	 * - a simple backup-style UI
	 * - a full UI for greater control.
	 *
	 * The simple UI provides the user with the ability to specify what to back up,
	 * where to back it up to, and whether to do an incremental or full backup.
	 *
	 * The full UI provides the user with many more options controlling how the rsync
	 * process operates, as well as the ability to store and manipulate presets
	 * representing synchronisations that are often run. It also provides a facility
	 * to simulate a synchronisation to dry run some settings before attempting a
	 * synchronisation.
	 *
	 * The largest part of the main window is dedicated to customising preset
	 * settings. Two tabs - basic and advanced settings - group the preset settings
	 * into the most commonly used and less frequently used settings. The window
	 * has a full menu and two toolbars from which options can be chosen.
	 *
	 * The presets toolbar and presets menu both provide facilities to add, update
	 * and remove presets from the set stored in the application. The menu also has
	 * options to import and export presets to and from external files.
	 *
	 * The synchronise toolbar provides options to execute the current settings
	 * either as a dry-run (simulate) or as a full rsync session (synchronise).
	 * These options are replicated in the File menu.
	 *
	 * The core functionality exposed to the user derives from the main Application
	 * singleton object. The MainWindow listens for the Application::preferencesChanged
	 * signal to know when it needs to refresh itself based on changes to the user's
	 * preferences. Subclasses can connect and disconnect this using the
	 * connectApplication() and disconnectApplication() methods if signal processing
	 * needs to be suspended temporarily for an internal operations.
	 *
	 * Most of the class's methods are public slots that correspond to the options
	 * available in the GUI. The saveSettingsToCurrentPreset(), removeCurrentPreset()
	 * and newPresetFromSettings() slots are invoked when the user clicks the respective
	 * toolbar button or chooses the respective menu item. The import and export preset
	 * menu items are connected to the importPreset() and exportPreset() slots; the
	 * _simulate_ and _synchronise_ toolbar buttons and menu items are connected to the
	 * simulate() and synchronise() slots, and the _about_ and _about rsync_ menu items
	 * are connected to the about() and aboutRsync() slots. Finally, the preferences item
	 * in the file menu, and the preferences button, both call the showPreferences()
	 * slot.
	 */

	/**
	 * \brief Create a new MainWindow.
	 */
	MainWindow::MainWindow()
	: QMainWindow(nullptr),
	  m_ui(new Ui::MainWindow),
	  m_prefsWindow(nullptr),
	  m_aboutDialogue(nullptr) {
		m_ui->setupUi(this);
		m_ui->presetsToolbar->insertWidget(m_ui->actionNew, m_ui->presets);
		QIcon appIcon(":/icons/application");
		m_ui->simpleLogo->setPixmap(appIcon.pixmap(64));
		m_ui->simpleSourceAndDestination->setSourceLabel(tr("Backup"));
		m_ui->simpleSourceAndDestination->setDestinationLabel(tr("To"));

		QFont titleFont = m_ui->simpleUiTitle->font();
		titleFont.setPointSizeF(titleFont.pointSizeF() * 1.5);
		titleFont.setBold(true);
		m_ui->simpleUiTitle->setFont(titleFont);

		m_ui->notification->setVisible(false);

		// group is owned by MainWindow and will be deleted when QWidget
		// base class destructor is called
		QActionGroup * uiSwitchGroup = new QActionGroup(this);
		uiSwitchGroup->addAction(m_ui->actionSimpleUi);
		uiSwitchGroup->addAction(m_ui->actionFullUi);

		QString appDisplayName = qyncApp->applicationDisplayName();
		setWindowIcon(appIcon);
		setWindowTitle(appDisplayName);
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

		connect(m_ui->actionSimpleUi, &QAction::toggled, this, &MainWindow::useSimpleUi);

		connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
		connect(m_ui->actionAboutRsync, &QAction::triggered, this, &MainWindow::aboutRsync);
		connect(m_ui->actionExport, &QAction::triggered, this, &MainWindow::exportPreset);
		connect(m_ui->actionImport, &QAction::triggered, this, &MainWindow::importPreset);
		connect(m_ui->actionNew, &QAction::triggered, this, &MainWindow::newPreset);
		connect(m_ui->actionPreferences, &QAction::triggered, this, &MainWindow::showPreferences);
		connect(m_ui->actionRemove, &QAction::triggered, this, &MainWindow::removeCurrentPreset);
		connect(m_ui->actionSave, &QAction::triggered, this, &MainWindow::saveSettingsToCurrentPreset);
		connect(m_ui->actionSaveAs, &QAction::triggered, this, &MainWindow::newPresetFromSettings);
		connect(m_ui->actionSimulate, &QAction::triggered, this, &MainWindow::simulate);
		connect(m_ui->actionSync, &QAction::triggered, this, &MainWindow::synchronise);

		connect(m_ui->chooseLogFile, &QToolButton::clicked, this, &MainWindow::chooseLogFile);
		connect(m_ui->preferences, &QToolButton::clicked, m_ui->actionPreferences, &QAction::trigger);
		connect(m_ui->quitButton, &QPushButton::clicked, this, &MainWindow::close);
		connect(m_ui->synchroniseButton, &QPushButton::clicked, m_ui->actionSync, &QAction::trigger);
		
		connect(m_ui->presets, &PresetCombo::currentPresetChanged, this, &MainWindow::showPreset);
		connect(m_ui->menuMyPresets, &PresetMenu::presetIndexTriggered, m_ui->presets, &QComboBox::setCurrentIndex);

		connectApplication();

		m_prefsWindow.reset(new PreferencesDialogue);
		m_prefsWindow->setWindowTitle(tr("%1 Preferences").arg(appDisplayName));

		m_aboutDialogue.reset(new AboutDialogue);
		m_aboutDialogue->setWindowTitle(tr("About %1").arg(appDisplayName));

		// force the UI to follow the preferences on startup
		onPreferencesChanged();

		// ensure UI is in correct state for selected preset
		if(!m_ui->presets->currentItemIsNewPreset()) {
			showPreset(m_ui->presets->currentPreset());
		}
	}

	/**
	 * \brief Show an unobtrusive notification in the main window.
	 *
	 * \param title The title of the notification.
	 * \param msg The message to show.
	 * \param type The type of the notification.
	 *
	 * The title can be empty to indicate there is no title. The message may
	 * contain rich text in the format that Qt's rich text document engine
	 * understands. The type determines the background colour of the
	 * notification: yellow for warnings, red for errors, and the standard
	 * UI background colour for all others.
	 */
	void MainWindow::showNotification(const QString & title, const QString & message, NotificationType type)
	{
		QString content;

		if(!title.isEmpty()) {
			content = "<strong>" % title % "</strong> ";
		}

		content = content % message;
		m_ui->notification->setMaximumHeight(0);
		m_ui->notification->setMessage(content);
		m_ui->notification->setType(type);
		m_ui->notification->show();
	}

	/**
	 * \brief Destroy the MainWindow.
	 */
	MainWindow::~MainWindow() = default;

	/**
	 * \brief Show the details of a preset from the application.
	 *
	 * \param preset is a reference to the preset to show.
	 *
	 * The widgets in the window are updated to reflect the settings in the preset.
	 */
	void MainWindow::showPreset(const Preset & preset) {
		m_ui->preserveTime->setChecked(preset.preserveTime());
		m_ui->preservePermissions->setChecked(preset.preservePermissions());
		m_ui->preserveOwner->setChecked(preset.preserveOwner());
		m_ui->preserveGroup->setChecked(preset.preserveGroup());

		m_ui->windowsCompatible->setChecked(preset.windowsCompatability());
		m_ui->honourDeletions->setChecked(preset.honourDeletions());

		m_ui->simpleDoFullBackup->setChecked(preset.ignoreTimes());
		m_ui->alwaysCompareChecksums->setChecked(preset.alwaysCompareChecksums());
		m_ui->preserveDevices->setChecked(preset.preserveDevices());
		m_ui->keepPartialFiles->setChecked(preset.keepPartialTransfers());
		m_ui->symlinksAsSymlinks->setChecked(preset.copySymlinksAsSymlinks());
		m_ui->makeBackups->setChecked(preset.makeBackups());

		m_ui->compressInTransit->setChecked(preset.useTransferCompression());

		if(preset.onlyUpdateExistingEntries()) {
			m_ui->includeInSynchronisation->setWhat(SynchroniseWhatCombo::What::OnlyPreExisting);
		}
		else if(preset.dontUpdateExistingEntries()) {
			m_ui->includeInSynchronisation->setWhat(SynchroniseWhatCombo::What::OnlyNonExistant);
		}
		else {
			m_ui->includeInSynchronisation->setWhat(SynchroniseWhatCombo::What::Everything);
		}

		m_ui->dontMapUidGid->setChecked(preset.dontMapUsersAndGroups());
		m_ui->hardlinksAsHardlinks->setChecked(preset.copyHardlinksAsHardlinks());
		m_ui->itemisedChanges->setChecked(preset.showItemisedChanges());

		// it's easy, and quicker, to ensure these are manually synchronised rather
		// than wait for propagation of the signal
		QSignalBlocker mainSrcDestBlocker(m_ui->sourceAndDestination);
		QSignalBlocker basicSrcDestBlocker(m_ui->simpleSourceAndDestination);

		m_ui->sourceAndDestination->setSource(preset.source());
		m_ui->sourceAndDestination->setDestination(preset.destination());
		m_ui->simpleSourceAndDestination->setSource(preset.source());
		m_ui->simpleSourceAndDestination->setDestination(preset.destination());

		m_ui->logFile->setText(preset.logFile());

		m_ui->actionRemove->setEnabled(true);
	}

	/**
	 * \brief (Re)read the preferences from the application.
	 *
	 * This slot essentially makes the visual appearance of the GUI match
	 * the settings stored in the application's preferences object.
	 */
	void MainWindow::onPreferencesChanged() {
		useSimpleUi(qyncApp->preferences().useSimpleUi());
	}

	/**
	 * \brief Disconnect the application's signals from the window's slots.
	 */
	void MainWindow::disconnectApplication() {
		qyncApp->disconnect(this);
	}

	/**
	 * \brief Connect the application's signals to the window's slots.
	 */
	void MainWindow::connectApplication() {
		connect(qyncApp, &Application::preferencesChanged, this, &MainWindow::onPreferencesChanged);
	}

	/**
	 * \brief Choose between the simple and full UIs.
	 *
	 * \param useSimple \b true to use the simple user interface, \b false to
	 * use the full user interface.
	 */
	void MainWindow::useSimpleUi(bool useSimple) {
		if(useSimple) {
			m_ui->actionSimpleUi->setChecked(true);
			m_ui->actionFullUi->setChecked(false);

			m_ui->presetsToolbar->hide();
			m_ui->synchroniseToolbar->hide();
			m_ui->mainStack->setCurrentWidget(m_ui->simpleUi);
			m_ui->synchroniseButton->setText(tr("Backup"));
			adjustSize();
		}
		else {
			m_ui->actionSimpleUi->setChecked(false);
			m_ui->actionFullUi->setChecked(true);

			const auto & prefs = qyncApp->preferences();
			m_ui->presetsToolbar->setToolButtonStyle(prefs.toolBarButtonStyle());
			m_ui->synchroniseToolbar->setToolButtonStyle(prefs.toolBarButtonStyle());

			m_ui->presetsToolbar->setVisible(prefs.showPresetsToolBar());
			m_ui->synchroniseToolbar->setVisible(prefs.showSynchroniseToolBar());

			m_ui->mainStack->setCurrentWidget(m_ui->fullUi);
			m_ui->synchroniseButton->setText(tr("Synchronise"));
			adjustSize();
		}
	}

	/**
	 * \brief Choose a local file for the rsycn output log.
	 *
	 * A file browser is presented for the user to to choose a log file.
	 * If the user does not cancel the dialogue, the chosen file is set
	 * as the text in the log file line edit.
	 */
	void MainWindow::chooseLogFile() {
		QString newLog = QFileDialog::getSaveFileName(this, tr("Choose log file"), m_ui->logFile->text());

		if(!newLog.isNull()) {
			m_ui->logFile->setText(newLog);
		}
	}

	/**
	 * \brief Switch the content of the source and destination line edits.
	 */
	void MainWindow::switchSourceAndDestination() {
		m_ui->sourceAndDestination->swapSourceAndDestination();
		m_ui->simpleSourceAndDestination->swapSourceAndDestination();
	}

	/**
	 * \brief Save the current settings to the currently-selected preset.
	 *
	 * The current preset is retrieved from the application and updated with
	 * the settings set in all the widgets in the window.
	 */
	void MainWindow::saveSettingsToCurrentPreset() {
		if(m_ui->presets->currentItemIsNewPreset()) {
			// saving to <new preset> item so create a new one instead
			newPresetFromSettings();
			return;
		}

		Preset & myPreset = m_ui->presets->currentPreset();
		fillPreset(myPreset);
		myPreset.save();
	}

	/**
	 * \brief Remove the currently selected preset.
	 *
	 * The current preset is removed from the application.
	 */
	void MainWindow::removeCurrentPreset() {
		if(m_ui->presets->currentItemIsNewPreset()) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("There are no presets to remove."), NotificationType::Warning);
			return;
		}

		if(!qyncApp->removePreset(m_ui->presets->currentIndex())) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The selected preset could not be removed:\n\n%1").arg(qyncApp->lastError()), NotificationType::Warning);
			return;
		}
	}

	/**
	 * \brief Create a new preset.
	 *
	 * A dialogue box is presented for the user to enter a name for the
	 * new preset. If the dialogue is not cancelled, a new preset is
	 * created using the current settings and added to the application.
	 */
	void MainWindow::newPresetFromSettings() {
		newPreset(true);
	}

	/**
	 * \brief Create a new preset.
	 *
	 * \param fill Whether or not the new preset should inherit the settings
	 * currently selected.
	 *
	 * A dialogue box is presented for the user to enter a name for the
	 * new preset. If the dialogue is not cancelled, a new preset
	 * is created (using the current settings if \b fill == \b true) and
	 * added to the application's master list.
	 */
	void MainWindow::newPreset(bool fill) {
		bool ok;
		QString name = QInputDialog::getText(this, tr("%1 Request").arg(qyncApp->applicationDisplayName()), tr("Enter a name for the new preset"), QLineEdit::Normal, tr("New Preset"), &ok);

		if(!ok) {
			return;
		}

		Preset & p = qyncApp->addPreset(name);

		if(fill) {
			fillPreset(p);
		}

		m_ui->presets->setCurrentIndex(m_ui->presets->count() - 1);
	}

	/**
	 * \brief Import a preset from a file.
	 *
	 * A file dialogue is presented for the user to choose a file from which
	 * to import a preset. If the user does not cancel the dialogue and the
	 * file s/he chooses is a valid preset file, the preset is added to
	 * the application. Any failures to do so, other than the user cancelling
	 * the dialogue, are reported to the user.
	 */
	void MainWindow::importPreset() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Import %1 preset").arg(qyncApp->applicationDisplayName()));

		if(fileName.isEmpty()) {
			return;
		}

		if(!qyncApp->loadPreset(fileName)) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The file \"%1\" was not a valid %2 preset file.").arg(fileName).arg(qyncApp->applicationDisplayName()), NotificationType::Warning);
			return;
		}

		m_ui->presets->setCurrentIndex(m_ui->presets->count() - 1);
	}

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
	void MainWindow::exportPreset() {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Export %1 preset").arg(qyncApp->applicationDisplayName()));

		if(fileName.isEmpty()) {
			return;
		}

		QFileInfo f(fileName);

		if(f.exists() && f.isDir()) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The path you selected is a directory. You cannot save a %1 preset over a director.").arg(qyncApp->applicationDisplayName()), NotificationType::Warning);
			return;
		}

		QString name;

		if(!m_ui->presets->currentItemIsNewPreset()) {
			name = qyncApp->preset(m_ui->presets->currentIndex()).name();
		}

		if(name.isEmpty()) {
			name = QFileInfo(fileName).baseName();
		}

		Preset temp;
		fillPreset(temp);
		temp.setName(name);

		if(!temp.saveCopyAs(fileName)) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The %1 preset could not be exported to the file \"%2\".").arg(qyncApp->applicationDisplayName()).arg(fileName), NotificationType::Warning);
		}
	}

	/**
	 * \brief Fill a preset with the current settings.
	 */
	void MainWindow::fillPreset(Preset & p) const {
		// check which UI is in use and act accordingly
		if(m_ui->mainStack->currentWidget() == m_ui->simpleUi) {
			p.setDefaults();
			p.setSource(m_ui->simpleSourceAndDestination->source());
			p.setDestination(m_ui->simpleSourceAndDestination->destination());

			p.setCopyHardlinksAsHardlinks(false);
			p.setCopySymlinksAsSymlinks(false);
			p.setDontMapUsersAndGroups(true);
			p.setDontUpdateExistingEntries(false);
			p.setHonourDeletions(true);
			p.setKeepPartialTransfers(false);
			p.setMakeBackups(false);
			p.setOnlyUpdateExistingEntries(false);
			p.setPreserveDevices(false);
			p.setPreserveGroup(true);
			p.setPreserveOwner(true);
			p.setPreservePermissions(true);
			p.setPreserveTime(true);
			p.setShowItemisedChanges(false);
			p.setUseTransferCompression(true);
			p.setWindowsCompatability(false);

			if(m_ui->simpleDoFullBackup->isChecked()) {
				p.setAlwaysCompareChecksums(false);
				p.setIgnoreTimes(true);
			}
			else {
				p.setAlwaysCompareChecksums(true);
				p.setIgnoreTimes(false);
			}
		}
		else {
			p.setPreserveGroup(m_ui->preserveGroup->isChecked());
			p.setPreserveOwner(m_ui->preserveOwner->isChecked());
			p.setPreservePermissions(m_ui->preservePermissions->isChecked());
			p.setPreserveTime(m_ui->preserveTime->isChecked());

			p.setWindowsCompatability(m_ui->windowsCompatible->isChecked());
			p.setHonourDeletions(m_ui->honourDeletions->isChecked());

			p.setAlwaysCompareChecksums(m_ui->alwaysCompareChecksums->isChecked());
			p.setIgnoreTimes(m_ui->ignoreTimes->isChecked());
			p.setPreserveDevices(m_ui->preserveDevices->isChecked());
			p.setKeepPartialTransfers(m_ui->keepPartialFiles->isChecked());
			p.setCopySymlinksAsSymlinks(m_ui->symlinksAsSymlinks->isChecked());
			p.setMakeBackups(m_ui->makeBackups->isChecked());

			p.setUseTransferCompression(m_ui->compressInTransit->isChecked());

			switch(m_ui->includeInSynchronisation->what()) {
				case SynchroniseWhatCombo::What::OnlyPreExisting:
					p.setOnlyUpdateExistingEntries(true);
					p.setDontUpdateExistingEntries(false);
					break;

				case SynchroniseWhatCombo::What::OnlyNonExistant:
					p.setOnlyUpdateExistingEntries(false);
					p.setDontUpdateExistingEntries(true);
					break;

				case SynchroniseWhatCombo::What::Everything:
					p.setOnlyUpdateExistingEntries(false);
					p.setDontUpdateExistingEntries(false);
					break;
			}

			p.setDontMapUsersAndGroups(m_ui->dontMapUidGid->isChecked());
			p.setCopyHardlinksAsHardlinks(m_ui->hardlinksAsHardlinks->isChecked());
			p.setShowItemisedChanges(m_ui->itemisedChanges->isChecked());

			p.setSource(m_ui->sourceAndDestination->source());
			p.setDestination(m_ui->sourceAndDestination->destination());

			p.setLogFile(m_ui->logFile->text());
		}
	}

	/**
	 * \brief Helper for synchronise() and simulate().
	 *
	 * \param process The process to run.
	 *
	 * This helper is called by MainWindow::simulate() and MainWindow::synchronise() to
	 * run the process to carry out the requested operation. These functions share code
	 * that is identical save for the details of the process to run. This helper saves
	 * repeating this common code.
	 *
	 * \return \b true if the Process was started, \b false otherwise.
	 */
	bool MainWindow::runProcess(std::shared_ptr<Process> & process) {
		if(m_ui->simpleUi == m_ui->mainStack->currentWidget()) {
			// while simple backup in process, prevent any other being started
			m_ui->simpleDoFullBackup->setEnabled(false);
			m_ui->simpleDoIncrementalBackup->setEnabled(false);
			m_ui->simpleSourceAndDestination->setEnabled(false);
			m_ui->synchroniseButton->setEnabled(false);

			// re-enable the UI when the process has finished
			connect(process.get(), static_cast<void (Process::*)(Process::ExitCode)>(&Process::finished), [this]() {
				m_ui->simpleDoFullBackup->setEnabled(true);
				m_ui->simpleDoIncrementalBackup->setEnabled(true);
				m_ui->simpleSourceAndDestination->setEnabled(true);
				m_ui->synchroniseButton->setEnabled(true);
			});

			// widget shares ownership of the process
			m_ui->simpleProcessWidget->setProcess(process);
		}
		else {
			// dialogue shares ownership of the process
			// dialogue deletes itself on closure
			ProcessDialogue * w = new ProcessDialogue(process, this);
			w->setWindowTitle(tr("%1 %2: %3").arg(qyncApp->applicationDisplayName(), (process->isDryRun() ? tr("simulation") : tr("synchronisation")), m_ui->presets->currentText()));
			w->show();
		}

		process->start();
		return true;
	}

	/**
	 * \brief Start a simulation based on the current settings.
	 *
	 * The current settings are used to create a Process object that will
	 * simulate the synchronisation. A simulation is a dry-run of \b rsync
	 * that performs all operations defined by the settings without actually
	 * modifying anything on disk or on a remote server.
	 */
	void MainWindow::simulate() {
		Preset preset;
		fillPreset(preset);
		auto process = std::make_shared<Process>(preset, Process::RunType::DryRun);

		if(!runProcess(process)) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), "The simulation failed:\n\n" + qyncApp->lastError());
		}
	}

	/**
	 * \brief Start a synchronisation based on the current settings.
	 *
	 * The current settings are used to create a Process object that will
	 * execute the synchronisation.
	 */
	void MainWindow::synchronise() {
		Preset preset;
		fillPreset(preset);
		auto process = std::make_shared<Process>(preset);

		if(!runProcess(process)) {
			showNotification(tr("%1 Warning").arg(qyncApp->applicationDisplayName()), "The synchronisation failed:\n\n" + qyncApp->lastError());
		}
	}

	/**
	 * \brief Show the preferences dialogue.
	 */
	void MainWindow::showPreferences() {
		m_prefsWindow->show();
		m_prefsWindow->raise();
		m_prefsWindow->activateWindow();
	}

	/**
	 * \brief Show the about Qync dialogue.
	 */
	void MainWindow::about() {
		m_aboutDialogue->show();
		m_aboutDialogue->raise();
		m_aboutDialogue->activateWindow();
	}

	/**
	 * \brief Show the about rsync dialogue.
	 *
	 * The content is retrieved from \ref Application::rsyncVersionText()
	 */
	void MainWindow::aboutRsync() {
		QMessageBox::information(this, tr("Qync - About rsync"), qyncApp->rsyncVersionText());
	}

}  // namespace Qync
