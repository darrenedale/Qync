/**
 * \file MainWindow.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the MainWindow class.
 *
 * \dep
 * - MainWindow.h
 * - Manager.h
 * - Preferences.h
 * - Preset.h
 * - ProcessDialogue.h
 * - PreferencesDialogue.h
 * - common.h
 * - QDebug
 * - QDir
 * - QString
 * - QIcon
 * - QToolBar
 * - QMenuBar
 * - QMenu
 * - QLabel
 * - QComboBox
 * - QCheckBox
 * - QLineEdit
 * - QToolButton
 * - QTabWidget
 * - QGroupBox
 * - QVBoxLayout
 * - QHBoxLayout
 * - QGridLayout
 * - QMessageBox
 * - QFileDialog
 * - QInputDialog
 * - QMetaObject
 * - QMetaProperty
 * - QApplication
 *
 * \todo
 * - use QIcon::fromTheme() wherever possible
 */

#include "MainWindow.h"

#include <QDebug>
#include <QDir>
#include <QString>
#include <QIcon>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QTabWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QMetaObject>
#include <QMetaProperty>
#include <QApplication>
#include <QStandardPaths>

#include "Manager.h"
#include "GuiPreferences.h"
#include "Preset.h"
#include "ProcessDialogue.h"
#include "PreferencesDialogue.h"
#include "common.h"


namespace Qync {


	MainWindow::MainWindow(Manager * manager)
	  : m_manager(0),
		 m_tempPreset(0),
		 m_prefsWindow(0) {
		setWindowIcon(QIcon(":/icons/application"));
		setWindowTitle(QYNC_APP_NAME);
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		m_tempPreset = new Preset();
		createWidgets();
		setManager(manager);

		if(m_manager) {
			GuiPreferences * newPrefs = new GuiPreferences(Manager::configurationDirectoryPath() + "guipreferences");
			m_manager->setPreferences(newPrefs);
		}
	}


	MainWindow::~MainWindow(void) {
		if(m_tempPreset) {
			delete m_tempPreset;
			m_tempPreset = nullptr;
		}

		if(m_prefsWindow) {
			delete m_prefsWindow;
			m_prefsWindow = nullptr;
		}
	}


