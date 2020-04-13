/**
 * \file process.cpp
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Implementation of the Process class.
 *
 * \note This class uses some C++17 features:
 * - initialisations statements in if
 *
 * \dep
 * - map
 * - process.h
 * - QDebug
 * - QProcess
 * - QFile
 * - QRegularExpression
 * - preset.h
 * - application.h
 * - preferences.h
 */

#include "process.h"

#include <map>

#include <QDebug>
#include <QProcess>
#include <QHash>
#include <QFile>
#include <QRegularExpression>
#include <QtGlobal>

#include "preset.h"
#include "application.h"
#include "preferences.h"


namespace Qync {


	/**
	 * \class Process
	 * \author Darren Edale
	 * \date October 2017
	 * \version 1.1.0
	 *
	 * \brief Wraps the rsync process.
	 *
	 * The QProcess for the rsync command is wrapped inside an object of this class.
	 * The command, arguments and optional log file are set in the constructor from
	 * the Preset object provided, and cannot later be modified - this is intended
	 * as a read-only class that is provided by the application to represent processes
	 * and as such the manager exercises complete control over its attributes. The
	 * command, arguments and log file name are accessible with the command(),
	 * arguments() and logFile() methods.
	 *
	 * The start() method starts the command. and the stop() method interrupts the
	 * processing of the command. The actual QProcess can be retrieved with the
	 * process() method. The process is only available if the command is either
	 * currently running or has been run at some point in the past - i.e. the
	 * start() method has been successfully called at some point in the object's
	 * existence.
	 *
	 * The class monitors the output and error streams of the process and emits
	 * signals when significant events occur. The finished() signal indicates that
	 * the process completed successfully; the error() signal indicates that an
	 * error was encountered (but not necessarily that the error was fatal). The
	 * interrupted() signal occurs when the rsync process was interrupted for any
	 * reason, including when the stop() slot is called. This signal indicates that
	 * the rsync process has terminated prematurely. The failed() signal is
	 * emitted when the rsync process has failed (except when it has been
	 * interrupted - that is interrupted() and failed() are mutually exclusive).
	 *
	 * To monitor the progress of the rsync process, the newItemStarted(),
	 * itemProgress(), itemProgressBytes() and overallProgress() signals are
	 * available. The newItemStarted() signal indicates that rsync has started the
	 * synchronisation of a new file or directory; itemProgress() and
	 * itemProgressBytes() indicate how much of the current item has been
	 * synchronised as a percentage of its total size and in bytes respectively. The
	 * overallProgress() signal is not yet implemented, but it is intended to
	 * indicate an overall percentage progress for the rsync command as a whole.
	 *
	 * Finally, if you are interested in capturing the standard output stream of
	 * the process in its entirety, you can do so by connecting to the
	 * standardOutputUpdated() signal. The argument provided with this signal is all
	 * the data from the standard output stream since the last time the signal was
	 * emitted.
	 */


	/**
	 * \brief Create a new Process.
	 *
	 * \param preset is the preset from which to create the process.
	 * \param type The type of rsync process to create.
	 *
	 * The type argument should be RunType::Normal (the default) for normal execution
	 * of rsync or RunType::DryRun to perform a dry run. A dry run of rsync does
	 * everything a normal run of rsync would do, except make any actual changes to
	 * the destination. This run type is generally used for simulations.
	 *
	 * The rsync command to run is gathered directly from the application
	 * preferences.
	 */
	Process::Process(const Preset & preset, RunType type)
	: Process(qyncApp->preferences().rsyncPath(), preset, type) {
	}


	/**
	 * \brief Create a new Process.
	 *
	 * \param cmd is the path to the rsync command to run.
	 * \param preset is the preset from which to create the process.
	 * \param type The type of rsync process to create.
	 *
	 * The type argument should be RunType::Normal (the default) for normal execution
	 * of rsync or RunType::DryRun to perform a dry run. A dry run of rsync does
	 * everything a normal run of rsync would do, except make any actual changes to
	 * the destination. This run type is generally used for simulations.
	 */
	Process::Process(const QString & cmd, const Preset & preset, RunType type)
	: QObject(),
	  m_process(new QProcess),
	  m_command(cmd),
	  m_runType(type) {
		m_logFileName = preset.logFile();

		if(RunType::DryRun == type) {
			m_args = rsyncArguments(preset, {"--dry-run"});
		}
		else {
			m_args = rsyncArguments(preset);
		}
	}


