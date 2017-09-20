/**
 * \file Manager.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.5
 *
 * \brief Declaration of the Manager class.
 *
 * \dep
 * - memory
 * - QApplication
 * - QList
 * - QString
 */

#ifndef QYNC_APPLICATION_H
#define QYNC_APPLICATION_H

#include <memory>

#include <QApplication>
#include <QList>
#include <QString>

namespace Qync {

	class Preset;
	class Process;
	class Preferences;

	class Application
	  : public QApplication {
		Q_OBJECT

	public:
		Application(int & argc, char ** argv);
		~Application(void);

		static QString configurationPath(void);
		static QString presetsPath(void);

		QString lastError(void) const;
		QString rsyncVersionText(void);

		const Preferences * preferences(void) const;
		bool setPreferences(Preferences * prefs);

		inline int presetCount(void) const {
			return m_presets.size();
		}

		inline const QList<Preset *> presets(void) const {
			return m_presets;
		}

		Preset * preset(int index) const;
		bool removePreset(int index);
		bool removePreset(Preset * preset);
		bool insertPreset(Preset * preset, int index = -1);

		inline bool addPreset(Preset * preset) {
			return insertPreset(preset, -1);
		}

		void clearPresets(void);

		Process * simulate(int i) const;
		Process * simulate(const Preset * preset) const;
		Process * synchronise(int i) const;
		Process * synchronise(const Preset * preset) const;

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
		static QString s_presetsPath;

		std::unique_ptr<Preferences> m_prefs;
		mutable QString m_lastError;
		QList<Preset *> m_presets;
	};

}  // namespace Qync

#define qyncApp (static_cast<Qync::Application *>(QApplication::instance()))

#endif  // QYNC_APPLICATION_H
