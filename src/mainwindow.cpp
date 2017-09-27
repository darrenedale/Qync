/**
 * \file MainWindow.cpp
 * \author Darren Edale
 * \date September, 2017
 * \version 1.0.0
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
 * - QApplication
 * - QStandardPaths
 * - application.h
 * - guipreferences.h
 * - preset.h
 * - processdialogue.h
 * - preferencesdialogue.h
 * - aboutdialogue.h
 * - functions.h
 *
 * \todo convert includeInSynchronisation in .ui file to a custom subclass of
 * QComboBox
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
#include <QApplication>
#include <QStandardPaths>

#include "application.h"
#include "guipreferences.h"
#include "preset.h"
#include "process.h"
#include "processdialogue.h"
#include "preferencesdialogue.h"
#include "aboutdialogue.h"
#include "functions.h"


/* this identifies the <new preset> item in the presets list
 * it is set as the item data for the item. */
#define QYNC_MAINWINDOW_NEW_PRESET_TAG -99


namespace Qync {


	/**
	 * \class MainWindow
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
	 *
	 * \brief The main window of the Qync GUI.
	 *
	 * The main window is the hub of user interaction with the Qync application. It
	 * provides the user with the ability to manipulate the list of presets stored
	 * in the application, customise presets either for one-off rsync processes or to
	 * alter the saved preset, and to launch simulations or synchronisations. It
	 * also provides access to the preferences window.
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
	 * The core functionality exposed to the user derives from a QyncManager
	 * object that is stored in the main window object. The main window does not
	 * take ownership of the application and the creator of the application is responsible
	 * for its timely destruction. The application that the window is using can
	 * be retrieved using the application() method. It can be set using the protected
	 * setManager() method, which will cause the old application to be deleted.
	 * Subclasses can also connect and disconnect the application's signals and slots
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
	 * The application provides some useful signals that the window makes use of. It
	 * emits the presetsChanged() signal which is connected to the refreshPresets()
	 * slot so that the combo box showing the list of presets is always
	 * synchronised with the set of presets stored in the application. It also emits
	 * the preferencesChanged() signal, which is connected to the readPreferences()
	 * slot to ensure that the GUI is always styled according to the user's
	 * preferences.
	 */


	/* indices of items in includeInSynchronisation combo */
	const int MainWindow::UpdateEverything = 0;
	const int MainWindow::OnlyUpdateExisting = 1;
	const int MainWindow::DontUpdateExisting = 2;


	/**
	 * \brief Create a new QyncMainWindow.
	 */
	MainWindow::MainWindow(void)
	: QMainWindow(nullptr),
	  m_ui(new Ui::MainWindow),
	  m_prefsWindow(nullptr),
	  m_aboutDialogue(nullptr) {
		m_ui->setupUi(this);
		m_ui->presetsToolbar->insertWidget(m_ui->actionNew, m_ui->presets);

		m_ui->simpleLogo->setPixmap(QIcon(":/icons/application").pixmap(64));
		m_ui->simpleSourceAndDestination->setSourceLabel(tr("Backup"));
		m_ui->simpleSourceAndDestination->setDestinationLabel(tr("To"));

		QFont titleFont = m_ui->simpleUiTitle->font();
		titleFont.setPointSizeF(titleFont.pointSizeF() * 1.5);
		titleFont.setBold(true);
		m_ui->simpleUiTitle->setFont(titleFont);

		connect(m_ui->actionSimpleUi, &QAction::toggled, [& action = m_ui->actionFullUi](bool uncheck) {
			action->setChecked(!uncheck);
		});

		connect(m_ui->actionFullUi, &QAction::toggled, [& action = m_ui->actionSimpleUi](bool uncheck) {
			action->setChecked(!uncheck);
		});

		connect(m_ui->actionSimpleUi, &QAction::triggered, [this](void) {
			useSimpleUi(true);
		});

		connect(m_ui->actionFullUi, &QAction::triggered, [this](void) {
			useSimpleUi(false);
		});

		setWindowIcon(QIcon(":/icons/application"));
		setWindowTitle(qyncApp->applicationDisplayName());
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

		connectApplication();
		refreshPresets();

		/* force the UI to follow the preferences on startup */
		onPreferencesChanged();

		m_prefsWindow.reset(new PreferencesDialogue);
		m_prefsWindow->setWindowTitle(tr("%1 Preferences").arg(qyncApp->applicationDisplayName()));

		m_aboutDialogue.reset(new AboutDialogue);
		m_aboutDialogue->setWindowTitle(tr("About %1").arg(qyncApp->applicationDisplayName()));

		/* TODO this feature has not yet been implemented */
		m_ui->simpleBackupType->hide();

		/* ensure UI is in correct state for selected preset */
		showPreset(m_ui->presets->currentIndex());
	}