	void MainWindow::createWidgets(void) {
		static bool done = false;

		if(!done) {
			done = true;
			QLabel * myLabel;
			m_preset = new QComboBox();
			m_preset->setMinimumWidth(150);
			m_preset->setToolTip(tr("Choose a preset from your saved set."));
			connect(m_preset, SIGNAL(currentIndexChanged(int)), this, SLOT(showPreset(int)));

			m_preserveTime = new QCheckBox(tr("Time"));
			m_preservePerms = new QCheckBox(tr("Permissions"));
			m_preserveOwner = new QCheckBox(tr("Owner"));
			m_preserveGroup = new QCheckBox(tr("Group"));

			m_preserveTime->setToolTip(tr("Make sure destination files or directories have the same timestamps as their sources."));
			m_preservePerms->setToolTip(tr("Make sure destination files or directories have the same access permissions as their sources."));
			m_preserveOwner->setToolTip(tr("Make sure destination files or directories have the same owner as their sources. (See also mapping of UID/GID values in Advanced Settings.)"));
			m_preserveGroup->setToolTip(tr("Make sure destination files or directories have the same group as their sources. (See also mapping of UID/GID values in Advanced Settings.)"));

			m_windowsCompatible = new QCheckBox(tr("Windows Compatible"));
			m_deleteOnServer = new QCheckBox(tr("Honour Deletions"));

			m_windowsCompatible->setToolTip(tr("Windows FAT filesystems only store file modification times to a precision of 2s. Using this setting will force rsync to consider two entries to have been modified at the same time if their modification times differ by less than 2s."));
			m_deleteOnServer->setToolTip(tr("Using this setting will ask rsync to remove files from the destination that have been removed from the source. Only files deleted from within the source tree specified for the source will be affected on the destination."));

			m_alwaysChecksum = new QCheckBox(tr("Always compare checksums"));
			m_preserveDevices = new QCheckBox(tr("Preserve devices"));
			m_keepParitalTransfers = new QCheckBox(tr("Keep partially transferred files"));
			m_symlinksAsSymlinks = new QCheckBox(tr("Copy symlinks as symlinks"));
			m_makeBackups = new QCheckBox(tr("Make backups"));

			m_compressInTransit = new QCheckBox(tr("Compress files in transit"));
			m_onlyUpdateExisting = new QCheckBox(tr("Only update existing files"));
			m_dontUpdateExisting = new QCheckBox(tr("Don't update existing files"));
			m_dontMapUidGid = new QCheckBox(tr("Don't map UID/GID values"));
			m_copyHardlinksAsHardlinks = new QCheckBox(tr("Copy hardlinks as hardlinks"));
			m_showItemisedChanges = new QCheckBox(tr("Show itemised changes"));

			m_alwaysChecksum->setToolTip(tr("Use the checksum of source and desination files rather than their size and modification time to decide which files need to be updated."));
			m_preserveDevices->setToolTip(tr("Source files that are special device files will be created as special device files at the destination. If this option is not selected, source files that are device files will be created as regular files at the destination."));
			m_keepParitalTransfers->setToolTip(tr(""));
			m_symlinksAsSymlinks->setToolTip(tr("Source files and directories that are symbolic links will be created as symbolic links at the destination.\n\nIf this option is not selected, source files and directories that are symbolic links will be created as regular files at the destination."));
			m_makeBackups->setToolTip(tr("Make sure destination files or directories have the same timestamps as their sources."));

			m_compressInTransit->setToolTip(tr("In transit, file data will be compressed to save bandwidth."));
			m_onlyUpdateExisting->setToolTip(tr("Only files and directories that already exist at the destination will be updated; new source files and directories will not be created at the destination."));
			m_dontUpdateExisting->setToolTip(tr("Files and directories that already exist at the destination will be ignored; only new source files and directories will be created at the destination."));
			m_dontMapUidGid->setToolTip(tr("Use the source UID and GID values for destination files and directories rather than attempting to find the UID and GID of a matching named user/group on the destination."));
			m_copyHardlinksAsHardlinks->setToolTip(tr("Source files that are hard links will be created as hard links at the destination.\n\nIf this option is not selected, source files that are hard links will be created as regular files at the destination."));
			m_showItemisedChanges->setToolTip(tr(""));
			connect(m_onlyUpdateExisting, SIGNAL(toggled(bool)), this, SLOT(syncIgnoreExistingToOnlyExisting()));
			connect(m_dontUpdateExisting, SIGNAL(toggled(bool)), this, SLOT(syncOnlyExistingToIgnoreExisting()));

			m_source = new QLineEdit();
			m_source->setToolTip(tr("The source for rsync. It must contain a trailing <b>/</b> if it is a directory."));
			m_dest = new QLineEdit();
			m_dest->setToolTip(tr("The destination for rsync. It must contain a trailing <b>/</b> if it is a directory."));

			m_chooseSourceDir = new QToolButton();
			m_chooseDestinationDir = new QToolButton();
			m_chooseSourceDir->setIcon(QIcon(":/icons/buttons/choosedir"));
			m_chooseDestinationDir->setIcon(QIcon(":/icons/buttons/choosedir"));
			m_chooseSourceDir->setToolTip(tr("Choose an existing directory for the source."));
			m_chooseDestinationDir->setToolTip(tr("Choose an existing directory for the destination."));
			connect(m_chooseSourceDir, SIGNAL(clicked()), this, SLOT(chooseSourceDirectory()));
			connect(m_chooseDestinationDir, SIGNAL(clicked()), this, SLOT(chooseDestinationDirectory()));

			m_chooseSourceFile = new QToolButton();
			m_chooseDestinationFile = new QToolButton();
			m_chooseSourceFile->setIcon(QIcon(":/icons/buttons/choosefile"));
			m_chooseDestinationFile->setIcon(QIcon(":/icons/buttons/choosefile"));
			m_chooseSourceFile->setToolTip(tr("Choose an existing file for the source."));
			m_chooseDestinationFile->setToolTip(tr("Choose an existing file for the destination."));
			connect(m_chooseSourceFile, SIGNAL(clicked()), this, SLOT(chooseSourceFile()));
			connect(m_chooseDestinationFile, SIGNAL(clicked()), this, SLOT(chooseDestinationFile()));

			m_logFile = new QLineEdit();
			m_chooseLogFile = new QToolButton();
			m_chooseLogFile->setIcon(QIcon(":/icons/buttons/choosefile"));
			m_chooseLogFile->setToolTip(tr("Choose the log file."));
			connect(m_chooseLogFile, SIGNAL(clicked()), this, SLOT(chooseLogFile()));

			m_presetsToolbar = new QToolBar("Presets");
			m_presetsToolbar->addWidget(myLabel = new QLabel(tr("Presets")));
			myLabel->setBuddy(m_preset);
			m_presetsToolbar->addWidget(m_preset);

			m_savePresetAction = m_presetsToolbar->addAction(QIcon::fromTheme("document-save", QIcon(":icons/toolbar/save")), "Save", this, SLOT(saveSettingsToCurrentPreset()));
			m_savePresetAction->setToolTip(tr("Save the current settings to the selected preset."));

			m_savePresetAsAction = m_presetsToolbar->addAction(QIcon::fromTheme("document-save-as", QIcon(":icons/toolbar/saveas")), "Save As", this, SLOT(newPresetFromSettings()));
			m_savePresetAsAction->setToolTip(tr("Save the current settings as a new preset."));

			m_removePresetAction = m_presetsToolbar->addAction(QIcon::fromTheme("user-trash", QIcon(":icons/toolbar/remove")), "Remove", this, SLOT(removeCurrentPreset()));
			m_removePresetAction->setToolTip(tr("Remove the selected preset."));

			addToolBar(m_presetsToolbar);

			m_synchroniseToolbar = new QToolBar("Synchronise");

			m_menuFileSimulate = m_synchroniseToolbar->addAction(QIcon::fromTheme("document-edit-verify", QIcon(":icons/toolbar/simulate")), "Simulate", this, SLOT(simulate()));
			m_menuFileSimulate->setToolTip(tr("Perform a dry-run (simulation) of the current settings."));

			m_menuFileExecute = m_synchroniseToolbar->addAction(QIcon::fromTheme("system-run", QIcon(":icons/toolbar/execute")), "Sync", this, SLOT(execute()));
			m_menuFileExecute->setToolTip(tr("Run rsync with the current settings."));

			addToolBar(m_synchroniseToolbar);

			QMenu * menu = new QMenu(tr("File"));
			m_menuFileChooseSource = menu->addAction(QIcon::fromTheme("folder", QIcon(":icons/menu/file/choosedir")), tr("Choose source folder"), this, SLOT(chooseSourceDirectory()));
			m_menuFileChooseDest = menu->addAction(QIcon::fromTheme("folder", QIcon(":icons/menu/file/choosedir")), tr("Choose destination folder"), this, SLOT(chooseDestinationDirectory()));
			m_menuFileSwitchSourceAndDest = menu->addAction(QIcon::fromTheme("", QIcon(":icons/menu/file/switch")), tr("Switch source and destination"), this, SLOT(switchSourceAndDestination()));
			menu->addSeparator();
			menu->addAction(m_menuFileSimulate);
			menu->addAction(m_menuFileExecute);
			menu->addSeparator();
			menu->addAction(QIcon::fromTheme("configure", QIcon(":icons/menu/file/preferences")), tr("Preferences"), this, SLOT(showPreferences()));
			menu->addSeparator();
			m_menuFileQuit = menu->addAction(QIcon::fromTheme("application-exit", QIcon(":icons/menu/file/quit")), tr("Quit"), this, SLOT(close()));
			m_menuFileQuit->setMenuRole(QAction::QuitRole); /* OSX only */
			menuBar()->addMenu(menu);

			menu = new QMenu(tr("Presets"));
			m_presetsMenu = new QMenu(tr("My Presets"));
			menu->addMenu(m_presetsMenu);
			menu->addSeparator();
			menu->addAction(m_savePresetAction);
			menu->addAction(m_savePresetAsAction);
			menu->addAction(m_removePresetAction);
			menu->addSeparator();
			menu->addAction(QIcon::fromTheme("document-import", QIcon(":/icons/menu/presets/import")), tr("Import..."), this, SLOT(importPreset()));
			menu->addAction(QIcon::fromTheme("document-export", QIcon(":/icons/menu/presets/export")), tr("Export..."), this, SLOT(exportPreset()));
			menuBar()->addMenu(menu);

			menu = new QMenu(tr("Help"));
			QAction * action = menu->addAction(QIcon::fromTheme("help-about", QIcon(":/icons/menu/help/about")), tr("About"), this, SLOT(about()));
			action->setMenuRole(QAction::AboutRole); /* OSX only */
			action = menu->addAction(QIcon(":/icons/menu/help/aboutqt"), tr("About Qt"), qApp, SLOT(aboutQt()));
			action->setMenuRole(QAction::AboutQtRole); /* OSX only */
			menu->addAction(QIcon(":/icons/menu/help/aboutrsync"), tr("About rsync"), this, SLOT(aboutRsync()));
			menuBar()->addMenu(menu);

			QTabWidget * container = new QTabWidget();
			QVBoxLayout * vLayout = new QVBoxLayout();
			QGridLayout * gLayout = new QGridLayout();
			gLayout->addWidget(myLabel = new QLabel(tr("Source")), 0, 0);
			myLabel->setBuddy(m_source);
			gLayout->addWidget(m_source, 0, 1);
			gLayout->addWidget(m_chooseSourceDir, 0, 2);
			gLayout->addWidget(m_chooseSourceFile, 0, 3);

			gLayout->addWidget(myLabel = new QLabel(tr("Destination")), 1, 0);
			myLabel->setBuddy(m_dest);
			gLayout->addWidget(m_dest, 1, 1);
			gLayout->addWidget(m_chooseDestinationDir, 1, 2);
			gLayout->addWidget(m_chooseDestinationFile, 1, 3);

			vLayout->addLayout(gLayout);

			QGroupBox * group = new QGroupBox(tr("Preserve"));
			gLayout = new QGridLayout();
			gLayout->addWidget(m_preserveTime, 0, 0);
			gLayout->addWidget(m_preservePerms, 1, 0);
			gLayout->addWidget(m_preserveOwner, 0, 1);
			gLayout->addWidget(m_preserveGroup, 1, 1);

			group->setLayout(gLayout);
			vLayout->addWidget(group);

			group = new QGroupBox();
			gLayout = new QGridLayout();
			gLayout->addWidget(m_windowsCompatible, 0, 0);
			gLayout->addWidget(m_deleteOnServer, 0, 1);
			group->setLayout(gLayout);
			vLayout->addWidget(group);

			vLayout->addStretch(2);

			QWidget * page = new QWidget();
			page->setLayout(vLayout);

			container->addTab(page, QIcon::fromTheme("preferences-other", QIcon(":/icons/tabs/basic_settings")), tr("Basic Settings"));

			gLayout = new QGridLayout();
			gLayout->addWidget(m_alwaysChecksum, 0, 0);
			gLayout->addWidget(m_preserveDevices, 1, 0);
			gLayout->addWidget(m_onlyUpdateExisting, 2, 0);
			gLayout->addWidget(m_keepParitalTransfers, 3, 0);
			gLayout->addWidget(m_symlinksAsSymlinks, 4, 0);
			gLayout->addWidget(m_makeBackups, 5, 0);

			gLayout->addWidget(m_compressInTransit, 0, 1);
			gLayout->addWidget(m_dontUpdateExisting, 2, 1);
			gLayout->addWidget(m_dontMapUidGid, 3, 1);
			gLayout->addWidget(m_copyHardlinksAsHardlinks, 4, 1);
			gLayout->addWidget(m_showItemisedChanges, 5, 1);

			QHBoxLayout * hLayout = new QHBoxLayout();
			hLayout->addWidget(myLabel = new QLabel(tr("Log file")));
			myLabel->setBuddy(m_logFile);
			hLayout->addWidget(m_logFile);
			hLayout->addWidget(m_chooseLogFile);
			gLayout->addLayout(hLayout, 6, 0, 1, 2, Qt::AlignLeft);

			vLayout = new QVBoxLayout();
			vLayout->addLayout(gLayout);
			vLayout->addStretch(2);

			page = new QWidget();
			page->setLayout(vLayout);

			container->addTab(page, QIcon::fromTheme("preferences-system", QIcon(":/icons/tabs/advanced_settings")), tr("Advanced Settings"));

			setCentralWidget(container);
		}
	}


