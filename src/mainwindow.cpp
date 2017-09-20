/**
 * \file MainWindow.cpp
 * \author Darren Edale
 * \date September, 2017
 * \version 0.9.5
 *
 * \brief Implementation of the MainWindow class.
 *
 * \dep
 * - QDebug
 * - QDir
 * - QString
 * - QIcon
 * - QMessageBox
 * - QFileDialog
 * - QInputDialog
 * - QMetaObject
 * - QMetaProperty
 * - QApplication
 * - mainwindow.h
 * - ui_mainwindow.h
 * - application.h
 * - preferences.h
 * - preset.h
 * - processdialogue.h
 * - preferencesdialogue.h
 * - functions.h
 *
 * \todo use QIcon::fromTheme() wherever possible
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
#include <QMetaObject>
#include <QMetaProperty>
#include <QApplication>
#include <QStandardPaths>

#include "application.h"
#include "guipreferences.h"
#include "preset.h"
#include "process.h"
#include "processdialogue.h"
#include "preferencesdialogue.h"
#include "functions.h"


namespace Qync {


	/**
	 * \class MainWindow
	 * \author Darren Edale
	 * \date September 2017
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


	/* indices of items in includeInSynchronisation combo */
	const int MainWindow::UpdateEverything = 0;
	const int MainWindow::OnlyUpdateExisting = 1;
	const int MainWindow::DontUpdateExisting = 2;


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
	MainWindow::MainWindow(void)
	  : QMainWindow(nullptr),
		 m_ui(new Ui::MainWindow),
		 m_prefsWindow(nullptr) {
		m_ui->setupUi(this);
		m_ui->presetsToolbar->insertWidget(m_ui->actionNew, m_ui->presets);
		setWindowIcon(QIcon(":/icons/application"));
		setWindowTitle(qyncApp->applicationDisplayName());
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


		GuiPreferences * newPrefs = new GuiPreferences(Application::configurationPath() + "/guipreferences");
		qyncApp->setPreferences(newPrefs);

		//		createWidgets();
		connectApplication();
		refreshPresets();
		readPreferences();

		m_prefsWindow.reset(new PreferencesDialogue);
		m_prefsWindow->setWindowTitle(tr("%1 Preferences").arg(qyncApp->applicationDisplayName()));
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
	 * preset is fetched from the manager and the widgets in the window
	 * are updated to reflect the settings in the preset.
	 */
	void MainWindow::showPresetFromMenu(void) {
		QAction * action = qobject_cast<QAction *>(sender());

		if(!action) {
			qDebug() << "showPresetFromMenu() slot call did not result from trigger of action from My Presets menu";
			return;
		}

		int index = action->data().toInt();

		if(index < 0 || index >= qyncApp->presetCount()) {
			qDebug() << "action from My Presets menu contains invalid preset index";
			refreshPresets();
			return;
		}

		m_ui->presets->setCurrentIndex(index);
	}


	/**
	 * \brief Show the details of a preset from the manager.
	 *
	 * \param index is the index of the preset to show.
	 *
	 * The preset is fetched from the manager and the widgets in the window
	 * are updated to reflect the settings in the preset.
	 */
	void MainWindow::showPreset(int index) {
		/* NOTE index can be > last preset in app because the combo box has
	   <New Preset> empty item as its last index */
		if(0 == index && -1 == m_ui->presets->itemData(0).toInt()) {
			/* <New Preset> chosen */
			m_ui->actionRemove->setEnabled(false);
			return;
		}

		if(0 <= index && qyncApp->presetCount() > index) {
			Preset * preset = qyncApp->preset(index);
			Q_ASSERT_X(preset, "MainWindow::showPreset(int)", "Manager provided null preset");
			m_ui->preserveTime->setChecked(preset->preserveTime());
			m_ui->preservePermissions->setChecked(preset->preservePermissions());
			m_ui->preserveOwner->setChecked(preset->preserveOwner());
			m_ui->preserveGroup->setChecked(preset->preserveGroup());

			m_ui->windowsCompatible->setChecked(preset->windowsCompatability());
			m_ui->honourDeletions->setChecked(preset->honourDeletions());

			m_ui->alwaysCompareChecksums->setChecked(preset->alwaysCompareChecksums());
			m_ui->preserveDevices->setChecked(preset->preserveDevices());
			m_ui->keepPartialFiles->setChecked(preset->keepPartialTransfers());
			m_ui->symlinksAsSymlinks->setChecked(preset->copySymlinksAsSymlinks());
			m_ui->makeBackups->setChecked(preset->makeBackups());

			m_ui->compressInTransit->setChecked(preset->useTransferCompression());

			if(preset->onlyUpdateExistingEntries()) {
				m_ui->includeInSynchronisation->setCurrentIndex(OnlyUpdateExisting);
			}
			else if(preset->dontUpdateExistingEntries()) {
				m_ui->includeInSynchronisation->setCurrentIndex(DontUpdateExisting);
			}
			else {
				m_ui->includeInSynchronisation->setCurrentIndex(UpdateEverything);
			}

			m_ui->dontMapUidGid->setChecked(preset->dontMapUsersAndGroups());
			m_ui->hardlinksAsHardlinks->setChecked(preset->copyHardlinksAsHardlinks());
			m_ui->itemisedChanges->setChecked(preset->showItemisedChanges());

			m_ui->source->setText(preset->source());
			m_ui->destination->setText(preset->destination());

			m_ui->logFile->setText(preset->logFile());

			m_ui->actionRemove->setEnabled(true);
		}
	}


	/**
	 * \brief (Re)read the preferences from the manager.
	 *
	 * This slot essentially makes the visual appearance of the GUI match
	 * the settings stored in the manager's preferences object.
	 */
	void MainWindow::readPreferences(void) {
		const GuiPreferences * prefs = dynamic_cast<const GuiPreferences *>(qyncApp->preferences());

		if(prefs) {
			m_ui->presetsToolbar->setToolButtonStyle(prefs->toolBarButtonStyle());
			m_ui->synchroniseToolbar->setToolButtonStyle(prefs->toolBarButtonStyle());

			m_ui->presetsToolbar->setVisible(prefs->showPresetsToolBar());
			m_ui->synchroniseToolbar->setVisible(prefs->showSynchroniseToolBar());
		}
	}


	void MainWindow::disconnectApplication(void) {
		qyncApp->disconnect(this);
	}


	void MainWindow::connectApplication(void) {
		connect(qyncApp, &Application::presetsChanged, this, &MainWindow::refreshPresets);
		connect(qyncApp, &Application::preferencesChanged, this, &MainWindow::readPreferences);
	}


	/**
	 * \brief Refresh the presets combo box.
	 *
	 * The list of presets in the combo box is discarded and it is
	 * repopulated with the set of presets queried from the manager.
	 */
	void MainWindow::refreshPresets(void) {
		m_ui->presets->clear();
		m_ui->menuMyPresets->clear();

		int i = 0;

		for(const Preset * preset : qyncApp->presets()) {
			m_ui->presets->addItem(preset->name());
			QAction * action = m_ui->menuMyPresets->addAction(preset->name());
			connect(action, &QAction::triggered, this, &MainWindow::showPresetFromMenu);
			action->setData(i);
			++i;
		}

		if(0 == i) {
			m_ui->presets->addItem(tr("<New Preset>"));
			m_ui->presets->setItemData(0, -1);
		}
	}


	void MainWindow::chooseSourceFile(void) {
		QString newSource = QFileDialog::getOpenFileName(this, tr("Choose source"), m_ui->source->text());
		if(!newSource.isNull())
			m_ui->source->setText(newSource);
	}


	void MainWindow::chooseDestinationFile(void) {
		QString newDest = QFileDialog::getOpenFileName(this, tr("Choose destination"), m_ui->destination->text());
		if(!newDest.isNull())
			m_ui->destination->setText(newDest);
	}


	void MainWindow::chooseSourceDirectory(void) {
		QString newSource = QFileDialog::getExistingDirectory(this, tr("Choose source"), m_ui->source->text());
		if(!newSource.isNull()) {
			if(!newSource.endsWith(("/")))
				newSource.append("/");
			m_ui->source->setText(newSource);
		}
	}


	void MainWindow::chooseDestinationDirectory(void) {
		QString newDest = QFileDialog::getExistingDirectory(this, tr("Choose destination"), m_ui->destination->text());
		if(!newDest.isNull()) {
			if(!newDest.endsWith(("/")))
				newDest.append("/");
			m_ui->destination->setText(newDest);
		}
	}


	void MainWindow::chooseLogFile(void) {
		QString newLog = QFileDialog::getSaveFileName(this, tr("Choose log file"), m_ui->logFile->text());
		if(!newLog.isNull())
			m_ui->logFile->setText(newLog);
	}


	void MainWindow::switchSourceAndDestination(void) {
		QString t = m_ui->source->text();
		m_ui->source->setText(m_ui->destination->text());
		m_ui->destination->setText(t);
	}


	/**
	 * \brief Save the current settings to the currently-selected preset.
	 *
	 * The current preset is retrieved from the manager and updated with
	 * the settings set in all the widgets in the window.
	 */
	void MainWindow::saveSettingsToCurrentPreset(void) {
		int i = m_ui->presets->currentIndex();

		if(0 == i && -1 == m_ui->presets->itemData(0)) {
			/* saving to <new preset> items so creaet a new one instead */
			newPresetFromSettings();
			return;
		}

		Preset * oldPreset = qyncApp->preset(i);

		if(!oldPreset) {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The selected preset could not be replaced as it could not be found."));
			return;
		}

		/* get current settings */
		Preset temp;
		fillPreset(temp);
		const QMetaObject * mo = oldPreset->metaObject();

		/* copy current settings into original preset */
		oldPreset->setSource(temp.source());
		oldPreset->setDestination(temp.destination());

		for(i = mo->propertyOffset(); i < mo->propertyCount(); i++)
			oldPreset->setProperty(mo->property(i).name(), temp.property(mo->property(i).name()));

		/* save and redisplay original preset */
		oldPreset->save();
		showPreset(m_ui->presets->currentIndex());
	}


	/**
	 * \brief Remove the currently selected preset.
	 *
	 * The current preset is removed from the manager. This triggers a
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
	 * created using the current settings and added to the manager.
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


	void MainWindow::importPreset(void) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Import %1 preset").arg(qyncApp->applicationDisplayName()));

		if(!fileName.isEmpty()) {
			Preset * p = Preset::load(fileName);

			if(!p) {
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The file \"%1\" was not a valid %2 preset file.").arg(fileName).arg(qyncApp->applicationDisplayName()), QMessageBox::Ok);
				return;
			}

			/* don't refresh on presetesChanged() signal from manager - we'll do it ourselves */
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

			Preset temp;
			fillPreset(temp);
			QString name;

			if(m_ui->presets->count() > 0) {
				Preset * myPreset = qyncApp->preset(m_ui->presets->currentIndex());

				if(myPreset) {
					name = myPreset->name();
				}
			}

			/* name is as current preset; if no preset or preset has no name (?),
		 * use the basename of the save path chosen by user */
			if(name.isNull()) {
				name = QFileInfo(fileName).baseName();
			}

			temp.setName(name);

			if(!temp.saveCopyAs(fileName)) {
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The %1 preset could not be exported to the file \"%2\".").arg(qyncApp->applicationDisplayName()).arg(fileName));
			}
		}
	}


	void MainWindow::fillPreset(Preset & p) const {
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

		switch(m_ui->includeInSynchronisation->currentIndex()) {
			case OnlyUpdateExisting:
				p.setOnlyUpdateExistingEntries(true);
				p.setDontUpdateExistingEntries(false);
				break;

			case DontUpdateExisting:
				p.setOnlyUpdateExistingEntries(false);
				p.setDontUpdateExistingEntries(true);
				break;

			default:
				qWarning() << "unexpeced selected index" << m_ui->includeInSynchronisation->currentIndex() << "in \"what to sync\" combo box";
			case UpdateEverything:
				p.setOnlyUpdateExistingEntries(false);
				p.setDontUpdateExistingEntries(false);
				break;
		}

		p.setDontMapUsersAndGroups(m_ui->dontMapUidGid->isChecked());
		p.setCopyHardlinksAsHardlinks(m_ui->hardlinksAsHardlinks->isChecked());
		p.setShowItemisedChanges(m_ui->itemisedChanges->isChecked());

		p.setSource(m_ui->source->text());
		p.setDestination(m_ui->destination->text());

		p.setLogFile(m_ui->logFile->text());
	}


	/**
	 * \brief Start a simulation based on the current settings.
	 */
	void MainWindow::simulate(void) {
		/* even though it's (currently) passed by raw ptr, the temp Process is not
		 * owned by the Process created by Application::simulate() */
		Preset temp;
		fillPreset(temp);
		Process * process = qyncApp->simulate(&temp);

		if(process) {
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
		/* even though it's (currently) passed by raw ptr, the temp Process is not
		 * owned by the Process created by Application::execute() */
		Preset temp;
		fillPreset(temp);
		Process * process = qyncApp->synchronise(&temp);

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


	void MainWindow::showPreferences(void) {
		m_prefsWindow->show();
		m_prefsWindow->raise();
		m_prefsWindow->activateWindow();
	}


	void MainWindow::about(void) {
		QMessageBox::about(this, tr("About %1").arg(qyncApp->applicationDisplayName()), "<p><big><b>" + qyncApp->applicationDisplayName() + " v" + qyncApp->applicationVersion() + "</b></big><br /><small>" + qyncApp->property("ReleaseDate").toString() + "</small></p><p><a href=\"" + qyncApp->property("ApplicationWebsite").toString() + "\">&lt;" + qyncApp->property("ApplicationWebsite").toString() + "&gt;</a></p><p>A Qt front end for rsync<br />Written by Darren Edale</p><p><small>" + qyncApp->applicationDisplayName() + " uses the Qt toolkit (<a href=\"http://qt-project.org/\">http://qt-project.org/</a>).</small></p>");
	}


	void MainWindow::aboutRsync(void) {
		QMessageBox::information(this, tr("Qync - About rsync"), qyncApp->rsyncVersionText());
	}


}  // namespace Qync