	/**
	 * \brief Destroy the Process.
	 */
	Process::~Process() = default;


	/**
	 * \brief Build a set of rsync arguments.
	 *
	 * \param preset is the preset from which to build the arguments.
	 * \param forceOptions is a set of extra rsync arguments to add to the
	 * returned list.
	 *
	 * Given a Preset object, this method will produce the set of
	 * arguments for \b rsync that correstponds to the settings in the
	 * preset. The list of arguments returned is suitable for use as
	 * the \b args parameter for a call to QProcess::start();
	 *
	 * It is possible to force the use of certain \b rsync arguments using
	 * the forceOptions parameter. Any options in this list are inserted
	 * into the returned list, even if this means an argument appears in the
	 * list more than once.
	 *
	 *\return A list of arguments.
	 */
	QStringList Process::rsyncArguments(const Preset & preset, const QStringList & forceOptions) {
		if(preset.source().isEmpty()) {
			qCritical() << __PRETTY_FUNCTION__ << "Preset's source is empty";
			return {};
		}

		if(preset.destination().isEmpty()) {
			qCritical() << __PRETTY_FUNCTION__ << "Preset's destination is empty";
			return {};
		}

		QStringList args(forceOptions);
		args.push_back("--recursive");
		args.push_back("--progress");
		args.push_back("--verbose");
		args.push_back("--out-format=f%n %l");

		/* basic settings */
		if(preset.preserveTime()) {
			args.push_back("--times");
		}

		if(preset.preservePermissions()) {
			args.push_back("--perms");
		}

		if(preset.preserveOwner()) {
			args.push_back("--owner");
		}

		if(preset.preserveGroup()) {
			args.push_back("--group");
		}

		if(preset.windowsCompatability()) {
			args.push_back("--modify-window=1");
		}

		if(preset.honourDeletions()) {
			args.push_back("--delete");
		}

		/* advanced settings */
		if(preset.ignoreTimes()) {
			args.push_back("--ignore-times");
		}

		if(preset.alwaysCompareChecksums()) {
			args.push_back("--checksum");
		}

		if(preset.preserveDevices()) {
			args.push_back("--devices");
		}

		if(preset.keepPartialTransfers()) {
			args.push_back("--partial");
		}

		if(preset.copySymlinksAsSymlinks()) {
			args.push_back("--links");
		}

		if(preset.makeBackups()) {
			args.push_back("--backup");
		}

		if(preset.useTransferCompression()) {
			args.push_back("--compress");
		}

		if(preset.onlyUpdateExistingEntries()) {
			args.push_back("--existing");
		}

		if(preset.dontUpdateExistingEntries()) {
			args.push_back("--ignore-existing");
		}

		if(preset.dontMapUsersAndGroups()) {
			args.push_back("--numeric-ids");
		}

		if(preset.copyHardlinksAsHardlinks()) {
			args.push_back("--hard-links");
		}

		if(preset.showItemisedChanges()) {
			args.push_back("--itemize-changes");
		}

		/* source and dest */
		args.push_back(preset.source());
		args.push_back(preset.destination());

		return args;
	}