	void MainWindow::showPresetFromMenu(void) {
		if(!m_manager) {
			qDebug() << "MainWindow has no Manager";
			return;
		}

		QAction * action = qobject_cast<QAction *>(sender());

		if(!action) {
			qDebug() << "showPresetFromMenu() slot call did not result from trigger of action from My Presets menu";
			return;
		}

		int index = action->data().toInt();

		if(index < 0 || index >= m_manager->presetCount()) {
			qDebug() << "action from My Presets menu contains invalid preset index";
			refreshPresets();
			return;
		}

		m_preset->setCurrentIndex(index);
	}


	void MainWindow::showPreset(int index) {
		if(!m_manager) {
			qDebug() << "MainWindow has no Manager";
			return;
		}

		/* NOTE index can be > last preset in manager because the combo box has
	   <New Preset> empty item as its last index */
		if(index >= 0 && index < m_manager->presetCount()) {
			Preset * preset = m_manager->preset(index);
			Q_ASSERT_X(preset, "MainWindow::showPreset(int)", "Manager provided null preset");
			m_preserveTime->setChecked(preset->preserveTime());
			m_preservePerms->setChecked(preset->preservePermissions());
			m_preserveOwner->setChecked(preset->preserveOwner());
			m_preserveGroup->setChecked(preset->preserveGroup());

			m_windowsCompatible->setChecked(preset->windowsCompatability());
			m_deleteOnServer->setChecked(preset->honourDeletions());

			m_alwaysChecksum->setChecked(preset->alwaysCompareChecksums());
			m_preserveDevices->setChecked(preset->preserveDevices());
			m_keepParitalTransfers->setChecked(preset->keepPartialTransfers());
			m_symlinksAsSymlinks->setChecked(preset->copySymlinksAsSymlinks());
			m_makeBackups->setChecked(preset->makeBackups());

			m_compressInTransit->setChecked(preset->useTransferCompression());
			m_onlyUpdateExisting->setChecked(preset->onlyUpdateExistingEntries());
			m_dontUpdateExisting->setChecked(preset->dontUpdateExistingEntries());
			m_dontMapUidGid->setChecked(preset->dontMapUsersAndGroups());
			m_copyHardlinksAsHardlinks->setChecked(preset->copyHardlinksAsHardlinks());
			m_showItemisedChanges->setChecked(preset->showItemisedChanges());

			m_source->setText(preset->source());
			m_dest->setText(preset->destination());

			m_logFile->setText(preset->logFile());

			m_removePresetAction->setEnabled(true);
		}
		else {
			m_removePresetAction->setEnabled(false);
		}
	}


