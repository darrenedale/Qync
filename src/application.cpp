/**
 * @file application.cpp
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Implementation of the Application class.
 */

#include "application.h"

#include <QtCore/QtGlobal>
#include <QtCore/QStringBuilder>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QStandardPaths>

#include "mainwindow.h"
#include "preset.h"
#include "process.h"
#include "preferences.h"

static constexpr const char * const QyncAppName = "Qync";
static constexpr const char * const QyncAppVersionString = "1.1.1";
static constexpr const char * const QyncAppVersionDate = "April 2020";
static constexpr const char * const QyncAppBuildId = "ac4e7d5";
static constexpr const char * const QyncAppWebsite = "https://www.equituk.net/";
static constexpr const char * const QyncOrgName = "BitCraft";
static constexpr const char * const QyncOrgDomain = "bitcraft.eu";

using namespace Qync;

/**
 * @class Application
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Manages core application functionality for Qync.
 *
 * The Manager keeps a set of presets and preferences for the application and acts as a central point around which
 * the user interface can operate, whatever type of iterface that happens to be. It provides controlled access
 * to the core resources of the application - presets, preferences - and provides a means by which instances of the
 * rsync process can be spawned.
 *
 * The class aslo provides a number of signals that enable the user interface to be kept informed of important
 * events such as when presets are changed, when the preferences change and when a process is spawned.
 *
 * The application stores all of its configuration details in a hidden directory in the user's home directory. The
 * @b preferences file stores the application preferences as XML, and the presets folder stores each preset in its
 * own XML file.
 */

/**
 * @typedef Qync::Application::PresetList
 * @brief An alias for the container type for the list of presets.
 */

/**
 * @brief Create a new application.
 */
Application::Application(int & argc, char ** argv)
:   QApplication(argc, argv),
    m_rsyncVersionText(),
    m_configPath(),
    m_presetsPath(),
    m_presets(),
    m_prefs(),
    m_mainWindow(nullptr),
    m_lastError()
{
    setApplicationName(QyncAppName);
    setApplicationDisplayName(QyncAppName);
    setApplicationVersion(QyncAppVersionString);
    setOrganizationName(QyncOrgName);
    setOrganizationDomain(QyncOrgDomain);
    setProperty("ReleaseDate", QyncAppVersionDate);
    setProperty("BuildId", QyncAppBuildId);
    setProperty("ApplicationWebsite", QyncAppWebsite);

    // do this here rather than in initialisation section above because we need QStandardPaths to take account of
    // organisation name/domain and app name set in this constructor. in the init section these are not yet set
    m_configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    m_presetsPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) % "/presets";

    // ensure presets and config dirs exist
    auto root = QDir::root();
    root.mkpath(m_configPath);
    root.mkpath(m_presetsPath);

    m_prefs.loadFrom(m_configPath + "/guipreferences");
    loadPresets();

    // MainWindow constructor uses Application instance, specifically app display name, so
    // this must be instantiated after the Application instance is set up sufficiently
    m_mainWindow = std::make_unique<MainWindow>();
}

/**
 * @brief Destroy the application.
 */
Application::~Application() {
    clearPresets();
}

/**
 * @brief Provides the path to the configuration directory.
 *
 * The configuration directory is where application components store their
 * configuration files.
 *
 * @return The configuration directory path.
 */
QString Application::configurationPath() {
    return m_configPath;
}

/**
 * @brief Provides the path to the presets directory.
 *
 * The presets directory is the directory where synchronisation presets
 * are stored.
 *
 * @return The presets directory path.
 */
QString Application::presetsPath() {
    return m_presetsPath;
}

/**
 * @brief Set the text that describes the last error.
 *
 * @param err is the error description.
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
 * @brief Get an explanation of what caused the last failed operation.
 *
 * The description is suitable for presentation to the end user. It is
 * only valid immediately after an operation failed. Any subsequent
 * successful operations do not necessarily reset the last error so
 * you may not rely on the return value of this method being an empty or
 * null string as an indicator of success or failure. You must always
 * check the return values of method calls if you are interested in
 * their success or otherwise.
 *
 * @return An explanation of the cause for the last error.
 */
QString Application::lastError() const {
    return m_lastError;
}

/**
 * @brief Start the application's main loop.
 *
 * The main window is shown and the main loop is executed.
 *
 * @return 0 on successful execution, non-0 on error.
 */
int Application::exec() {
    Q_ASSERT_X(qyncApp, __PRETTY_FUNCTION__, "no Application instance");
    Q_ASSERT_X(qyncApp->m_mainWindow, __PRETTY_FUNCTION__, "no main window (perhaps Application::~Application has been called?)");

    qyncApp->m_mainWindow->show();
    return QApplication::exec();
}

/**
 * @brief Retrieve an indexed preset from the application.
 *
 * @param index is the index of the preset to retrieve.
 *
 * Indices are 0-based. If the index is found to be out of bounds,
 * an assertion failure occurs.
 *
 * @return the preset at the index provided.
 */
Preset & Application::preset(int index) {
    Q_ASSERT_X(0 <= index && m_presets.size() > static_cast<PresetList::size_type>(index), __PRETTY_FUNCTION__, QString("index %1 is out of bounds (have presets 0 .. %2)").arg(index, static_cast<int>(m_presets.size() - 1)).toUtf8());
    return *m_presets[static_cast<PresetList::size_type>(index)];
}

/**
 * @brief Remove an indexed preset from the collection stored in the
 * application.
 *
 * @param index is index of the preset to remove.
 *
 * Indices are 0-based. The provided index must not be out of bounds.
 *
 * The preset at the provided index is removed from the collection.
 *
 * If this method returns @b true, one or more pointers in a
 * previously retrieved set from presets() is invalid, and existing
 * indices may not be valid. For this reason, all presets retrieved
 * from the application prior to a successful call to this method must be
 * considered invalid.
 *
 * @return @b true if the preset was removed from the collection,
 * @b false otherwise.
 */