	/**
	 * \brief Destroy the MainWindow.
	 */
	MainWindow::~MainWindow(void) = default;


	/**
	 * \brief Show the details of a preset selected from the menu.
	 *
	 * The preset is determined by examining the action that sent the
	 * signal. The index of the preset is stored in the action's data. The
	 * preset is fetched from the application and the widgets in the window
	 * are updated to reflect the settings in the preset.
	 */
	void MainWindow::showPresetFromMenu(void) {
		QAction * action = qobject_cast<QAction *>(sender());

		if(!action) {
			qCritical() << __PRETTY_FUNCTION__ << "showPresetFromMenu() slot call did not result from trigger of action from My Presets menu";
			return;
		}

		int index = action->data().toInt();

		if(index < 0 || index >= qyncApp->presetCount()) {
			qCritical() << __PRETTY_FUNCTION__ << "action from My Presets menu contains invalid preset index";
			refreshPresets();
			return;
		}

		m_ui->presets->setCurrentIndex(index);
	}


	/**
	 * \brief Show the details of a preset from the application.
	 *
	 * \param index is the index of the preset to show.
	 *
	 * The preset is fetched from the application and the widgets in the window
	 * are updated to reflect the settings in the preset.
	 */
	void MainWindow::showPreset(int index) {
		/* NOTE index can be > last preset because combo box has <New Preset> if no presets */
		if(0 == index && QYNC_MAINWINDOW_NEW_PRESET_TAG == m_ui->presets->itemData(0).toInt()) {
			/* <New Preset> chosen */
			m_ui->actionRemove->setEnabled(false);
			return;
		}

		if(0 <= index && qyncApp->presetCount() > index) {
			const Preset & preset = qyncApp->preset(index);
			m_ui->preserveTime->setChecked(preset.preserveTime());
			m_ui->preservePermissions->setChecked(preset.preservePermissions());
			m_ui->preserveOwner->setChecked(preset.preserveOwner());
			m_ui->preserveGroup->setChecked(preset.preserveGroup());

			m_ui->windowsCompatible->setChecked(preset.windowsCompatability());
			m_ui->honourDeletions->setChecked(preset.honourDeletions());

			m_ui->alwaysCompareChecksums->setChecked(preset.alwaysCompareChecksums());
			m_ui->preserveDevices->setChecked(preset.preserveDevices());
			m_ui->keepPartialFiles->setChecked(preset.keepPartialTransfers());
			m_ui->symlinksAsSymlinks->setChecked(preset.copySymlinksAsSymlinks());
			m_ui->makeBackups->setChecked(preset.makeBackups());

			m_ui->compressInTransit->setChecked(preset.useTransferCompression());

			if(preset.onlyUpdateExistingEntries()) {
				m_ui->includeInSynchronisation->setCurrentIndex(OnlyUpdateExisting);
			}
			else if(preset.dontUpdateExistingEntries()) {
				m_ui->includeInSynchronisation->setCurrentIndex(DontUpdateExisting);
			}
			else {
				m_ui->includeInSynchronisation->setCurrentIndex(UpdateEverything);
			}

			m_ui->dontMapUidGid->setChecked(preset.dontMapUsersAndGroups());
			m_ui->hardlinksAsHardlinks->setChecked(preset.copyHardlinksAsHardlinks());
			m_ui->itemisedChanges->setChecked(preset.showItemisedChanges());

			/* it's quicker and easy to ensure these are manually synchronised rather
			 * than wait for signals and slots to do it */
			QSignalBlocker mainSrcDestBlocker(m_ui->sourceAndDestination);
			QSignalBlocker basicSrcDestBlocker(m_ui->simpleSourceAndDestination);

			m_ui->sourceAndDestination->setSource(preset.source());
			m_ui->sourceAndDestination->setDestination(preset.destination());
			m_ui->simpleSourceAndDestination->setSource(preset.source());
			m_ui->simpleSourceAndDestination->setDestination(preset.destination());

			m_ui->logFile->setText(preset.logFile());

			m_ui->actionRemove->setEnabled(true);
		}
	}


	/**
	 * \brief (Re)read the preferences from the application.
	 *
	 * This slot essentially makes the visual appearance of the GUI match
	 * the settings stored in the application's preferences object.
	 */
	void MainWindow::onPreferencesChanged(void) {
		useSimpleUi(qyncApp->preferences().useSimpleUi());
	}