	void MainWindow::readPreferences(void) {
		if(!m_manager) {
			qDebug() << "MainWindow has no Manager";
			return;
		}

		const GuiPreferences * prefs = dynamic_cast<const GuiPreferences *>(m_manager->preferences());

		if(prefs) {
			m_presetsToolbar->setToolButtonStyle(prefs->toolBarButtonStyle());
			m_synchroniseToolbar->setToolButtonStyle(prefs->toolBarButtonStyle());

			if(prefs->showPresetsToolBar())
				m_presetsToolbar->show();
			else
				m_presetsToolbar->hide();

			if(prefs->showSynchroniseToolBar())
				m_synchroniseToolbar->show();
			else
				m_synchroniseToolbar->hide();
		}
	}


	void MainWindow::syncIgnoreExistingToOnlyExisting(void) {
		if(m_onlyUpdateExisting->isChecked()) {
			m_dontUpdateExisting->setChecked(false);
		}
	}


	void MainWindow::syncOnlyExistingToIgnoreExisting(void) {
		if(m_dontUpdateExisting->isChecked()) {
			m_onlyUpdateExisting->setChecked(false);
		}
	}


	void MainWindow::disconnectManager(void) {
		if(m_manager) {
			disconnect(m_manager, SIGNAL(presetsChanged()), this, SLOT(refreshPresets()));
			disconnect(m_manager, SIGNAL(preferencesChanged()), this, SLOT(readPreferences()));
		}
	}


