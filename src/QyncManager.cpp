/**
 * \file QyncManager.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the QyncManager class.
 *
 * \dep
 * - QyncManager.h
 * - QyncPreset.h
 * - QyncProcess.h
 * - QyncPreferences.h
 * - QDebug
 * - QDir
 * - QFileInfo
 * - QProcess
 *
 * \todo
 * Nothing.
 */

#include "QyncManager.h"

#include "QyncPreset.h"
#include "QyncProcess.h"
#include "QyncPreferences.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>


QString QyncManager::s_rsyncVersionText;
QString QyncManager::s_configPath;


QyncManager::QyncManager( void )
:	m_prefs(0),
	m_doSignals(true) {
	QyncManager::initialiseClass();
	loadPresets();
}


QyncManager::~QyncManager( void ) {
	clearPresets();
	disposePrefs();
}


QString QyncManager::configurationDirectoryPath( void ) {
	initialiseClass();
	return s_configPath;
}

void QyncManager::disposePrefs( void ) {
	if(m_prefs) delete m_prefs;
	m_prefs = 0;
}


void QyncManager::initialiseClass( void ) {
	static bool s_initialised = false;

	if(!s_initialised) {
#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
		QyncManager::s_configPath = QDir::homePath() + "/.qync/";
#elif defined(Q_OS_DARWIN)
		QyncManager::s_configPath = QDir::homePath() + "/.qync/";
#elif defined(Q_OS_WIN32)
        QyncManager::s_configPath = QDir::homePath() + "/.qync/";
#endif

		QDir presetPath(QyncManager::s_configPath + "presets/");
		if(!presetPath.exists()) presetPath.mkpath(QyncManager::QyncManager::s_configPath + "presets/");
		s_initialised = true;
	}
}


void QyncManager::setLastError( const QString & err ) const {
	m_lastError = err;
}


QString QyncManager::lastError( void ) const {
	return m_lastError;
}


const QyncPreferences * QyncManager::preferences( void ) const {
	return m_prefs;
}


bool QyncManager::setPreferences( QyncPreferences * prefs ) {
	disposePrefs();
	QyncManager::s_rsyncVersionText = QString();	/* force re-read on next call to rsyncVersionText() as binary might have changed */
	m_prefs = prefs;
	emit(preferencesChanged());
	return true;
}


int QyncManager::presetCount( void ) const {
	return m_presets.size();
}


QList<QyncPreset *> QyncManager::presets( void ) const {
	return m_presets;
}


QyncPreset * QyncManager::preset( int index ) const {
	if(index < 0 || index >= m_presets.size()) {
		qDebug() << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
		return 0;
	}

	return m_presets.at(index);
}


bool QyncManager::removePreset( int index ) {
	if(index < 0 || index >= m_presets.size()) {
		qDebug() << "index out of bounds: " << index << " < 0 || " << index << " >= " << m_presets.size();
		setLastError(tr("The preset at position %1 in the list could not be found.").arg(index));
		return false;
	}

	return removePreset(m_presets.at(index));
}


bool QyncManager::removePreset( QyncPreset * preset ) {
	if(m_presets.contains(preset)) {
		QStringList err;
		QFileInfo f(preset->fileName());

		if(f.exists() && f.absoluteFilePath().startsWith(QyncManager::s_configPath + "presets/")) {
			if(!QFile::remove(f.absoluteFilePath())) err << tr("The file for the preset could not be deleted from disk. It will reappear next time your presets are reloaded.");
		}

		if(m_presets.removeAll(preset) > 1) err << tr("The preset was in the list more than once. Each instance of it in the list was removed.");
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


bool QyncManager::insertPreset( QyncPreset * preset, int i ) {
	if(preset && !m_presets.contains(preset)) {
		if(i < 0 || i > m_presets.size()) i = m_presets.size();
		m_presets.insert(i, preset);

		QFileInfo f(preset->fileName());

		/* if the file is not in the presets dir, give it a new filename */
		if(!f.absoluteFilePath().startsWith(QyncManager::s_configPath + "presets/")) {
			QString fileName;
			QDir presetDir(QyncManager::s_configPath + "presets/");
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


bool QyncManager::addPreset( QyncPreset * preset ) {
	return insertPreset(preset, -1);
}


void QyncManager::clearPresets( void ) {
	foreach(QyncPreset * preset, m_presets)
		if(preset) delete preset;

	m_presets.clear();
}


bool QyncManager::loadPresets( void ) {
	return loadPresets(QyncManager::s_configPath + "presets/");
}


bool QyncManager::loadPresets( const QString & path ) {
	m_doSignals = false;
	QDir d(path);

	if(!d.exists()) {
        qDebug() << "path" << path << "does not exist";
		return false;
	}

	clearPresets();

	foreach(QString fileName, d.entryList(QDir::Files | QDir::Readable)) {
		QyncPreset * myPreset = QyncPreset::load(d.absoluteFilePath(fileName));

		if(myPreset)
			addPreset(myPreset);
		else
            qDebug() << "failed to load preset from file" << fileName;
	}

	m_doSignals = true;
	emit(presetsChanged());
	return true;
}


QyncProcess * QyncManager::simulate( int i ) const {
	if(i < 0 || i > m_presets.size()) return 0;
	return simulate(preset(i));
}


QyncProcess * QyncManager::simulate( const QyncPreset * preset ) const {
	if(!preset || !m_prefs) return 0;
	//QStringList args = QyncProcess::rsyncArguments(preset, QStringList("--dry-run"));
	//qDebug() << "command: " << m_prefs->rsyncPath() << args;

	//if(!args.isEmpty()) {
		QyncProcess * p = new QyncProcess(m_prefs->rsyncPath(), preset);
		emit(processStarted(p));
		return p;
	//}

	setLastError(tr("The rsync program could not be started because it was not possible to determine the correct command parameters."));
	return 0;
}


QyncProcess * QyncManager::execute( int i ) const {
	if(i < 0 || i > m_presets.size()) return 0;
	return execute(preset(i));
}


QyncProcess * QyncManager::execute( const QyncPreset * preset ) const {
	if(!preset || !m_prefs) return 0;
	//QStringList args = QyncProcess::rsyncArguments(preset);
	//qDebug() << "command: " << m_prefs->rsyncPath() << args;

	//if(!args.isEmpty()) {
		QyncProcess * p = new QyncProcess(m_prefs->rsyncPath(), preset);
		emit(processStarted(p));
		return p;
	//}

	setLastError(tr("The rsync program could not be started because it was not possible to determine the correct command parameters."));
	return 0;
}


QString QyncManager::rsyncVersionText( void ) {
	if(m_prefs && QyncManager::s_rsyncVersionText.isEmpty()) {
		QProcess p(this);
        //qDebug() << ("starting process: " + m_prefs->rsyncPath() + " --version");
		p.start(m_prefs->rsyncPath(), QStringList() << "--version", QIODevice::ReadOnly | QIODevice::Text);
		p.waitForFinished();
		QyncManager::s_rsyncVersionText = p.readAll();
	}

	return QyncManager::s_rsyncVersionText;
}
