/**
 * \file Manager.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the Manager class.
 *
 * \dep
 * - Manager.h
 * - Preset.h
 * - Process.h
 * - Preferences.h
 * - QDebug
 * - QDir
 * - QFileInfo
 * - QProcess
 *
 * \todo
 * Nothing.
 */

#include "Manager.h"

#include "Preset.h"
#include "Process.h"
#include "Preferences.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>


namespace Qync {


	QString Manager::s_rsyncVersionText;
	QString Manager::s_configPath;


	Manager::Manager(void)
	  : m_prefs(0),
		 m_doSignals(true) {
		Manager::initialiseClass();
		loadPresets();
	}


	Manager::~Manager(void) {
		clearPresets();
		disposePrefs();
	}


	QString Manager::configurationDirectoryPath(void) {
		initialiseClass();
		return s_configPath;
	}

	void Manager::disposePrefs(void) {
		if(m_prefs)
			delete m_prefs;
		m_prefs = nullptr;
	}


	void Manager::initialiseClass(void) {
		static bool s_initialised = false;

		if(!s_initialised) {
#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
			Manager::s_configPath = QDir::homePath() + "/.qync/";
#elif defined(Q_OS_DARWIN)
			Manager::s_configPath = QDir::homePath() + "/.qync/";
#elif defined(Q_OS_WIN32)
			Manager::s_configPath = QDir::homePath() + "/.qync/";
#endif

			QDir presetPath(Manager::s_configPath + "presets/");
			if(!presetPath.exists())
				presetPath.mkpath(Manager::Manager::s_configPath + "presets/");
			s_initialised = true;
		}
	}


	void Manager::setLastError(const QString & err) const {
		m_lastError = err;
	}


	QString Manager::lastError(void) const {
		return m_lastError;
	}


	const Preferences * Manager::preferences(void) const {
		return m_prefs;
	}


	bool Manager::setPreferences(Preferences * prefs) {
		disposePrefs();
		Manager::s_rsyncVersionText = QString(); /* force re-read on next call to rsyncVersionText() as binary might have changed */
		m_prefs = prefs;
		emit(preferencesChanged());
		return true;
	}


	int Manager::presetCount(void) const {
		return m_presets.size();
	}


	QList<Preset *> Manager::presets(void) const {
		return m_presets;
	}


	Preset * Manager::preset(int index) const {
		if(index < 0 || index >= m_presets.size()) {
			qDebug() << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
			return 0;
		}

		return m_presets.at(index);
	}


	bool Manager::removePreset(int index) {
		if(index < 0 || index >= m_presets.size()) {
			qDebug() << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
			setLastError(tr("The preset at position %1 in the list could not be found.").arg(index));
			return false;
		}

		return removePreset(m_presets.at(index));
	}


	bool Manager::removePreset(Preset * preset) {
		if(m_presets.contains(preset)) {
			QStringList err;
			QFileInfo f(preset->fileName());

			if(f.exists() && f.absoluteFilePath().startsWith(Manager::s_configPath + "presets/")) {
				if(!QFile::remove(f.absoluteFilePath()))
					err << tr("The file for the preset could not be deleted from disk. It will reappear next time your presets are reloaded.");
			}

			if(m_presets.removeAll(preset) > 1)
				err << tr("The preset was in the list more than once. Each instance of it in the list was removed.");
			delete preset;

			if(m_doSignals) {
				emit(presetRemoved());
				emit(presetsChanged());
			}

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


	bool Manager::insertPreset(Preset * preset, int i) {
		if(preset && !m_presets.contains(preset)) {
			if(i < 0 || i > m_presets.size())
				i = m_presets.size();
			m_presets.insert(i, preset);

			QFileInfo f(preset->fileName());

			/* if the file is not in the presets dir, give it a new filename */
			if(!f.absoluteFilePath().startsWith(Manager::s_configPath + "presets/")) {
				QString fileName;
				QDir presetDir(Manager::s_configPath + "presets/");
				int index = 0;

				do {
					fileName = "preset" + QString::number(++index);
				} while(presetDir.exists(fileName));

				preset->setFileName(presetDir.absoluteFilePath(fileName));
			}

			preset->save();

			if(m_doSignals) {
				emit(presetAdded(preset, i));
				emit(presetsChanged());
			}

			return true;
		}

		return false;
	}


	bool Manager::addPreset(Preset * preset) {
		return insertPreset(preset, -1);
	}


	void Manager::clearPresets(void) {
		foreach(Preset * preset, m_presets)
			if(preset)
				delete preset;

		m_presets.clear();
	}


	bool Manager::loadPresets(void) {
		return loadPresets(Manager::s_configPath + "presets/");
	}


	bool Manager::loadPresets(const QString & path) {
		m_doSignals = false;
		QDir d(path);

		if(!d.exists()) {
			qDebug() << "path" << path << "does not exist";
			return false;
		}

		clearPresets();

		foreach(QString fileName, d.entryList(QDir::Files | QDir::Readable)) {
			Preset * myPreset = Preset::load(d.absoluteFilePath(fileName));

			if(myPreset)
				addPreset(myPreset);
			else
				qDebug() << "failed to load preset from file" << fileName;
		}

		m_doSignals = true;
		emit(presetsChanged());
		return true;
	}


	Process * Manager::simulate(int i) const {
		if(i < 0 || i > m_presets.size())
			return 0;
		return simulate(preset(i));
	}


	Process * Manager::simulate(const Preset * preset) const {
		if(!preset || !m_prefs)
			return 0;
		//QStringList args = Process::rsyncArguments(preset, QStringList("--dry-run"));
		//qDebug() << "command: " << m_prefs->rsyncPath() << args;

		//if(!args.isEmpty()) {
		Process * p = new Process(m_prefs->rsyncPath(), preset);
		emit(processStarted(p));
		return p;
		//}

		setLastError(tr("The rsync program could not be started because it was not possible to determine the correct command parameters."));
		return 0;
	}


	Process * Manager::execute(int i) const {
		if(i < 0 || i > m_presets.size())
			return 0;
		return execute(preset(i));
	}


	Process * Manager::execute(const Preset * preset) const {
		if(!preset || !m_prefs)
			return 0;
		//QStringList args = Process::rsyncArguments(preset);
		//qDebug() << "command: " << m_prefs->rsyncPath() << args;

		//if(!args.isEmpty()) {
		Process * p = new Process(m_prefs->rsyncPath(), preset);
		emit(processStarted(p));
		return p;
		//}

		setLastError(tr("The rsync program could not be started because it was not possible to determine the correct command parameters."));
		return 0;
	}


	QString Manager::rsyncVersionText(void) {
		if(m_prefs && Manager::s_rsyncVersionText.isEmpty()) {
			QProcess p(this);
			//qDebug() << ("starting process: " + m_prefs->rsyncPath() + " --version");
			p.start(m_prefs->rsyncPath(), QStringList() << "--version", QIODevice::ReadOnly | QIODevice::Text);
			p.waitForFinished();
			Manager::s_rsyncVersionText = p.readAll();
		}

		return Manager::s_rsyncVersionText;
	}


}  // namespace Qync
