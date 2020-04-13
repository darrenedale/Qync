/**
 * \file process.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the Process class.
 *
 * \dep
 * - memory
 * - QtCore/QObject
 * - QtCore/QString
 * - QtCore/QStringList
 * - QtCore/QFile
 * - QtCore/QMetaObject
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

		inline RunType runType() const {
			return m_runType;
		}

		inline bool isDryRun() const {
			return RunType::DryRun == m_runType;
		}

	Q_SIGNALS:
		void started();
		void newItemStarted(QString item);
		void itemProgress(int progress);
		void itemProgressBytes(int progress);
		void itemSecondsRemaining(int seconds);
		void overallProgress(int progress);
		void transferSpeed(float bytesPerSecond);
		void finished(Process::ExitCode code);
		void finished(QString msg);
		void interrupted(QString msg);
		void failed(QString msg);
		void error(QString err);

	public Q_SLOTS:
		void start();
		void stop();

	private Q_SLOTS:
		void parseStdout();
		void parseStderr();
		void onProcessFinished();

	protected:
		static QStringList rsyncArguments(const Preset & preset, const QStringList & forceOptions = {});
		static const QString & defaultExitCodeMessage(const Process::ExitCode & code);

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