bool Application::removePreset(int index) {
    Q_ASSERT_X(0 <= index && m_presets.size() > static_cast<PresetList::size_type>(index), __PRETTY_FUNCTION__, QString("index %1 out of bounds (must be in range 0..%2)").arg(index, static_cast<int>(m_presets.size() - 1)).toUtf8());

    auto presetToRemove = m_presets.begin() + index;
    bool ret = true;
    QFileInfo f((*presetToRemove)->fileName());

    if(f.exists() && f.absoluteFilePath().startsWith(m_presetsPath + "/") && QFile::remove(f.absoluteFilePath())) {
        setLastError("The file for the preset could not be deleted from disk. It will reappear next time your presets are reloaded.");
        ret = false;
    }

    m_presets.erase(presetToRemove);
    Q_EMIT presetsChanged();
    return ret;
}

/**
 * @brief Add a preset to the collection stored in the application.
 *
 * @param name The name of the preset to add.
 *
 * The Application creates a preset with the given name and returns a
 * reference to it. The created preset is saved to disk with the other,
 * existing presets in the presets directory (see presetsPath()).
 *
 * @return A reference to the preset added.
 */
Preset & Application::addPreset(const QString & name) {
    m_presets.emplace_back(std::make_unique<Preset>(name));
    Preset & preset = *m_presets.back();
    QString fileName;
    QDir presetDir(m_presetsPath);
    int index = 0;

    do {
        fileName = "preset" + QString::number(++index);
    } while(presetDir.exists(fileName));

    preset.setFileName(presetDir.absoluteFilePath(fileName));

    preset.save();
    Q_EMIT presetsChanged();
    return preset;
}

/**
 * @brief Remove all presets stored in the application.
 *
 * All presets will be removed and the application will end up containing
 * no presets. All presets that were contained in the application before
 * the call will cease to exist, so any references to presets retrieved
 * using preset() or presets() will be invalid.
 */
void Application::clearPresets() {
    m_presets.clear();
}

/**
 * @brief Load the presets from the default location.
 *
 * The existing presets are discarded and the set stored in the default
 * location are loaded. The default location is usually in the user's
 * home directory. This differs according to platform.
 *
 * return @b true if the presets were loaded, @b false otherwise.
 */
bool Application::loadPresets() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) % "/presets/";
    QSignalBlocker blocker(this);
    QDir presetDir(path);

    if(!presetDir.exists()) {
        qWarning() << __PRETTY_FUNCTION__ << "path" << path << "does not exist";
        return false;
    }

    clearPresets();

    for(const auto & fileName : presetDir.entryList(QDir::Files | QDir::Readable)) {
        Preset preset;

        if(!loadPreset(presetDir.absoluteFilePath(fileName))) {
            qWarning() << __PRETTY_FUNCTION__ << "failed to load preset from file" << fileName;
        }
    }

    blocker.unblock();
    Q_EMIT presetsChanged();
    return true;
}

/**
 * @brief Load a preset from a file.
 *
 * @param fileName The path to the file to load.
 *
 * The preset is loaded from the file. If successful, it is added to the
 * application's list of presets, and saved to the presets directory on
 * disk.
 *
 * If it was loaded successfully, the loaded Preset is guaranteed to be
 * the last in the stored list of presets.
 *
 * @return @b true if the preset was loaded successfully, @b false if not.
 */
bool Application::loadPreset(const QString & fileName) {
    m_presets.emplace_back(std::make_unique<Preset>());
    Preset & preset = *m_presets.back();
    bool ret = preset.load(fileName);

    if(!ret) {
        m_presets.erase(m_presets.end());
    }
    else {
        if(!QFileInfo(fileName).absolutePath().startsWith(m_presetsPath)) {
            int i = 0;
            QString newFileName;

            do {
                ++i;
                newFileName = m_presetsPath % "/preset" % QString::number(i);
            } while(QFileInfo::exists(newFileName));

            if(!preset.saveAs(newFileName)) {
                qWarning() << "failed to save preset to" << newFileName;
            }
        }

        Q_EMIT presetsChanged();
    }

    return ret;
}

/**
 * @brief Get the version text of the rsync binary.
 *
 * The version text is the text output when the command @b rsync
 * @b --version is issued.
 *
 * @return The version text, or a null string if the rsync binary is
 * not valid.
 */
QString Application::rsyncVersionText() {
    if(m_rsyncVersionText.isEmpty()) {
        QProcess p(this);
        p.start(m_prefs.rsyncPath(), QStringList() << "--version", QIODevice::ReadOnly | QIODevice::Text);
        p.waitForFinished();
        m_rsyncVersionText = p.readAll();
    }

    return m_rsyncVersionText;
}

/**
 * @fn Application::presetCount()
 * @brief Get the number of presets stored in the application.
 *
 * @return The number of presets.
 */

/**
 * @fn Preferences & Application::preferences()
 * @brief Get the application preferences.
 *
 * @return the application preferences.
 */

/**
 * @fn Application::presets()
 * @brief Retrieve the presets stored in the application.
 *
 * @return the presets (or an empty set if the application does not have
 * any presets stored.
 */

/**
 * @fn Application::presetsChanged()
 * @brief Emitted when a preset has been removed.
 *
 * When emitted with either presetAdded() or presetRemoved(), this
 * signal is always emitted after the other, never before.
 */

/**
 * @fn Application::preferencesChanged()
 * @brief Emitted when the application preferences have changed.
 */
