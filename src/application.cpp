/**
 * \file application.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the Application class.
 *
 * \dep
 * - application.h
 * - QDebug
 * - QDir
 * - QFileInfo
 * - QProcess
 * - QStandardPaths
 * - preset.h
 * - process.h
 * - preferences.h
 */

#include "application.h"

#include <QtGlobal>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

#include "aliases.h"
#include "mainwindow.h"
#include "preset.h"
#include "process.h"
#include "preferences.h"


#define QYNC_APP_NAME "Qync"
#define QYNC_APP_DISPLAY_NAME "Qync"
#define QYNC_APP_VERSION_STRING "1.0.1"
#define QYNC_APP_VERSION_DATE "September 2017"
#define QYNC_APP_BUILD_ID "493061f"
#define QYNC_APP_WEBSITE "http://www.equituk.net/"
#define QYNC_ORG_NAME "BitCraft"
#define QYNC_ORG_DOMAIN "bitcraft.eu"


namespace Qync {


	/**
	 * \class Application
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
	 *
	 * \brief Manages core application functionality for Qync.
	 *
	 * The Manager keeps a set of presets and preferences for the application
	 * and acts as a central point around which the user interface can operate,
	 * whatever type of iterface that happens to be. It provides controlled access
	 * to the core resources of the application - presets, preferences - and
	 * provides a means by which instances of the rsync process can be spawned.
	 *
	 * The class aslo provides a number of signals that enable the user interface
	 * to be kept informed of important events such as when presets are changed,
	 * when the preferences change and when a process is spawned.
	 *
	 * The application stores all of its configuration details in a hidden directory
	 * in the user's home directory. The \b preferences file stores the application
	 * preferences as XML, and the presets folder stores each preset in its own
	 * XML file.
	 */


	/**
	 * \brief Create a new application.
	 */
	Application::Application(int & argc, char ** argv)
	: QApplication(argc, argv),
	  m_rsyncVersionText(),
	  m_configPath(),
	  m_presetsPath(),
	  m_presets(),
	  m_prefs(),
	  m_lastError() {
		setApplicationName(QYNC_APP_NAME);
		setApplicationDisplayName(QYNC_APP_DISPLAY_NAME);
		setApplicationVersion(QYNC_APP_VERSION_STRING);
		setOrganizationName(QYNC_ORG_NAME);
		setOrganizationDomain(QYNC_ORG_DOMAIN);
		setProperty("ReleaseDate", QYNC_APP_VERSION_DATE);
		setProperty("BuildId", QYNC_APP_BUILD_ID);
		setProperty("ApplicationWebsite", QYNC_APP_WEBSITE);

		/* do this here rather than in initialisation section above because we need
		 * QStandardPaths to take account of organisation name/domain and app name set
		 * in this constructor. in the init section these are not yet set */
		m_configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
		m_presetsPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/presets";

		// ensure presets and config dirs exist
		auto root = QDir::root();
		root.mkpath(m_configPath);
		root.mkpath(m_presetsPath);

		m_prefs.loadFrom(m_configPath + "/guipreferences");
		loadPresets();
	}


	/**
	 * \brief Destroy the application.
	 */
	Application::~Application(void) {
		clearPresets();
	}


	/**
	 * \brief Provides the path to the configuration directory.
	 *
	 * The configuration directory is where application components store their
	 * configuration files.
	 *
	 * \return The configuration directory path.
	 */
	QString Application::configurationPath(void) {
		return m_configPath;
	}


	/**
	 * \brief Provides the path to the presets directory.
	 *
	 * The presets directory is the directory where synchronisation presets
	 * are stored.
	 *
	 * \return The presets directory path.
	 */
	QString Application::presetsPath(void) {
		return m_presetsPath;
	}


	/**
	 * \brief Set the text that describes the last error.
	 *
	 * \param err is the error description.
	 *
	 * Methods that indicate success or failure must use this method if
	 * they indicate failure so that the end user can be presented with a
	 * description of what went wrong. They need not reset it if they
	 * succeed as long as the clearly indicate success.
	 */
	void Application::setLastError(const QString & err) const {
		m_lastError = err;
	}


	/**
	 * \brief Get an explanation of what caused the last failed operation.
	 *
	 * The description is suitable for presentation to the end user. It is
	 * only valid immediately after an operation failed. Any subsequent
	 * successful operations do not necessarily reset the last error so
	 * you may not rely on the return value of this method being an empty or
	 * null string as an indicator of success or failure. You must always
	 * check the return values of method calls if you are interested in
	 * their success or otherwise.
	 *
	 * \return An explanation of the cause for the last error.
	 */
	QString Application::lastError(void) const {
		return m_lastError;
	}


	/**
	 * \brief Start the application's main loop.
	 *
	 * The main window is shown and the main loop is executed.
	 *
	 * \return 0 on successful execution, non-0 on error.
	 */
	int Application::exec(void) {
		MainWindow win;
		win.show();
		return QApplication::exec();
	}