	/**
	 * \brief Provides a default explanation of an exit code.
	 *
	 * \param code is the exit code to explain.
	 *
	 * The explanation will be a null string if the code is not valid.
	 *
	 * \return The explanation.
	 */
	const QString & Process::defaultExitCodeMessage(const Process::ExitCode & code) {
		static const QString emptyString;

		static std::map<ExitCode, QString> s_messages = {
		  {ExitCode::Success, tr("The rsync process completed successfully.")},
		  {ExitCode::SyntaxError, tr("The rsync process could not be started because one or more arguments were not valid.")},
		  {ExitCode::ProtocolIncompatibility, tr("The rysnc process could not be started because the source and destination rsync versions are incompatible.")},
		  {ExitCode::FileSelectionError, tr("The rsync process failed because one or more source files or directories could not be found.")},
		  {ExitCode::UnsupportedAction, tr("The rsync process failed because the action requested is not supported.\n\nThis usually means you attempted to transfer data from a 32-bit computer to a 64-bit computer, or vice versa.")},
		  {ExitCode::TransmissionProtocolStartupError, tr("The rsync process failed because it was unable to initialise its data transmission protocol.")},
		  {ExitCode::UnableToWriteLogFile, tr("The rsync process completed but was unable to write to its log file.")},
		  {ExitCode::SocketIoError, tr("The rsync process failed because it encountered a network input/output error.")},
		  {ExitCode::FileIoError, tr("The rsync process failed because it encountered a file input/output error.")},
		  {ExitCode::DataStreamError, tr("The rsync process failed because of a failure in the transmission protocol's data stream.")},
		  {ExitCode::ProgramDiagnosticsError, tr("The rsync process failed because it was unable successfully to complete some internal diagnostics.")},
		  {ExitCode::IpcCodeError, tr("The rsync process failed because of an inter-process communication problem.")},
		  {ExitCode::InterruptReceived, tr("The rsync process was interrupted by the operating system.")},
		  {ExitCode::WaitpidError, tr("The rsync process failed while waiting for a process state to change.")},
		  {ExitCode::MemoryAllocationError, tr("The rsync process failed because it was unable to allocate some memory.")},
		  {ExitCode::PartialTransferError, tr("The rsync process completed but some files or directories were only partially transferred.\n\nThis could mean that the destination filesystem does not support some features (such as access permissions or ownership).")},
		  {ExitCode::VanishedSourceFile, tr("The rsync process failed because a source file or directory vanished while rsync was transferring its contents.")},
		  {ExitCode::MaximumDeletionsExceeded, tr("The rsync process aborted because the maximum number of deletions was exceeded.")},
		  {ExitCode::DataTransmissionTimeout, tr("The rsync process failed because it had to wait too long for data to be transmitted.")},
		  {ExitCode::ConnectionTimeout, tr("The rsync process failed because its network connection timed out.")},
		};

		if(s_messages.end() == s_messages.find(code)) {
			return emptyString;
		}

		return s_messages[code];
	}


	/**
	 * \brief Start the process.
	 *
	 * Once the process has started, the started() signal will be emitted
	 * and other progress and information signals will start to be emitted.
	 */
	void Process::start() {
		Q_ASSERT(m_process);
		m_process->start(m_command, m_args);

		if(!m_logFileName.isEmpty()) {
			m_logFile.setFileName(m_logFileName);
			m_logFile.open(QIODevice::WriteOnly);
		}

		m_stdoutConnection = connect(m_process.get(), &QProcess::readyReadStandardOutput, this, &Process::parseStdout);
		//		connect(m_process.get(), &QProcess::readyReadStandardError, this, &Process::parseStderr);
		connect(m_process.get(), qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &Process::onProcessFinished);
		Q_EMIT started();
	}


	/**
	 * \brief Attempt to stop the process.
	 *
	 * If the process has been started, this will attempt to interrupt it.
	 * If the attempt is successful, the interrupted() signal will be
	 * emitted. No error messages will be generated by a call to this slot.
	 */
	void Process::stop() {
		Q_ASSERT(m_process);
		if(m_process->state() != QProcess::NotRunning) {
			m_process->close();
			Q_EMIT interrupted("");
			disconnect(m_stdoutConnection);
		}
	}


	/**
	 * \brief Read the process's error output stream.
	 *
	 * This method is not currently used, but its intention is to read
	 * the standard error stream of the rsync process and emit an
	 * appropriate error message.
	 */
	void Process::parseStderr() {
		Q_ASSERT(m_process);

		//	QString data = m_process->readAllStandardError();
		//	Q_EMIT error(data));
	}