	/**
	 * \brief Disconnect the application's signals from the window's slots.
	 */
	void MainWindow::disconnectApplication(void) {
		qyncApp->disconnect(this);
	}


	/**
	 * \brief Connect the application's signals to the window's slots.
	 */
	void MainWindow::connectApplication(void) {
		connect(qyncApp, &Application::presetsChanged, this, &MainWindow::refreshPresets);
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
			adjustSize();
		}
	}


	/**
	 * \brief Refresh the presets combo box.
	 *
	 * The list of presets in the combo box is discarded and it is
	 * repopulated with the set of presets queried from the application.
	 */
	void MainWindow::refreshPresets(void) {
		m_ui->presets->clear();
		m_ui->menuMyPresets->clear();

		int i = 0;

		for(const auto & preset : qyncApp->presets()) {
			m_ui->presets->addItem(preset->name());
			QAction * action = m_ui->menuMyPresets->addAction(preset->name());
			connect(action, &QAction::triggered, this, &MainWindow::showPresetFromMenu);
			action->setData(i);
			++i;
		}

		if(0 == i) {
			m_ui->presets->addItem(tr("<New Preset>"));
			m_ui->presets->setItemData(0, QYNC_MAINWINDOW_NEW_PRESET_TAG);
		}
	}


	/**
	 * \brief Choose a local file for the rsycn output log.
	 *
	 * A local file browser is presented for the user to to choose a log
	 * file. If the user does not cancel the dialogue, the chosen file is
	 * set as the text in the log file line edit.
	 */
	void MainWindow::chooseLogFile(void) {
		QString newLog = QFileDialog::getSaveFileName(this, tr("Choose log file"), m_ui->logFile->text());

		if(!newLog.isNull()) {
			m_ui->logFile->setText(newLog);
		}
	}


	/**
	 * \brief Switch the content of the source and destination line edits.
	 */
	void MainWindow::switchSourceAndDestination(void) {
		m_ui->sourceAndDestination->swapSourceAndDestination();
	}


	/**
	 * \brief Save the current settings to the currently-selected preset.
	 *
	 * The current preset is retrieved from the application and updated with
	 * the settings set in all the widgets in the window.
	 */
	void MainWindow::saveSettingsToCurrentPreset(void) {
		int i = m_ui->presets->currentIndex();

		if(0 == i && QYNC_MAINWINDOW_NEW_PRESET_TAG == m_ui->presets->itemData(0)) {
			/* saving to <new preset> items so creaet a new one instead */
			newPresetFromSettings();
			return;
		}

		/* get current settings */
		Preset & myPreset = qyncApp->preset(i);
		fillPreset(myPreset);
		myPreset.save();
		showPreset(i);
	}


	/**
	 * \brief Remove the currently selected preset.
	 *
	 * The current preset is removed from the application. This triggers a
	 * refresh of the presets list.
	 */
	void MainWindow::removeCurrentPreset(void) {
		if(qyncApp->presetCount() < 1) {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("There are no presets to remove."));
			return;
		}

		if(!qyncApp->removePreset(m_ui->presets->currentIndex())) {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The selected preset could not be removed:\n\n%1").arg(qyncApp->lastError()));
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
	void MainWindow::newPresetFromSettings(void) {
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

		Preset * p = new Preset();

		if(fill) {
			fillPreset(*p);
		}

		p->setName(name);
		disconnectApplication(); /* don't refresh on presetesChanged() signal from app - we'll do it ourselves */

		if(qyncApp->addPreset(p)) {
			refreshPresets();
			/* it was added successfully, therefore we know the list contains at least one
			 * item, and we know also that as it is not empty the <New Preset> item won't
			 * be present, so we know that the index of the item we just added should be
			 * the last one in the combo */
			m_ui->presets->setCurrentIndex(m_ui->presets->count() - 1);
		}
		else {
			delete p;
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The new preset could not be created:\n\n%1").arg(qyncApp->lastError()));
		}

		connectApplication();
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
	void MainWindow::importPreset(void) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Import %1 preset").arg(qyncApp->applicationDisplayName()));

		if(!fileName.isEmpty()) {
			Preset * p = Preset::load(fileName);

			if(!p) {
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The file \"%1\" was not a valid %2 preset file.").arg(fileName).arg(qyncApp->applicationDisplayName()), QMessageBox::Ok);
				return;
			}

			/* don't refresh on presetesChanged() signal from application - we'll do it ourselves */
			disconnectApplication();

			if(qyncApp->addPreset(p)) {
				refreshPresets();
				/* it was added successfully, therefore we know the list contains at least one
				 * item, and we know also that as it is not empty the <New Preset> item won't
				 * be present, so we know that the index of the item we just added should be
				 * the last one in the combo */
				m_ui->presets->setCurrentIndex(m_ui->presets->count() - 1);
			}
			else {
				delete p;
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The imported %1 preset could not be created:\n\n%2").arg(qyncApp->applicationDisplayName()).arg(qyncApp->lastError()));
			}

			connectApplication();
		}
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
	void MainWindow::exportPreset(void) {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Export %1 preset").arg(qyncApp->applicationDisplayName()));

		if(!fileName.isEmpty()) {
			QFileInfo f(fileName);

			if(f.exists()) {
				if(f.isDir()) {
					QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The path you selected is a directory. You cannot save a %1 preset over a director.").arg(qyncApp->applicationDisplayName()));
					return;
				}
			}

			QString name;

			/* if we're editing an existing preset, copy its name */
			if(-1 != m_ui->presets->currentIndex() && QYNC_MAINWINDOW_NEW_PRESET_TAG != m_ui->presets->currentData()) {
				name = qyncApp->preset(m_ui->presets->currentIndex()).name();
			}

			/* if no name use the basename of the save path chosen by user */
			if(name.isEmpty()) {
				name = QFileInfo(fileName).baseName();
			}

			Preset temp;
			fillPreset(temp);
			temp.setName(name);

			if(!temp.saveCopyAs(fileName)) {
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The %1 preset could not be exported to the file \"%2\".").arg(qyncApp->applicationDisplayName()).arg(fileName));
			}
		}
	}


	/**
	 * \brief Fill a preset with the current settings.
	 */
	void MainWindow::fillPreset(Preset & p) const {
		/* check which UI is in use and act accordingly */
		if(m_ui->mainStack->currentWidget() == m_ui->simpleUi) {
			p.setDefaults();
			p.setSource(m_ui->simpleSourceAndDestination->source());
			p.setDestination(m_ui->simpleSourceAndDestination->destination());
		}
		else {
			p.setPreserveGroup(m_ui->preserveGroup->isChecked());
			p.setPreserveOwner(m_ui->preserveOwner->isChecked());
			p.setPreservePermissions(m_ui->preservePermissions->isChecked());
			p.setPreserveTime(m_ui->preserveTime->isChecked());

			p.setWindowsCompatability(m_ui->windowsCompatible->isChecked());
			p.setHonourDeletions(m_ui->honourDeletions->isChecked());

			p.setAlwaysCompareChecksums(m_ui->alwaysCompareChecksums->isChecked());
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
	 * \brief Start a simulation based on the current settings.
	 */
	void MainWindow::simulate(void) {
		Preset temp;
		fillPreset(temp);
		Process * process = qyncApp->simulate(temp);

		if(process) {
			/* dialogue consumes the process */
			ProcessDialogue * w = new ProcessDialogue(process, this);
			w->setWindowTitle(tr("%1 Simulation: %2").arg(qyncApp->applicationDisplayName()).arg(m_ui->presets->currentText()));
			w->show();
			process->start();
		}
		else {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), "The simulation failed:\n\n" + qyncApp->lastError());
		}
	}


	/**
	 * \brief Start rsync based on the current settings.
	 */
	void MainWindow::synchronise(void) {
		Preset temp;
		fillPreset(temp);
		Process * process = qyncApp->synchronise(temp);

		if(process) {
			/* dialogue consumes the process */
			ProcessDialogue * w = new ProcessDialogue(process, this);
			w->setWindowTitle(tr("%1 Synchronisation: %2").arg(qyncApp->applicationDisplayName()).arg(m_ui->presets->currentText()));
			w->show();
			process->start();
		}
		else {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), "The synchronisation failed:\n\n" + qyncApp->lastError());
		}
	}


	/**
	 * \brief Show the preferences dialogue.
	 */
	void MainWindow::showPreferences(void) {
		m_prefsWindow->show();
		m_prefsWindow->raise();
		m_prefsWindow->activateWindow();
	}


	/**
	 * \brief Show the about Qync dialogue.
	 */
	void MainWindow::about(void) {
		m_aboutDialogue->show();
		m_aboutDialogue->raise();
		m_aboutDialogue->setFocus();
	}


	/**
	 * \brief Show the about rsync dialogue.
	 *
	 * The dialogue content is retrieved from
	 * \ref QyncManager::rsyncVersionText()
	 */
	void MainWindow::aboutRsync(void) {
		QMessageBox::information(this, tr("Qync - About rsync"), qyncApp->rsyncVersionText());
	}


}  // namespace Qync
