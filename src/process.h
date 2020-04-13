/**
 * @file process.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the Process class.
 */

#ifndef QYNC_PROCESS_H
#define QYNC_PROCESS_H

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QMetaObject>

class QProcess;

namespace Qync {

	class Preset;
	class Preferences;

	class Process
	: public QObject {
		Q_OBJECT

	public:
		/* based on v3.0.7 of rsync. see the rsync man page */
		enum class ExitCode {
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

		enum class RunType : unsigned char {
			Normal = 0,
			DryRun,
		};

		explicit Process(const Preset & preset, RunType type = RunType::Normal);
		Process(QString cmd, const Preset & preset, RunType type = RunType::Normal);
		~Process() override;

		[[nodiscard]] inline RunType runType() const {
			return m_runType;
		}

		[[nodiscard]] inline bool isDryRun() const {
			return RunType::DryRun == m_runType;
		}

	Q_SIGNALS:
		void started();
		void newItemStarted(QString);
		void itemProgress(int);
		void itemProgressBytes(int);
		void itemSecondsRemaining(int);
		void overallProgress(int);
		void transferSpeed(float);
		void finished(Process::ExitCode);
		void finished(QString);
		void interrupted(QString);
		void failed(QString);
		void error(QString);

	public Q_SLOTS:
		void start();
		void stop();

	private Q_SLOTS:
		void parseStdout();
		void parseStderr();
		void onProcessFinished();

	protected:
		static QStringList rsyncArguments(const Preset &, const QStringList & = {});
		static const QString & defaultExitCodeMessage(const Process::ExitCode &);

	private:
		std::unique_ptr<QProcess> m_process;
		QString m_command;
		RunType m_runType;
		QStringList m_args;
		QString m_logFileName;
		QFile m_logFile;
		QString m_outputCache;
		QMetaObject::Connection m_stdoutConnection;
	};

}  // namespace Qync

#endif  // QYNC_PROCESS_H