	void MainWindow::connectManager(void) {
		if(m_manager) {
			connect(m_manager, SIGNAL(presetsChanged()), this, SLOT(refreshPresets()));
			connect(m_manager, SIGNAL(preferencesChanged()), this, SLOT(readPreferences()));
		}
	}


	Manager * MainWindow::manager(void) const {
		return m_manager;
	}


	bool MainWindow::setManager(Manager * manager) {
		disconnectManager();
		m_manager = manager;
		connectManager();
		refreshPresets();
		readPreferences();
		return true;
	}


	void MainWindow::refreshPresets(void) {
		m_preset->clear();
		m_presetsMenu->clear();

		if(m_manager) {
			int i = 0;

			for(Preset * preset : m_manager->presets()) {
				m_preset->addItem(preset->name());
				QAction * action = m_presetsMenu->addAction(preset->name());  //, this, SLOT(showPresetFromMenu()));
				connect(action, &QAction::triggered, this, &MainWindow::showPresetFromMenu);
				action->setData(i);
				i++;
			}
		}

		m_preset->addItem(tr("<New Preset>"));
	}


	void MainWindow::chooseSourceFile(void) {
		QString newSource = QFileDialog::getOpenFileName(this, tr("Choose source"), m_source->text());
		if(!newSource.isNull())
			m_source->setText(newSource);
	}