	/**
	 * \brief Retrieve an indexed preset from the application.
	 *
	 * \param index is the index of the preset to retrieve.
	 *
	 * Indices are 0-based. If the index is found to be out of bounds,
	 * an assertion failure occurs.
	 *
	 * \return the preset at the index provided.
	 */
	Preset & Application::preset(int index) const {
		Q_ASSERT_X(0 <= index && m_presets.size() > static_cast<PresetList::size_type>(index), __PRETTY_FUNCTION__, QString("index %1 is out of bounds (have presets 0 .. %2)").arg(index).arg(m_presets.size() - 1).toUtf8().data());
		return *m_presets[static_cast<PresetList::size_type>(index)];
	}


	/**
	 * \brief Remove an indexed preset from the collection stored in the
	 * application.
	 *
	 * \param index is index of the preset to remove.
	 *
	 * Indices are 0-based. If the index is found to be out of bounds,
	 * no action will be taken.
	 *
	 * If the index is within bounds the preset at that index is removed
	 * from the collection and deleted.
	 *
	 * If this method returns \b true, one or more pointers in a
	 * previously retrieved set from presets() is invalid, and existing
	 * indices may not be valid. For this reason, all presets retrieved
	 * from the application prior to a successful call to this method must be
	 * considered invalid.
	 *
	 * \return \b true if the preset was removed from the collection,
	 * \b false otherwise.
	 */
	bool Application::removePreset(int index) {
		if(0 > index || m_presets.size() <= static_cast<PresetList::size_type>(index)) {
			qCritical() << __PRETTY_FUNCTION__ << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
			setLastError(tr("The preset at position %1 in the list could not be found.").arg(index));
			return false;
		}

		auto presetToRemove = m_presets.begin() + index;
		bool ret = true;
		QFileInfo f((*presetToRemove)->fileName());

		if(f.exists() && f.absoluteFilePath().startsWith(m_presetsPath + "/")) {
			if(!QFile::remove(f.absoluteFilePath())) {
				setLastError("The file for the preset could not be deleted from disk. It will reappear next time your presets are reloaded.");
				ret = false;
			}
		}

		m_presets.erase(presetToRemove);

		Q_EMIT presetRemoved();
		Q_EMIT presetsChanged();

		return ret;
	}


	/**
	 * \brief Add a preset to the collection stored in the application.
	 *
	 * \param preset is the preset to add.
	 *
	 * The Application object takes ownership of the Preset.
	 *
	 * \return \b \c true if the preset was added, \b \c false otherwise.
	 */
	bool Application::addPreset(Preset * preset) {
		Q_ASSERT_X(preset, __PRETTY_FUNCTION__, "null Preset pointer provided");

		m_presets.push_back(std::unique_ptr<Preset>(preset));
		QFileInfo f(preset->fileName());

		/* if the file is not in the presets dir, give it a new filename */
		if(!f.absoluteFilePath().startsWith(m_presetsPath + "/")) {
			QString fileName;
			QDir presetDir(m_presetsPath);
			int index = 0;

			do {
				fileName = "preset" + QString::number(++index);
			} while(presetDir.exists(fileName));

			preset->setFileName(presetDir.absoluteFilePath(fileName));
		}

		preset->save();
		Q_EMIT presetsChanged();

		return true;
	}


	/**
	 * \brief Remove all presets stored in the application.
	 *
	 * All presets will be removed and the application will end up containing
	 * no presets. All presets that were contained in the application before
	 * the call will be deleted, so any pointers to presets retrieved using
	 * preset() or presets() will be invalid.
	 */
	void Application::clearPresets(void) {
		m_presets.clear();
	}


