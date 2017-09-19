/**
 * \file Manager.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the Manager class.
 *
 * \dep
 * - QObject
 * - QList
 * - QString
 * - QStringList
 * - QProcess
 */

#ifndef QYNC_APPLICATION_H
#define QYNC_APPLICATION_H

#include <memory>

#include <QApplication>
#include <QList>
#include <QString>
#include <QStringList>
#include <QProcess>

namespace Qync {

	class Preset;
	class Process;
	class Preferences;

	class Application
	  : public QApplication {
		Q_OBJECT

	public:
		/**
		 * \brief Create a new manager.
		 */
		Application(int & argc, char ** argv);

		/**
		 * \brief Destroy the manager.
		 */
		~Application(void);

		/**
		 * \brief Provides the path to the configuration directory.
		 *
		 * The configuration directory is the directory where components in
		 * the application can store their configuration files.
		 *
		 * \return The configuration directory path.
		 */
		static QString configurationDirectoryPath(void);

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
		QString lastError(void) const;

		/**
		 * \brief Get the version text of the rsync binary.
		 *
		 * The version text is the text output when the command \b rsync
		 * \b --version is issued.
		 *
		 * \return The version text, or a null string if the rsync binary is
		 * not valid.
		 */
		QString rsyncVersionText(void);

		/**
		 * \brief Get the application preferences.
		 *
		 * \return the application preferences.
		 */
		const Preferences * preferences(void) const;

		/**
		 * \brief Set the application preferences.
		 *
		 * \param prefs is the new set of application preferences.
		 *
		 * If the preferences are successfully set, the preferencesChanged()
		 * signal is emitted. On success, the preferences object passed to this
		 * is consumed. On failure, it is not.
		 *
		 * \return \b true if the preferences were set, \b false otherwise.
		 */
		bool setPreferences(Preferences * prefs);

		/**
		 * \brief Get the number of presets stored in the manager.
		 *
		 * \return The number of presets.
		 */
		int presetCount(void) const;

		/**
		  * \brief Retrieve the presets stored in the manager.
		  *
		  * The pointers in the set returned are still owned by the manager.
		  * Your code may not delete any of them.
		  *
		  * \return the presets (or an empty set if the manager does not have
		  * any presets stored.
		  */
		QList<Preset *> presets(void) const;

		/**
		  * \brief Retrieve an indexed preset from the manager.
		  *
		  * \param index is the index of the preset to retrieve.
		  *
		  * Indices are 0-based. If the index is found to be out of bounds,
		  * a null pointer will be returned.
		  *
		  * The pointer returned is still owned by the manager. Your code
		  * may not delete it.
		  *
		  * \return the preset at the index provided, or \b null if the
		  * index is out of bounds.
		  */
		Preset * preset(int index) const;

		/**
		  * \brief Remove an indexed preset from the collection stored in the
		  * manager.
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
		  * from the manager prior to a successful call to this method must be
		  * considered invalid.
		  *
		  * \return \b true if the preset was removed from the collection,
		  * \b false otherwise.
		  */
		bool removePreset(int index);

		/**
		  * \brief Remove a known preset from the collection stored in the
		  * manager.
		  *
		  * \param preset is the preset to remove.
		  *
		  * If the preset is in the collection, it is removed and deleted. If
		  * not, it is untouched. If this method returns \b true the pointer
		  * passed in is no longer valid.
		  *
		  * If this method returns \b true, the pointer passed in is no longer
		  * valid, along with one or more pointers in any previously retrieved
		  * set from presets(), and existing indices may not be valid. For this
		  * reason, all presets retrieved from the manager prior to a successful
		  * call to this method must be considered invalid.
		  *
		  * \return \b true if the preset was removed from the collection,
		  * \b false otherwise.
		  */
		bool removePreset(Preset * preset);

		/**
		  * \brief Insert a preset into the collection stored in the manager.
		  *
		  * \param preset is the preset to add.
		  * \param index is the index at which to add it.
		  *
		  * Indices are 0-based. If an index is found to be out of bounds,
		  * the preset will be added to the end of the collection.
		  *
		  * The manager takes ownership of the preset and will delete it at
		  * the appropriate time.
		  *
		  * \return \b \c true if the preset was added, \b \c false otherwise.
		  */
		bool insertPreset(Preset * preset, int index = -1);

		/**
		  * \brief Add a preset to the end of the collection stored in the
		  * manager.
		  *
		  * \param preset is the preset to add.
		  *
		  * The manager takes ownership of the preset and will delete it at
		  * the appropriate time.
		  *
		  * \return \b \c true if the preset was added, \b \c false otherwise.
		  */
		bool addPreset(Preset * preset);

		/**
		  * \brief Remove all presets stored in the manager.
		  *
		  * All presets will be removed and the manager will end up containing
		  * no presets. All presets that were contained in the manager before
		  * the call will be deleted, so any pointers to presets retrieved using
		  * preset() or presets() will be invalid.
		  */
		void clearPresets(void);

		/**
		 * \brief Run a simulation of a preset.
		 *
		 * \param i is the index of the preset to simulate.
		 *
		 * The preset identified by its index is used to create a QyncProcess
		 * object that will simulate the preset. A simulation is a dry-run of
		 * rsync that performs all operations set up in the preset without
		 * actually modifying anything on disk or on a remote server.
		 *
		 * The process created is released by the manager. The calling code is
		 * responsible for its management, including deleting it when it is no
		 * longer required.
		 *
		 * If the index is out of bounds, \b null will be returned.
		 *
		 * \return A pointer to a process to simulate the preset, or a \b null
		 * pointer if the preset cannot be simulated.
		 */
		Process * simulate(int i) const;

		/**
		 * \brief Run a simulation of a preset.
		 *
		 * \param preset is the preset to simulate.
		 *
		 * The preset need not be one stored in the manager. It is used to
		 * create a QyncProcess object that will simulate the preset. A
		 * simulation is a dry-run of \b rsync that performs all operations set
		 * up in the preset without actually modifying anything on disk or on a
		 * remote server.
		 *
		 * The process created is released by the manager. The calling code is
		 * responsible for its management, including deleting it when it is no
		 * longer required. Similarly, the preset passed in is not consumed by
		 * the manager and it is up to its creator to ensure it is deleted
		 * at the appropriate time.
		 *
		 * \return A pointer to a process to simulate the preset, or a \b null
		 * pointer if the preset cannot be simulated.
		 */
		Process * simulate(const Preset * preset) const;

		/**
		 * \brief Execute of a preset.
		 *
		 * \param i is the index of the preset to execute.
		 *
		 * The preset identified by its index is used to create a QyncProcess
		 * object that will execute the preset.
		 *
		 * The process created is released by the manager. The calling code is
		 * responsible for its management, including deleting it when it is no
		 * longer required.
		 *
		 * If the index is out of bounds, \b null will be returned.
		 *
		 * \return A pointer to a process to execute the preset, or a \b null
		 * pointer if the preset cannot be executed.
		 */
		Process * execute(int i) const;

		/**
		 * \brief Execute a preset.
		 *
		 * \param preset is the preset to execute.
		 *
		 * The preset need not be one stored in the manager. It is used to
		 * create a QyncProcess object that will execute the preset.
		 *
		 * The process created is released by the manager. The calling code is
		 * responsible for its management, including deleting it when it is no
		 * longer required. Similarly, the preset passed in is not consumed by
		 * the manager and it is up to its creator to ensure it is deleted
		 * at the appropriate time.
		 *
		 * \return A pointer to a process to execute the preset, or a \b null
		 * pointer if the preset cannot be executed.
		 */
		Process * execute(const Preset * preset) const;

	public Q_SLOTS:
		/**
		 * \brief Load the presets from the default location.
		 *
		 * The existing presets are discarded and the set stored in the default
		 * location are loaded. The default location is usually in the user's
		 * home directory. This differs according to platform.
		 *
		 * return \b true if the presets were loaded, \b false otherwise.
		 */
		bool loadPresets(void);

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
		bool loadPresets(const QString & path);

	Q_SIGNALS:
		/**
		 * \brief Emitted when a preset has been added.
		 *
		 * \param preset is a pointer to the preset that was added.
		 * \param index is its index in the list of presets stored in the
		 * manager.
		 *
		 * When emitted, this signal is always emitted before presetsChanged(),
		 * never after.
		 */
		void presetAdded(Preset * preset, int index);

		/**
		 * \brief Emitted when a preset has been removed.
		 *
		 * When emitted, this signal is always emitted before presetsChanged(),
		 * never after.
		 */
		void presetRemoved(void);

		/**
		 * \brief Emitted when a preset has been removed.
		 *
		 * When emitted with either presetAdded() or presetRemoved(), this
		 * signal is always emitted after the other, never before.
		 */
		void presetsChanged(void);

		/**
		 * \brief Emitted when the application preferences have changed.
		 */
		void preferencesChanged(void);

		/**
		 * \brief Emitted when a process has been started.
		 *
		 * \param process is the process created.
		 *
		 * Extreme care needs to be taken when connecting to this signal. It is
		 * emitted whenever a call to simulate() or execute() produces a valid
		 * process. The process provided with the signal is owned elsewhere. The
		 * manager does not guarantee its lifespan and the class(es) connecting
		 * to the signal do not own it. The process could therefore be deleted
		 * at any time without notice. Connecting classes may listen for signals
		 * from the process and connect to its slots, but may not call any
		 * method or otherwise manipulate the process.
		 *
		 * This means that the process may be invalid by the time your class
		 * receives this signal, that the process may never get started and that
		 * the process may never emit any signals at all.
		 */
		void processStarted(Process * process) const;

	protected:
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
		void setLastError(const QString & err) const;


	private:
		static QString s_rsyncVersionText;
		static QString s_configPath;

		std::unique_ptr<Preferences> m_prefs;
		mutable QString m_lastError;
		QList<Preset *> m_presets;
	};

}  // namespace Qync

#define qyncApp (static_cast<Qync::Application *>(QApplication::instance()))

#endif  // QYNC_APPLICATION_H