	void MainWindow::chooseDestinationFile(void) {
		QString newDest = QFileDialog::getOpenFileName(this, tr("Choose destination"), m_dest->text());
		if(!newDest.isNull())
			m_dest->setText(newDest);
	}


	void MainWindow::chooseSourceDirectory(void) {
		QString newSource = QFileDialog::getExistingDirectory(this, tr("Choose source"), m_source->text());
		if(!newSource.isNull()) {
			if(!newSource.endsWith(("/")))
				newSource.append("/");
			m_source->setText(newSource);
		}
	}


	void MainWindow::chooseDestinationDirectory(void) {
		QString newDest = QFileDialog::getExistingDirectory(this, tr("Choose destination"), m_dest->text());
		if(!newDest.isNull()) {
			if(!newDest.endsWith(("/")))
				newDest.append("/");
			m_dest->setText(newDest);
		}
	}


	void MainWindow::chooseLogFile(void) {
		QString newLog = QFileDialog::getSaveFileName(this, tr("Choose log file"), m_logFile->text());
		if(!newLog.isNull())
			m_logFile->setText(newLog);
	}


	void MainWindow::switchSourceAndDestination(void) {
		QString t = m_source->text();
		m_source->setText(m_dest->text());
		m_dest->setText(t);
	}


	void MainWindow::saveSettingsToCurrentPreset(void) {
		int i = m_preset->currentIndex();

		if(i >= m_preset->count() - 1) {
			newPresetFromSettings();
			return;
		}

		Preset * oldPreset = m_manager->preset(i);

		if(!oldPreset) {
			QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The selected preset could not be replaced as it could not be found."));
			return;
		}

		/* get current settings */
		fillPreset(m_tempPreset);
		const QMetaObject * mo = oldPreset->metaObject();

		/* copy current settings into original preset */
		oldPreset->setSource(m_tempPreset->source());
		oldPreset->setDestination(m_tempPreset->destination());

		for(i = mo->propertyOffset(); i < mo->propertyCount(); i++)
			oldPreset->setProperty(mo->property(i).name(), m_tempPreset->property(mo->property(i).name()));

		/* save and redisplay original preset */
		oldPreset->save();
		showPreset(m_preset->currentIndex());
	}