	/**
	 * \brief Parse the standard output stream of the rsync process.
	 *
	 * The process's output stream is captured and parsed. The following
	 * information is gathered from the output stream:
	 * - the progress of the current transfer
	 * - when a new item is starting to be transferred
	 * - overall stats about the transfer when rsync has finished
	 *
	 * The data gathered is emitted as signals from this class.
	 */
	void Process::parseStdout() {
		// captures:
		// 1: current item bytes transferred
		// 2: current item transferred %
		// 3: current transfer speed
		// 4: transfer speed unit (M, k or G for Mb, kb or Gb per second respectively)
		// 5: transfer time, # of hours
		// 6: transfer time, # of minutes
		// 7: transfer time, # of seconds
		// 8: (optional) transfer number
		// 9: (optional) # of items left to check
		// 10: (optinal) total number of items
		static QRegularExpression progressLine(" *(\\d+|\\d+(?:,\\d{3})*) *(\\d+)% *(\\d+\\.\\d{2})(.)B/s *(\\d+):(\\d{2}):(\\d{2})(?: +\\(xfr#(\\d+), (?:to|ir)-chk.(\\d+)\\/(\\d+)\\))?");

		// captures:
		// 1: new item path
		// 2: new item size in bytes
		static QRegularExpression newItemLine("f(.*) (\\d+)");

		// captures:
		// 1: total bytes sent
		// 2: total bytes received
		// 3: overall speed in bytes per second
		static QRegularExpression completedLine("sent (\\d+|\\d+(?:,\\d{3})*) bytes *received (\\d+|\\d+(?:,\\d{3})*) bytes *((?:\\d+|\\d+(?:,\\d{3})*)(?:\\.(\\d{2}))?) bytes/sec");

		Q_ASSERT_X(m_process, __PRETTY_FUNCTION__, "no process from which to read output");
		QString data = m_process->readAllStandardOutput();

		if(m_logFile.isOpen()) {
			m_logFile.write(data.toUtf8());
		}

		if(!data.isEmpty()) {
			/* this parses the output of rsync for lines indicating progress/new item started/completion */
			m_outputCache.append(data);
			m_outputCache = m_outputCache.replace('\r', '\n');
			QStringList lines(m_outputCache.split("\n"));

			/* parse all but the last line (since the last line might not be complete */
			int n = lines.size() - 1;

			for(int i = 0; i < n; i++) {
				data = lines[i];

				if(data.isEmpty()) {
					continue;
				}

				/* WARNING C++17 feature: if initialisation statements */
				if(QRegularExpressionMatch progressLineMatch = progressLine.match(data); progressLineMatch.hasMatch()) {
					QStringList caps = progressLineMatch.capturedTexts();

					int itemBytes = caps[1].remove("[^0-9]+").toInt();
					int itemPc = caps[2].toInt();
					float xferSpeed = caps[3].toFloat();

					/* whatever unit the speed is expressed in, convert it to bytes per sec */
					switch(caps[4][0].toLower().toLatin1()) {
						case 'k':
							xferSpeed *= 1024;
							break;

						case 'm':
							xferSpeed *= (1024 * 1024);
							break;

						case 'g':
							xferSpeed *= (1024 * 1024 * 1024);
							break;
					}

					int seconds = (caps[5].toInt() * 3600) + (caps[6].toInt() * 60) + caps[7].toInt();

					Q_EMIT transferSpeed(xferSpeed);
					Q_EMIT itemProgressBytes(itemBytes);
					Q_EMIT itemProgress(itemPc);
					Q_EMIT itemSecondsRemaining(seconds);

					if(11 <= caps.size()) {
						/* transferNumber = capture # 8 */
						int remainingItems = caps[9].toInt();
						int totalItems = caps[10].toInt();
						int completedItems = totalItems - remainingItems;
						float overallPc = static_cast<float>((completedItems * 100.0) / totalItems);
						Q_EMIT overallProgress(static_cast<int>(overallPc));
					}
				}
				else if(QRegularExpressionMatch newItemMatch = newItemLine.match(data); newItemMatch.hasMatch()) {
					Q_EMIT newItemStarted(newItemMatch.captured(1));
				}
				else if(QRegularExpressionMatch completedLineMatch = completedLine.match(data); completedLineMatch.hasMatch()) {
					Q_EMIT transferSpeed(completedLineMatch.captured(3).replace(',', "").toFloat());
				}
			}

			if(!lines.empty()) {
				m_outputCache = lines[lines.size() - 1];
			}
		}
	}


