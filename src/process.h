/**
 * \file Process.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.5
 *
 * \brief Declaration of the Process class.
 *
 * The Process class takes the command and arguments from the manager
 * rather than just being given the preset. This is because the preset is
 * owned by the manager and could be removed while the Process object is
 * still alive. In this scenario, the Process object would retain a
 * pointer to an object that no longer exists, and therefore any attempt by
 * the Process object to access the preset would result in a segfault.
 * Therefore, the code to build the rsync command must remain in the Manager
 * class.
 *
 * \dep
 * - memory
 * - QObject
 * - QString
 * - QStringList
 * - QHash
 */

#ifndef QYNC_PROCESS_H
#define QYNC_PROCESS_H

#include <memory>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QFile>

class QProcess;

namespace Qync {

	class Preset;
	class Preferences;

	class Process
	  : public QObject {
		Q_OBJECT
	public:
		/* based on v3.0.7 of rsync. see the rsync man page */
		enum ExitCode {
			Success = 0,
			SyntaxError = 1,
			ProtocolIncompatibility = 2,
			FileSelectionError = 3,
			UnsupportedAction = 4,
			TransmissionProtocolStartupError = 5,
			UnableToWriteLogFile = 6,
			SocketIoError = 10,
			FileIoError = 11,
			DataStreamError = 12,
			ProgramDiagnosticsError = 13,
			IpcCodeError = 14,
			InterruptReceived = 20,
			WaitpidError = 21,
			MemoryAllocationError = 22,
			PartialTransferError = 23,
			VanishedSourceFile = 24,
			MaximumDeletionsExceeded = 25,
			DataTransmissionTimeout = 30,
			ConnectionTimeout = 35
		};

		Process(const Preset * preset);
		Process(const QString & cmd, const Preset * preset);
		virtual ~Process(void);

		static QStringList rsyncArguments(const Preset * preset, const QStringList & forceOptions = QStringList());
		static QString defaultExitCodeMessage(const Process::ExitCode & code);

		inline QProcess * process(void) {
			return m_process.get();
		}

		inline QString command(void) const {
			return m_command;
		}

		inline QStringList arguments(void) const {
			return m_args;
		}

		inline QString logFile(void) const {
			return m_logFileName;
		}

	Q_SIGNALS:
		/**
		 * \brief Emitted when the \b rsync process has started.
		 *
		 * This is usually emitted as a result of a call to the start() slot.
		 */
		void started();

		/**
		 * \brief Emitted when the \b rsync has started processing a
		 * different file or directory.
		 *
		 * \param item is the path to the item whose processing has started.
		 *
		 * The item path is relative to the source.
		 */
		void newItemStarted(QString item);

		/**
		 * \brief Emitted when the progress of the current item being processed
		 * has changed.
		 *
		 * \param progress is the new progress for the item in %.
		 */
		void itemProgress(int progress);

		/**
		 * \brief Emitted when the progress of the current item being processed
		 * has changed.
		 *
		 * \param progress is the new progress for the item in bytes.
		 */
		void itemProgressBytes(int progress);

		/**
		 * \brief Emitted when overall progress has changed.
		 *
		 * \param progress is the new overall progress in %.
		 */
		void overallProgress(int progress);

		/**
		 * \brief Emitted when \b rsync has finished.
		 *
		 * \param code is the exit code.
		 */
		void finished(Process::ExitCode code);

		/**
		 * \brief Emitted when \b rsync has finished.
		 *
		 * \param msg is a user-friendly message indicating how the process
		 * finished. It could be empty, in wich case the signal receiver is
		 * entitled to assume the process finished completely successfully.
		 * In most cases, the message provided is a warning that the process
		 * finished successfully, with some caveats.
		 */
		void finished(QString msg);

		/**
		 * \brief Emitted when \b rsync has been interrupted.
		 *
		 * \param msg is the reason for interruption.
		 *
		 * The source of the interruption could be a call to the stop() slot, or
		 * some other interruption.
		 *
		 * failed() and interrupted() are mutually exclusive. While an
		 * interruption usually implies failure, it does not result in the
		 * failed() signal being emitted. Interruption is defined as when an
		 * external interruption halted rsync; failure is defined as when rsync
		 * itself determined it was unable to complete its task successfully.
		 *
		 * \sa failed()
		 */
		void interrupted(QString msg);

		/**
		 * \brief Emitted when \b rsync has failed.
		 *
		 * \param msg is the reason for failure.
		 *
		 * The reason for failure is not always known so the argument emitted
		 * with this signal can be empty or null.
		 *
		 * failed() and interrupted() are mutually exclusive. While an
		 * interruption usually implies failure, it does not result in the
		 * failed() signal being emitted. Interruption is defined as when an
		 * external interruption halted rsync; failure is defined as when rsync
		 * itself determined it was unable to complete its task successfully.
		 *
		 * \sa interrupted()
		 */
		void failed(QString msg);

		/**
		 * \brief Emitted when \b rsync has encountered an error.
		 *
		 * \param err is the error message.
		 *
		 * The error will be one generated by rsync, which is likely to be
		 * fairly cryptic unless the end user is familiar with \b rsync. It
		 * is recommended that errors emitted by this signal are introduced
		 * to the user gently!
		 */
		void error(QString err);

		/**
		 * \brief Emitted when more data has been written to \b rsync's
		 * standard output stream.
		 *
		 * \param data is the data written to standard output since this signal
		 * was last emitted, or since the process started if this is the first
		 * time the signal has been emitted.
		 */
		void standardOutputUpdated(QString data);

	public Q_SLOTS:
		void start(void);
		void stop(void);

	private Q_SLOTS:
		void parseStdout(void);
		void parseStderr(void);
		void processFinished(void);

	private:
		std::unique_ptr<QProcess> m_process;
		QString m_command;
		QStringList m_args;
		QString m_logFileName;
		QFile m_logFile;
		QString m_outputCache;
		QMetaObject::Connection m_stdoutConnection;
	};

}  // namespace Qync

#endif  // QYNC_PROCESS_H