	/**
	 * \brief Load the presets from the default location.
	 *
	 * The existing presets are discarded and the set stored in the default
	 * location are loaded. The default location is usually in the user's
	 * home directory. This differs according to platform.
	 *
	 * return \b true if the presets were loaded, \b false otherwise.
	 */
	bool Application::loadPresets(void) {
		return loadPresets(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/presets/");
	}


	/**
	 * \brief Load the presets from a specified location.
	 *
	 * \param path is the path to the directory from which to load the
	 * presets.
	 *
	 * The existing presets are discarded and the set stored in the
	 * specified location are loaded.
	 *
	 * return \b true if the presets were loaded, \b false otherwise.
	 */
	bool Application::loadPresets(const QString & path) {
		QSignalBlocker blocker(this);
		QDir d(path);

		if(!d.exists()) {
			qWarning() << __PRETTY_FUNCTION__ << "path" << path << "does not exist";
			return false;
		}

		clearPresets();

		for(const auto & fileName : d.entryList(QDir::Files | QDir::Readable)) {
			Preset * preset = Preset::load(d.absoluteFilePath(fileName));

			if(preset) {
				addPreset(preset);
			}
			else {
				qWarning() << __PRETTY_FUNCTION__ << "failed to load preset from file" << fileName;
			}
		}

		blocker.unblock();
		Q_EMIT presetsChanged();
		return true;
	}


	/**
	 * \brief Run a simulation of a preset.
	 *
	 * \param preset is the preset to simulate.
	 *
	 * The preset need not be one stored in the application. It is used to
	 * create a QyncProcess object that will simulate the preset. A
	 * simulation is a dry-run of \b rsync that performs all operations set
	 * up in the preset without actually modifying anything on disk or on a
	 * remote server.
	 *
	 * The process created is released by the application. The calling code is
	 * responsible for its management, including deleting it when it is no
	 * longer required. Similarly, the preset passed in is not consumed by
	 * the application and it is up to its creator to ensure it is deleted
	 * at the appropriate time.
	 *
	 * \return A pointer to a process to simulate the preset, or a \b null
	 * pointer if the preset cannot be simulated.
	 */
	std::shared_ptr<Process> Application::simulate(const Preset & preset) const {
		return std::make_shared<Process>(preset);
	}


	/**
	 * \brief Synchronise a preset.
	 *
	 * \param preset is the preset to synchronise.
	 *
	 * The preset need not be one stored in the application. It is used to
	 * create a Process object that will synchronise the preset.
	 *
	 * The process created is released by the application. The calling code is
	 * responsible for its management, including deleting it when it is no
	 * longer required. Similarly, the preset passed in is not consumed by
	 * the application and it is up to its creator to ensure it is deleted
	 * at the appropriate time.
	 *
	 * \return A pointer to a process to synchronise the preset, or a \b null
	 * pointer if the preset cannot be synchronised.
	 */
	std::shared_ptr<Process> Application::synchronise(const Preset & preset) const {
		return std::make_shared<Process>(preset);
	}


	/**
	 * \brief Get the version text of the rsync binary.
	 *
	 * The version text is the text output when the command \b rsync
	 * \b --version is issued.
	 *
	 * \return The version text, or a null string if the rsync binary is
	 * not valid.
	 */
	QString Application::rsyncVersionText(void) {
		if(m_rsyncVersionText.isEmpty()) {
			QProcess p(this);
			p.start(m_prefs.rsyncPath(), QStringList() << "--version", QIODevice::ReadOnly | QIODevice::Text);
			p.waitForFinished();
			m_rsyncVersionText = p.readAll();
		}

		return m_rsyncVersionText;
	}


	/**
	 * \fn Preferences & Application::preferences(void)
	 * \brief Get the application preferences.
	 *
	 * \return the application preferences.
	 */


	/**
	 * \fn Application::presetCount(void)
	 * \brief Get the number of presets stored in the application.
	 *
	 * \return The number of presets.
	 */


	/**
	 * \fn Application::presets(void)
	 * \brief Retrieve the presets stored in the application.
	 *
	 * The pointers in the set returned are still owned by the application.
	 * Your code may not delete any of them.
	 *
	 * \return the presets (or an empty set if the application does not have
	 * any presets stored.
	 */


	/**
	 * \fn Application::addPreset(Preset *)
	 * \brief Add a preset to the end of the collection stored in the
	 * application.
	 *
	 * \param preset is the preset to add.
	 *
	 * The application takes ownership of the preset and will delete it at
	 * the appropriate time.
	 *
	 * \return \b \c true if the preset was added, \b \c false otherwise.
	 */


	/**
	 * \fn Application::presetAdded(Preset *, int)
	 * \brief Emitted when a preset has been added.
	 *
	 * \param preset is a pointer to the preset that was added.
	 *
	 * When emitted, this signal is always emitted before presetsChanged(),
	 * never after.
	 */


	/**
	 * \fn Application::presetRemoved(void)
	 * \brief Emitted when a preset has been removed.
	 *
	 * When emitted, this signal is always emitted before presetsChanged(),
	 * never after.
	 */


	/**
	 * \fn Application::presetsChanged(void)
	 * \brief Emitted when a preset has been removed.
	 *
	 * When emitted with either presetAdded() or presetRemoved(), this
	 * signal is always emitted after the other, never before.
	 */


	/**
	 * \fn Application::preferencesChanged(void)
	 * \brief Emitted when the application preferences have changed.
	 */


	/**
	 * \fn Application::processStarted(Process *) const
	 * \brief Emitted when a process has been started.
	 *
	 * \param process is the process created.
	 *
	 * Extreme care needs to be taken when connecting to this signal. It is
	 * emitted whenever a call to simulate() or execute() produces a valid
	 * process. The process provided with the signal is owned elsewhere. The
	 * application does not guarantee its lifespan and the class(es) connecting
	 * to the signal do not own it. The process could therefore be deleted
	 * at any time without notice. Connecting classes may listen for signals
	 * from the process and connect to its slots, but may not call any
	 * method or otherwise manipulate the process.
	 *
	 * This means that the process may be invalid by the time your class
	 * receives this signal, that the process may never get started and that
	 * the process may never emit any signals at all.
	 */


}  // namespace Qync