	/**
	 * \brief Called when the QProcess finishes.
	 *
	 * This method is connected to the QProcess::finished() signal.
	 */
	void Process::onProcessFinished() {
		Q_ASSERT_X(m_process, __PRETTY_FUNCTION__, "called without a QProcess object");
		m_logFile.close();
		m_outputCache.clear();
		ExitCode code = static_cast<ExitCode>(m_process->exitCode());
		const QString & msg = defaultExitCodeMessage(code);
		Q_EMIT finished(code);

		switch(code) {
			case ExitCode::Success:
			case ExitCode::PartialTransferError:
			case ExitCode::UnableToWriteLogFile:
				Q_EMIT finished(msg);
				break;

			case ExitCode::InterruptReceived:
				Q_EMIT interrupted(msg);
				break;

			case ExitCode::SyntaxError:
			case ExitCode::ProtocolIncompatibility:
			case ExitCode::FileSelectionError:
			case ExitCode::UnsupportedAction:
			case ExitCode::TransmissionProtocolStartupError:
			case ExitCode::SocketIoError:
			case ExitCode::FileIoError:
			case ExitCode::DataStreamError:
			case ExitCode::ProgramDiagnosticsError:
			case ExitCode::IpcCodeError:
			case ExitCode::WaitpidError:
			case ExitCode::MemoryAllocationError:
			case ExitCode::VanishedSourceFile:
			case ExitCode::MaximumDeletionsExceeded:
			case ExitCode::DataTransmissionTimeout:
			case ExitCode::ConnectionTimeout:
				Q_EMIT failed(msg);
				break;
		}
	}


	/**
	 * \fn Process::started()
	 * \brief Emitted when the \b rsync process has started.
	 *
	 * This is usually emitted as a result of a call to the start() slot.
	 */


	/**
	 * \fn Process::newItemStarted(QString)
	 * \brief Emitted when the \b rsync has started processing a
	 * different file or directory.
	 *
	 * \param item is the path to the item whose processing has started.
	 *
	 * The item path is relative to the source.
	 */


	/**
	 * \fn Process::itemProgress(int)
	 * \brief Emitted when the progress of the current item being processed
	 * has changed.
	 *
	 * \param progress is the new progress for the item in %.
	 */


	/**
	 * \fn Process::itemProgressBytes(int)
	 * \brief Emitted when the progress of the current item being processed
	 * has changed.
	 *
	 * \param progress is the new progress for the item in bytes.
	 */


	/**
	 * \fn Process::overallProgress(int progress);
	 * \brief Emitted when overall progress has changed.
	 *
	 * \param progress is the new overall progress in %.
	 */


	/**
	 * \fn Process::transferSpeed(float)
	 * \brief Emitted when an update to the transfer speed is available.
	 *
	 * \param bytesPerSecond is the current transfer speed in bytes per second.
	 */


	/**
	 * \fn Process::finished(Process::ExitCode)
	 * \brief Emitted when \b rsync has finished.
	 *
	 * \param code is the exit code.
	 */


	/**
	 * \fn Process::finished(QString)
	 * \brief Emitted when \b rsync has finished.
	 *
	 * \param msg is a user-friendly message indicating how the process
	 * finished. It could be empty, in wich case the signal receiver is
	 * entitled to assume the process finished completely successfully.
	 * In most cases, the message provided is a warning that the process
	 * finished successfully, with some caveats.
	 */


	/**
	 * \fn Process::interrupted(QString)
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


	/**
	 * \fn Process::failed(QString)
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


	/**
	 * \fn Process::error(QString)
	 * \brief Emitted when \b rsync has encountered an error.
	 *
	 * \param err is the error message.
	 *
	 * The error will be one generated by rsync, which is likely to be
	 * fairly cryptic unless the end user is familiar with \b rsync. It
	 * is recommended that errors emitted by this signal are introduced
	 * to the user gently!
	 */


}  // namespace Qync