	void MainWindow::removeCurrentPreset(void) {
		if(m_manager->presetCount() < 1) {
			QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("There are no presets to remove."));
			return;
		}

		if(!m_manager->removePreset(m_preset->currentIndex())) {
			QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The selected preset could not be removed:\n\n%1").arg(m_manager->lastError()));
			return;
		}
	}


	void MainWindow::newPresetFromSettings(void) {
		bool ok;
		QString name = QInputDialog::getText(this, tr("%1 Request").arg(QYNC_APP_NAME), tr("Enter a name for the new preset"), QLineEdit::Normal, tr("New Preset"), &ok);

		if(ok) {
			Preset * p = new Preset();
			fillPreset(p);
			p->setName(name);
			disconnectManager(); /* don't refresh on presetesChanged() signal from manager - we'll do it ourselves */

			if(m_manager->addPreset(p)) {
				refreshPresets();
				m_preset->setCurrentIndex(m_preset->count() - 2);
			}
			else {
				delete p;
				QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The new preset could not be created:\n\n%1").arg(m_manager->lastError()));
			}

			connectManager();
		}
	}


	void MainWindow::importPreset(void) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Import %1 preset").arg(QYNC_APP_NAME));

		if(!fileName.isEmpty()) {
			Preset * p = Preset::load(fileName);

			if(!p) {
				QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The file \"%1\" was not a valid %2 preset file.").arg(fileName).arg(QYNC_APP_NAME), QMessageBox::Ok);
				return;
			}

			/* don't refresh on presetesChanged() signal from manager - we'll do it ourselves */
			disconnectManager();

			if(m_manager->addPreset(p)) {
				refreshPresets();
				m_preset->setCurrentIndex(m_preset->count() - 1);
			}
			else {
				delete p;
				QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The imported %1 preset could not be created:\n\n%2").arg(QYNC_APP_NAME).arg(m_manager->lastError()));
			}

			connectManager();
		}
	}


	void MainWindow::exportPreset(void) {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Export %1 preset").arg(QYNC_APP_NAME));

		if(!fileName.isEmpty()) {
			QFileInfo f(fileName);

			if(f.exists()) {
				if(f.isDir()) {
					QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The path you selected is a directory. You cannot save a %1 preset over a director.").arg(QYNC_APP_NAME));
					return;
				}
			}

			fillPreset(m_tempPreset);
			QString name;

			if(m_preset->count() > 0) {
				Preset * myPreset = m_manager->preset(m_preset->currentIndex());
				if(myPreset)
					name = myPreset->name();
			}

			/* name is as current preset; if no preset or preset has no name (?),
		 * use the basename of the save path chosen by user */
			if(name.isNull())
				name = QFileInfo(fileName).baseName();

			m_tempPreset->setName(name);

			if(!m_tempPreset->saveCopyAs(fileName))
				QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The %1 preset could not be exported to the file \"%2\".").arg(QYNC_APP_NAME).arg(fileName));
		}
	}


	void MainWindow::fillPreset(Preset * p) const {
		if(!p)
			return;

		p->setPreserveGroup(m_preserveGroup->isChecked());
		p->setPreserveOwner(m_preserveOwner->isChecked());
		p->setPreservePermissions(m_preservePerms->isChecked());
		p->setPreserveTime(m_preserveTime->isChecked());

		p->setWindowsCompatability(m_windowsCompatible->isChecked());
		p->setHonourDeletions(m_deleteOnServer->isChecked());

		p->setAlwaysCompareChecksums(m_alwaysChecksum->isChecked());
		p->setPreserveDevices(m_preserveDevices->isChecked());
		p->setKeepPartialTransfers(m_keepParitalTransfers->isChecked());
		p->setCopySymlinksAsSymlinks(m_symlinksAsSymlinks->isChecked());
		p->setMakeBackups(m_makeBackups->isChecked());

		p->setUseTransferCompression(m_compressInTransit->isChecked());
		p->setOnlyUpdateExistingEntries(m_onlyUpdateExisting->isChecked());
		p->setDontUpdateExistingEntries(m_dontUpdateExisting->isChecked());
		p->setDontMapUsersAndGroups(m_dontMapUidGid->isChecked());
		p->setCopyHardlinksAsHardlinks(m_copyHardlinksAsHardlinks->isChecked());
		p->setShowItemisedChanges(m_showItemisedChanges->isChecked());

		p->setSource(m_source->text());
		p->setDestination(m_dest->text());

		p->setLogFile(m_logFile->text());
	}


	void MainWindow::simulate(void) {
		fillPreset(m_tempPreset);
		Process * process = m_manager->simulate(m_tempPreset);

		if(process) {
			ProcessDialogue * w = new ProcessDialogue(process, this);
			w->setWindowTitle(tr("%1 Simulation: %2").arg(QYNC_APP_NAME).arg(m_preset->currentText()));
			w->show();
			process->start();
		}
		else
			QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), "The simulation failed:\n\n" + m_manager->lastError());
	}


	void MainWindow::execute(void) {
		fillPreset(m_tempPreset);
		Process * process = m_manager->execute(m_tempPreset);

		if(process) {
			ProcessDialogue * w = new ProcessDialogue(process, this);
			w->setWindowTitle(tr("%1 Synchronisation: %2").arg(QYNC_APP_NAME).arg(m_preset->currentText()));
			w->show();
			process->start();
		}
		else
			QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), "The synchronisation failed:\n\n" + m_manager->lastError());
	}


	void MainWindow::showPreferences(void) {
		if(!m_prefsWindow) {
			m_prefsWindow = new PreferencesDialogue(m_manager, this);
			m_prefsWindow->setWindowTitle(tr("%1 Preferences").arg(QYNC_APP_NAME));
		}

		if(!m_prefsWindow) {
			qDebug() << "failed to create prefs dialogue - out of memory?";
			QMessageBox::critical(this, tr("% Error").arg(QYNC_APP_NAME), tr("It was not possible to show the preferences window. Perhaps your system is running out of available memory?"), QMessageBox::Ok);
			return;
		}

		m_prefsWindow->show();
		m_prefsWindow->raise();
		m_prefsWindow->activateWindow();
	}


	void MainWindow::about(void) {
		QMessageBox::about(this, tr("About %1").arg(QYNC_APP_NAME),
								 "<p><big><b>" QYNC_APP_NAME " v" QYNC_VERSION_STRING "</b></big><br />"
								 "<small>" QYNC_VERSION_DATE "</small></p>"

								 "<p><a href=\"" QYNC_URL "\">&lt;" QYNC_URL "&gt;</a></p>"

								 "<p>A Qt front end for rsync<br />"
								 "Written by Darren Edale</p>"

								 "<p><small>" QYNC_APP_NAME " uses the Qt toolkit (<a href=\"http://qt-project.org/\">http://qt-project.org/</a>).</small></p>");
	}


	void MainWindow::aboutRsync(void) {
		QMessageBox::information(this, tr("Qync - About rsync"), m_manager->rsyncVersionText());
	}


}  // namespace Qync
