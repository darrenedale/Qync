/**
 * \file application.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.5
 *
 * \brief Implementation of the Application class.
 *
 * \dep
 * - application.h
 * - preset.h
 * - process.h
 * - preferences.h
 * - QDebug
 * - QDir
 * - QFileInfo
 * - QProcess
 * - QStandardPaths
 */

#include "application.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

#include "preset.h"
#include "process.h"
#include "preferences.h"


#define QYNC_APP_NAME "Qync"
#define QYNC_APP_DISPLAY_NAME "Qync"
#define QYNC_APP_VERSION_STRING "0.9.5"
#define QYNC_APP_VERSION_DATE "September 2017"
#define QYNC_APP_BUILD_ID "<undefined>"
#define QYNC_APP_URL "http://www.equituk.net/"


namespace Qync {

	/**
	 * \class Application
	 * \author Darren Edale
	 * \date September 2017
	 * \version 0.9.5
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
	 * The manager stores all of its configuration details in a hidden directory
	 * in the user's home directory. The \b preferences file stores the application
	 * preferences as XML, and the presets folder stores each preset in its own
	 * XML file.
	 */

	QString Application::s_rsyncVersionText;
	QString Application::s_configPath;


	Application::Application(int & argc, char ** argv)
	  : QApplication(argc, argv),
		 m_prefs{} {
		setApplicationName(QYNC_APP_NAME);
		setApplicationDisplayName(QYNC_APP_DISPLAY_NAME);
		setApplicationVersion(QYNC_APP_VERSION_STRING);
		setOrganizationName("BitCraft");
		setOrganizationDomain("bitcraft.eu");
		setProperty("ReleaseDate", QYNC_APP_VERSION_DATE);
		setProperty("BuildId", QYNC_APP_BUILD_ID);
		setProperty("ApplicationWebsite", QYNC_APP_URL);

		// ensure presets dir exists
		QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).mkpath("presets");
		loadPresets();
	}


	Application::~Application(void) {
		clearPresets();
	}


	QString Application::configurationDirectoryPath(void) {
		return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	}


	void Application::setLastError(const QString & err) const {
		m_lastError = err;
	}


	QString Application::lastError(void) const {
		return m_lastError;
	}


	const Preferences * Application::preferences(void) const {
		return m_prefs.get();
	}


	bool Application::setPreferences(Preferences * prefs) {
		Application::s_rsyncVersionText = QString(); /* force re-read on next call to rsyncVersionText() as binary might have changed */
		m_prefs.reset(prefs);
		Q_EMIT preferencesChanged();
		return true;
	}


	int Application::presetCount(void) const {
		return m_presets.size();
	}


	QList<Preset *> Application::presets(void) const {
		return m_presets;
	}


	Preset * Application::preset(int index) const {
		if(index < 0 || index >= m_presets.size()) {
			qDebug() << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
			return 0;
		}

		return m_presets.at(index);
	}


	bool Application::removePreset(int index) {
		if(index < 0 || index >= m_presets.size()) {
			qDebug() << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
			setLastError(tr("The preset at position %1 in the list could not be found.").arg(index));
			return false;
		}

		return removePreset(m_presets.at(index));
	}


	bool Application::removePreset(Preset * preset) {
		if(m_presets.contains(preset)) {
			QStringList err;
			QFileInfo f(preset->fileName());

			if(f.exists() && f.absoluteFilePath().startsWith(Application::s_configPath + "presets/")) {
				if(!QFile::remove(f.absoluteFilePath()))
					err << tr("The file for the preset could not be deleted from disk. It will reappear next time your presets are reloaded.");
			}

			if(m_presets.removeAll(preset) > 1)
				err << tr("The preset was in the list more than once. Each instance of it in the list was removed.");
			delete preset;

			Q_EMIT presetRemoved();
			Q_EMIT presetsChanged();

			switch(err.size()) {
				case 1:
					setLastError(err.at(0));
					return false;

				case 0:
					break;

				default:
					setLastError(tr("The following problems were encoutered when removing the preset:\n\n%1").arg(err.join("\n")));
					return false;
			}

			return true;
		}

		setLastError(tr("The preset to remove is not valid."));
		return false;
	}


	bool Application::insertPreset(Preset * preset, int i) {
		if(preset && !m_presets.contains(preset)) {
			if(i < 0 || i > m_presets.size())
				i = m_presets.size();
			m_presets.insert(i, preset);

			QFileInfo f(preset->fileName());

			/* if the file is not in the presets dir, give it a new filename */
			if(!f.absoluteFilePath().startsWith(Application::s_configPath + "presets/")) {
				QString fileName;
				QDir presetDir(Application::s_configPath + "presets/");
				int index = 0;

				do {
					fileName = "preset" + QString::number(++index);
				} while(presetDir.exists(fileName));

				preset->setFileName(presetDir.absoluteFilePath(fileName));
			}

			preset->save();
			Q_EMIT presetAdded(preset, i);
			Q_EMIT presetsChanged();

			return true;
		}

		return false;
	}


	bool Application::addPreset(Preset * preset) {
		return insertPreset(preset, -1);
	}


	void Application::clearPresets(void) {
		for(Preset * preset : m_presets) {
			delete preset;
		}

		m_presets.clear();
	}


	bool Application::loadPresets(void) {
		return loadPresets(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/presets/");
	}


	bool Application::loadPresets(const QString & path) {
		QSignalBlocker blocker(this);
		QDir d(path);

		if(!d.exists()) {
			qDebug() << "path" << path << "does not exist";
			return false;
		}

		clearPresets();

		for(const auto & fileName : d.entryList(QDir::Files | QDir::Readable)) {
			Preset * myPreset = Preset::load(d.absoluteFilePath(fileName));

			if(myPreset) {
				addPreset(myPreset);
			}
			else {
				qDebug() << "failed to load preset from file" << fileName;
			}
		}

		blocker.unblock();
		Q_EMIT presetsChanged();
		return true;
	}


	Process * Application::simulate(int i) const {
		if(i < 0 || i > m_presets.size())
			return 0;
		return simulate(preset(i));
	}


	Process * Application::simulate(const Preset * preset) const {
		if(!preset || !m_prefs) {
			return 0;
		}

		//QStringList args = Process::rsyncArguments(preset, QStringList("--dry-run"));
		//qDebug() << "command: " << m_prefs->rsyncPath() << args;

		//if(!args.isEmpty()) {
		Process * p = new Process(m_prefs->rsyncPath(), preset);
		Q_EMIT processStarted(p);
		return p;
		//}

		setLastError(tr("The rsync program could not be started because it was not possible to determine the correct command parameters."));
		return 0;
	}


	Process * Application::execute(int i) const {
		if(i < 0 || i > m_presets.size())
			return 0;
		return execute(preset(i));
	}


	Process * Application::execute(const Preset * preset) const {
		if(!preset || !m_prefs)
			return 0;
		//QStringList args = Process::rsyncArguments(preset);
		//qDebug() << "command: " << m_prefs->rsyncPath() << args;

		//if(!args.isEmpty()) {
		Process * p = new Process(m_prefs->rsyncPath(), preset);
		Q_EMIT processStarted(p);
		return p;
		//}

		setLastError(tr("The rsync program could not be started because it was not possible to determine the correct command parameters."));
		return 0;
	}


	QString Application::rsyncVersionText(void) {
		if(m_prefs && Application::s_rsyncVersionText.isEmpty()) {
			QProcess p(this);
			//qDebug() << ("starting process: " + m_prefs->rsyncPath() + " --version");
			p.start(m_prefs->rsyncPath(), QStringList() << "--version", QIODevice::ReadOnly | QIODevice::Text);
			p.waitForFinished();
			Application::s_rsyncVersionText = p.readAll();
		}

		return Application::s_rsyncVersionText;
	}


}  // namespace Qync
