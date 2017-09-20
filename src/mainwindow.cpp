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


		GuiPreferences * newPrefs = new GuiPreferences(Application::configurationDirectoryPath() + "guipreferences");
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


	//	void MainWindow::createWidgets(void) {
	//		static bool done = false;

	//		if(!done) {
	//			done = true;
	//			QLabel * myLabel;
	//			m_preset = new QComboBox();
	//			m_ui->presets->setMinimumWidth(150);
	//			m_ui->presets->setToolTip(tr("Choose a preset from your saved set."));
	//			connect(m_preset, SIGNAL(currentIndexChanged(int)), this, SLOT(showPreset(int)));

	//			m_preserveTime = new QCheckBox(tr("Time"));
	//			m_preservePerms = new QCheckBox(tr("Permissions"));
	//			m_preserveOwner = new QCheckBox(tr("Owner"));
	//			m_preserveGroup = new QCheckBox(tr("Group"));

	//			m_ui->preserveTime->setToolTip(tr("Make sure destination files or directories have the same timestamps as their sources."));
	//			m_ui->preservePermissions->setToolTip(tr("Make sure destination files or directories have the same access permissions as their sources."));
	//			m_ui->preserveOwner->setToolTip(tr("Make sure destination files or directories have the same owner as their sources. (See also mapping of UID/GID values in Advanced Settings.)"));
	//			m_ui->preserveGroup->setToolTip(tr("Make sure destination files or directories have the same group as their sources. (See also mapping of UID/GID values in Advanced Settings.)"));

	//			m_windowsCompatible = new QCheckBox(tr("Windows Compatible"));
	//			m_deleteOnServer = new QCheckBox(tr("Honour Deletions"));

	//			m_ui->windowsCompatible->setToolTip(tr("Windows FAT filesystems only store file modification times to a precision of 2s. Using this setting will force rsync to consider two entries to have been modified at the same time if their modification times differ by less than 2s."));
	//			m_ui->honourDeletions->setToolTip(tr("Using this setting will ask rsync to remove files from the destination that have been removed from the source. Only files deleted from within the source tree specified for the source will be affected on the destination."));

	//			m_alwaysChecksum = new QCheckBox(tr("Always compare checksums"));
	//			m_preserveDevices = new QCheckBox(tr("Preserve devices"));
	//			m_keepParitalTransfers = new QCheckBox(tr("Keep partially transferred files"));
	//			m_symlinksAsSymlinks = new QCheckBox(tr("Copy symlinks as symlinks"));
	//			m_makeBackups = new QCheckBox(tr("Make backups"));

	//			m_compressInTransit = new QCheckBox(tr("Compress files in transit"));
	//			m_onlyUpdateExisting = new QCheckBox(tr("Only update existing files"));
	//			m_dontUpdateExisting = new QCheckBox(tr("Don't update existing files"));
	//			m_dontMapUidGid = new QCheckBox(tr("Don't map UID/GID values"));
	//			m_copyHardlinksAsHardlinks = new QCheckBox(tr("Copy hardlinks as hardlinks"));
	//			m_showItemisedChanges = new QCheckBox(tr("Show itemised changes"));

	//			m_ui->alwaysCompareChecksums->setToolTip(tr("Use the checksum of source and desination files rather than their size and modification time to decide which files need to be updated."));
	//			m_ui->preserveDevices->setToolTip(tr("Source files that are special device files will be created as special device files at the destination. If this option is not selected, source files that are device files will be created as regular files at the destination."));
	//			m_ui->keepPartialFiles->setToolTip(tr(""));
	//			m_ui->symlinksAsSymlinks->setToolTip(tr("Source files and directories that are symbolic links will be created as symbolic links at the destination.\n\nIf this option is not selected, source files and directories that are symbolic links will be created as regular files at the destination."));
	//			m_ui->makeBackups->setToolTip(tr("Make sure destination files or directories have the same timestamps as their sources."));

	//			m_ui->compressInTransit->setToolTip(tr("In transit, file data will be compressed to save bandwidth."));
	//			m_ui->onlyUpdateExisting->setToolTip(tr("Only files and directories that already exist at the destination will be updated; new source files and directories will not be created at the destination."));
	//			m_dontUpdateExisting->setToolTip(tr("Files and directories that already exist at the destination will be ignored; only new source files and directories will be created at the destination."));
	//			m_ui->dontMapUidGid->setToolTip(tr("Use the source UID and GID values for destination files and directories rather than attempting to find the UID and GID of a matching named user/group on the destination."));
	//			m_ui->hardlinksAsHardlinks->setToolTip(tr("Source files that are hard links will be created as hard links at the destination.\n\nIf this option is not selected, source files that are hard links will be created as regular files at the destination."));
	//			m_ui->itemisedChanges->setToolTip(tr(""));
	//			connect(m_onlyUpdateExisting, SIGNAL(toggled(bool)), this, SLOT(syncIgnoreExistingToOnlyExisting()));
	//			connect(m_dontUpdateExisting, SIGNAL(toggled(bool)), this, SLOT(syncOnlyExistingToIgnoreExisting()));

	//			m_source = new QLineEdit();
	//			m_ui->source->setToolTip(tr("The source for rsync. It must contain a trailing <b>/</b> if it is a directory."));
	//			m_dest = new QLineEdit();
	//			m_ui->destination->setToolTip(tr("The destination for rsync. It must contain a trailing <b>/</b> if it is a directory."));

	//			m_chooseSourceDir = new QToolButton();
	//			m_chooseDestinationDir = new QToolButton();
	//			m_chooseSourceDir->setIcon(QIcon(":/icons/buttons/choosedir"));
	//			m_chooseDestinationDir->setIcon(QIcon(":/icons/buttons/choosedir"));
	//			m_chooseSourceDir->setToolTip(tr("Choose an existing directory for the source."));
	//			m_chooseDestinationDir->setToolTip(tr("Choose an existing directory for the destination."));
	//			connect(m_chooseSourceDir, SIGNAL(clicked()), this, SLOT(chooseSourceDirectory()));
	//			connect(m_chooseDestinationDir, SIGNAL(clicked()), this, SLOT(chooseDestinationDirectory()));

	//			m_chooseSourceFile = new QToolButton();
	//			m_chooseDestinationFile = new QToolButton();
	//			m_chooseSourceFile->setIcon(QIcon(":/icons/buttons/choosefile"));
	//			m_chooseDestinationFile->setIcon(QIcon(":/icons/buttons/choosefile"));
	//			m_chooseSourceFile->setToolTip(tr("Choose an existing file for the source."));
	//			m_chooseDestinationFile->setToolTip(tr("Choose an existing file for the destination."));
	//			connect(m_chooseSourceFile, SIGNAL(clicked()), this, SLOT(chooseSourceFile()));
	//			connect(m_chooseDestinationFile, SIGNAL(clicked()), this, SLOT(chooseDestinationFile()));

	//			m_logFile = new QLineEdit();
	//			m_chooseLogFile = new QToolButton();
	//			m_chooseLogFile->setIcon(QIcon(":/icons/buttons/choosefile"));
	//			m_chooseLogFile->setToolTip(tr("Choose the log file."));
	//			connect(m_chooseLogFile, SIGNAL(clicked()), this, SLOT(chooseLogFile()));

	//			m_ui->presetsToolbar = new QToolBar("Presets");
	//			m_ui->presetsToolbar->addWidget(myLabel = new QLabel(tr("Presets")));
	//			myLabel->setBuddy(m_preset);
	//			m_ui->presetsToolbar->addWidget(m_preset);

	//			m_savePresetAction = m_ui->presetsToolbar->addAction(QIcon::fromTheme("document-save", QIcon(":icons/toolbar/save")), "Save", this, SLOT(saveSettingsToCurrentPreset()));
	//			m_savePresetAction->setToolTip(tr("Save the current settings to the selected preset."));

	//			m_savePresetAsAction = m_ui->presetsToolbar->addAction(QIcon::fromTheme("document-save-as", QIcon(":icons/toolbar/saveas")), "Save As", this, SLOT(newPresetFromSettings()));
	//			m_savePresetAsAction->setToolTip(tr("Save the current settings as a new preset."));

	//			m_removePresetAction = m_ui->presetsToolbar->addAction(QIcon::fromTheme("user-trash", QIcon(":icons/toolbar/remove")), "Remove", this, SLOT(removeCurrentPreset()));
	//			m_removePresetAction->setToolTip(tr("Remove the selected preset."));

	//			addToolBar(m_ui->presetsToolbar);

	//			m_ui->synchroniseToolbar = new QToolBar("Synchronise");

	//			m_menuFileSimulate = m_ui->synchroniseToolbar->addAction(QIcon::fromTheme("document-edit-verify", QIcon(":icons/toolbar/simulate")), "Simulate", this, SLOT(simulate()));
	//			m_menuFileSimulate->setToolTip(tr("Perform a dry-run (simulation) of the current settings."));

	//			m_menuFileExecute = m_ui->synchroniseToolbar->addAction(QIcon::fromTheme("system-run", QIcon(":icons/toolbar/execute")), "Sync", this, SLOT(execute()));
	//			m_menuFileExecute->setToolTip(tr("Run rsync with the current settings."));

	//			addToolBar(m_ui->synchroniseToolbar);

	//			QMenu * menu = new QMenu(tr("File"));
	//			m_menuFileChooseSource = menu->addAction(QIcon::fromTheme("folder", QIcon(":icons/menu/file/choosedir")), tr("Choose source folder"), this, SLOT(chooseSourceDirectory()));
	//			m_menuFileChooseDest = menu->addAction(QIcon::fromTheme("folder", QIcon(":icons/menu/file/choosedir")), tr("Choose destination folder"), this, SLOT(chooseDestinationDirectory()));
	//			m_menuFileSwitchSourceAndDest = menu->addAction(QIcon::fromTheme("", QIcon(":icons/menu/file/switch")), tr("Switch source and destination"), this, SLOT(switchSourceAndDestination()));
	//			menu->addSeparator();
	//			menu->addAction(m_menuFileSimulate);
	//			menu->addAction(m_menuFileExecute);
	//			menu->addSeparator();
	//			menu->addAction(QIcon::fromTheme("configure", QIcon(":icons/menu/file/preferences")), tr("Preferences"), this, SLOT(showPreferences()));
	//			menu->addSeparator();
	//			m_menuFileQuit = menu->addAction(QIcon::fromTheme("application-exit", QIcon(":icons/menu/file/quit")), tr("Quit"), this, SLOT(close()));
	//			m_menuFileQuit->setMenuRole(QAction::QuitRole); /* OSX only */
	//			menuBar()->addMenu(menu);

	//			menu = new QMenu(tr("Presets"));
	//			m_ui->menuMyPresets = new QMenu(tr("My Presets"));
	//			menu->addMenu(m_ui->menuMyPresets);
	//			menu->addSeparator();
	//			menu->addAction(m_savePresetAction);
	//			menu->addAction(m_savePresetAsAction);
	//			menu->addAction(m_removePresetAction);
	//			menu->addSeparator();
	//			menu->addAction(QIcon::fromTheme("document-import", QIcon(":/icons/menu/presets/import")), tr("Import..."), this, SLOT(importPreset()));
	//			menu->addAction(QIcon::fromTheme("document-export", QIcon(":/icons/menu/presets/export")), tr("Export..."), this, SLOT(exportPreset()));
	//			menuBar()->addMenu(menu);

	//			menu = new QMenu(tr("Help"));
	//			QAction * action = menu->addAction(QIcon::fromTheme("help-about", QIcon(":/icons/menu/help/about")), tr("About"), this, SLOT(about()));
	//			action->setMenuRole(QAction::AboutRole); /* OSX only */
	//			action = menu->addAction(QIcon(":/icons/menu/help/aboutqt"), tr("About Qt"), qApp, SLOT(aboutQt()));
	//			action->setMenuRole(QAction::AboutQtRole); /* OSX only */
	//			menu->addAction(QIcon(":/icons/menu/help/aboutrsync"), tr("About rsync"), this, SLOT(aboutRsync()));
	//			menuBar()->addMenu(menu);

	//			QTabWidget * container = new QTabWidget();
	//			QVBoxLayout * vLayout = new QVBoxLayout();
	//			QGridLayout * gLayout = new QGridLayout();
	//			gLayout->addWidget(myLabel = new QLabel(tr("Source")), 0, 0);
	//			myLabel->setBuddy(m_source);
	//			gLayout->addWidget(m_source, 0, 1);
	//			gLayout->addWidget(m_chooseSourceDir, 0, 2);
	//			gLayout->addWidget(m_chooseSourceFile, 0, 3);

	//			gLayout->addWidget(myLabel = new QLabel(tr("Destination")), 1, 0);
	//			myLabel->setBuddy(m_dest);
	//			gLayout->addWidget(m_dest, 1, 1);
	//			gLayout->addWidget(m_chooseDestinationDir, 1, 2);
	//			gLayout->addWidget(m_chooseDestinationFile, 1, 3);

	//			vLayout->addLayout(gLayout);

	//			QGroupBox * group = new QGroupBox(tr("Preserve"));
	//			gLayout = new QGridLayout();
	//			gLayout->addWidget(m_preserveTime, 0, 0);
	//			gLayout->addWidget(m_preservePerms, 1, 0);
	//			gLayout->addWidget(m_preserveOwner, 0, 1);
	//			gLayout->addWidget(m_preserveGroup, 1, 1);

	//			group->setLayout(gLayout);
	//			vLayout->addWidget(group);

	//			group = new QGroupBox();
	//			gLayout = new QGridLayout();
	//			gLayout->addWidget(m_windowsCompatible, 0, 0);
	//			gLayout->addWidget(m_deleteOnServer, 0, 1);
	//			group->setLayout(gLayout);
	//			vLayout->addWidget(group);

	//			vLayout->addStretch(2);

	//			QWidget * page = new QWidget();
	//			page->setLayout(vLayout);

	//			container->addTab(page, QIcon::fromTheme("preferences-other", QIcon(":/icons/tabs/basic_settings")), tr("Basic Settings"));

	//			gLayout = new QGridLayout();
	//			gLayout->addWidget(m_alwaysChecksum, 0, 0);
	//			gLayout->addWidget(m_preserveDevices, 1, 0);
	//			gLayout->addWidget(m_onlyUpdateExisting, 2, 0);
	//			gLayout->addWidget(m_keepParitalTransfers, 3, 0);
	//			gLayout->addWidget(m_symlinksAsSymlinks, 4, 0);
	//			gLayout->addWidget(m_makeBackups, 5, 0);

	//			gLayout->addWidget(m_compressInTransit, 0, 1);
	//			gLayout->addWidget(m_dontUpdateExisting, 2, 1);
	//			gLayout->addWidget(m_dontMapUidGid, 3, 1);
	//			gLayout->addWidget(m_copyHardlinksAsHardlinks, 4, 1);
	//			gLayout->addWidget(m_showItemisedChanges, 5, 1);

	//			QHBoxLayout * hLayout = new QHBoxLayout();
	//			hLayout->addWidget(myLabel = new QLabel(tr("Log file")));
	//			myLabel->setBuddy(m_logFile);
	//			hLayout->addWidget(m_logFile);
	//			hLayout->addWidget(m_chooseLogFile);
	//			gLayout->addLayout(hLayout, 6, 0, 1, 2, Qt::AlignLeft);

	//			vLayout = new QVBoxLayout();
	//			vLayout->addLayout(gLayout);
	//			vLayout->addStretch(2);

	//			page = new QWidget();
	//			page->setLayout(vLayout);

	//			container->addTab(page, QIcon::fromTheme("preferences-system", QIcon(":/icons/tabs/advanced_settings")), tr("Advanced Settings"));

	//			setCentralWidget(container);
	//		}
	//	}


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


	void MainWindow::showPreset(int index) {
		/* NOTE index can be > last preset in manager because the combo box has
	   <New Preset> empty item as its last index */
		if(index >= 0 && index < qyncApp->presetCount()) {
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
			m_ui->onlyUpdateExisting->setChecked(preset->onlyUpdateExistingEntries());
			//			m_dontUpdateExisting->setChecked(preset->dontUpdateExistingEntries());
			m_ui->dontMapUidGid->setChecked(preset->dontMapUsersAndGroups());
			m_ui->hardlinksAsHardlinks->setChecked(preset->copyHardlinksAsHardlinks());
			m_ui->itemisedChanges->setChecked(preset->showItemisedChanges());

			m_ui->source->setText(preset->source());
			m_ui->destination->setText(preset->destination());

			m_ui->logFile->setText(preset->logFile());

			m_ui->actionRemove->setEnabled(true);
		}
		else {
			m_ui->actionRemove->setEnabled(false);
		}
	}


	void MainWindow::readPreferences(void) {
		const GuiPreferences * prefs = dynamic_cast<const GuiPreferences *>(qyncApp->preferences());

		if(prefs) {
			m_ui->presetsToolbar->setToolButtonStyle(prefs->toolBarButtonStyle());
			m_ui->synchroniseToolbar->setToolButtonStyle(prefs->toolBarButtonStyle());

			m_ui->presetsToolbar->setVisible(prefs->showPresetsToolBar());
			m_ui->synchroniseToolbar->setVisible(prefs->showSynchroniseToolBar());
		}
	}


	//	void MainWindow::syncIgnoreExistingToOnlyExisting(void) {
	//		if(m_ui->onlyUpdateExisting->isChecked()) {
	//			m_dontUpdateExisting->setChecked(false);
	//		}
	//	}


	//	void MainWindow::syncOnlyExistingToIgnoreExisting(void) {
	//		if(m_dontUpdateExisting->isChecked()) {
	//			m_ui->onlyUpdateExisting->setChecked(false);
	//		}
	//	}


	void MainWindow::disconnectApplication(void) {
		qyncApp->disconnect(this);
	}


	void MainWindow::connectApplication(void) {
		connect(qyncApp, &Application::presetsChanged, this, &MainWindow::refreshPresets);
		connect(qyncApp, &Application::preferencesChanged, this, &MainWindow::readPreferences);
	}


	void MainWindow::refreshPresets(void) {
		m_ui->presets->clear();
		m_ui->menuMyPresets->clear();

		int i = 0;

		for(Preset * preset : qyncApp->presets()) {
			m_ui->presets->addItem(preset->name());
			QAction * action = m_ui->menuMyPresets->addAction(preset->name());  //, this, SLOT(showPresetFromMenu()));
			connect(action, &QAction::triggered, this, &MainWindow::showPresetFromMenu);
			action->setData(i);
			i++;
		}

		m_ui->presets->addItem(tr("<New Preset>"));
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


	void MainWindow::saveSettingsToCurrentPreset(void) {
		int i = m_ui->presets->currentIndex();

		if(i >= m_ui->presets->count() - 1) {
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


	void MainWindow::newPresetFromSettings(void) {
		bool ok;
		QString name = QInputDialog::getText(this, tr("%1 Request").arg(qyncApp->applicationDisplayName()), tr("Enter a name for the new preset"), QLineEdit::Normal, tr("New Preset"), &ok);

		if(ok) {
			Preset * p = new Preset();
			fillPreset(*p);
			p->setName(name);
			disconnectApplication(); /* don't refresh on presetesChanged() signal from manager - we'll do it ourselves */

			if(qyncApp->addPreset(p)) {
				refreshPresets();
				m_ui->presets->setCurrentIndex(m_ui->presets->count() - 2);
			}
			else {
				delete p;
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The new preset could not be created:\n\n%1").arg(qyncApp->lastError()));
			}

			connectApplication();
		}
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
		p.setOnlyUpdateExistingEntries(m_ui->onlyUpdateExisting->isChecked());
		//		p.setDontUpdateExistingEntries(m_dontUpdateExisting->isChecked());
		p.setDontMapUsersAndGroups(m_ui->dontMapUidGid->isChecked());
		p.setCopyHardlinksAsHardlinks(m_ui->hardlinksAsHardlinks->isChecked());
		p.setShowItemisedChanges(m_ui->itemisedChanges->isChecked());

		p.setSource(m_ui->source->text());
		p.setDestination(m_ui->destination->text());

		p.setLogFile(m_ui->logFile->text());
	}


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


	void MainWindow::execute(void) {
		/* even though it's (currently) passed by raw ptr, the temp Process is not
		 * owned by the Process created by Application::execute() */
		Preset temp;
		fillPreset(temp);
		Process * process = qyncApp->execute(&temp);

		if(process) {
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
