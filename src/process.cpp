/**
 * \file process.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.7
 *
 * \brief Implementation of the Process class.
 *
 * \dep
 * - process.h
 * - QDebug
 * - QProcess
 * - QHash
 * - QFile
 * - QRegularExpression
 * - preset.h
 * - application.h
 * - preferences.h
 */

#include "process.h"

#include <QDebug>
#include <QProcess>
#include <QHash>
#include <QFile>
#include <QRegularExpression>

#include "preset.h"
#include "application.h"
#include "preferences.h"


namespace Qync {


	/**
	 * \class Process
	 * \author Darren Edale
	 * \date September 2017
	 * \version 0.9.7
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
	 *
	 * The rsync command to run is gathered directly from the application
	 * preferences.
	 */
	Process::Process(const Preset & preset)
	: Process(qyncApp->preferences()->rsyncPath(), preset) {
	}


	/**
	 * \brief Create a new Process.
	 *
	 * \param cmd is the path to the rsync command to run.
	 * \param preset is the preset from which to create the process.
	 */
	Process::Process(const QString & cmd, const Preset & preset)
	: QObject(),
	  m_process(new QProcess),
	  m_command(cmd) {
		m_logFileName = preset.logFile();
		m_args = rsyncArguments(preset);
	}


	/**
	 * \brief Destroy the Process.
	 */
	Process::~Process(void) = default;


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

		/* TODO use --info= instead of --progress (see rsync --info=help for details of how) */
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
	QString Process::defaultExitCodeMessage(const Process::ExitCode & code) {
		static QHash<Process::ExitCode, QString> s_messages;

		if(s_messages.size() == 0) {
			s_messages[Success] = tr("The rsync process completed successfully.");
			s_messages[SyntaxError] = tr("The rsync process could not be started because one or more arguments were not valid.");
			s_messages[ProtocolIncompatibility] = tr("The rysnc process could not be started because the source and destination rsync versions are incompatible.");
			s_messages[FileSelectionError] = tr("The rsync process failed because one or more source files or directories could not be found.");
			s_messages[UnsupportedAction] = tr("The rsync process failed because the action requested is not supported.\n\nThis usually means you attempted to transfer data from a 32-bit computer to a 64-bit computer, or vice versa.");
			s_messages[TransmissionProtocolStartupError] = tr("The rsync process failed because it was unable to initialise its data transmission protocol.");
			s_messages[UnableToWriteLogFile] = tr("The rsync process completed but was unable to write to its log file.");
			s_messages[SocketIoError] = tr("The rsync process failed because it encountered a network input/output error.");
			s_messages[FileIoError] = tr("The rsync process failed because it encountered a file input/output error.");
			s_messages[DataStreamError] = tr("The rsync process failed because of a failure in the transmission protocol's data stream.");
			s_messages[ProgramDiagnosticsError] = tr("The rsync process failed because it was unable successfully to complete some internal diagnostics.");
			s_messages[IpcCodeError] = tr("The rsync process failed because of an inter-process communication problem.");
			s_messages[InterruptReceived] = tr("The rsync process was interrupted by the operating system.");
			s_messages[WaitpidError] = tr("The rsync process failed while waiting for a process state to change.");
			s_messages[MemoryAllocationError] = tr("The rsync process failed because it was unable to allocate some memory.");
			s_messages[PartialTransferError] = tr("The rsync process completed but some files or directories were only partially transferred.\n\nThis could mean that the destination filesystem does not support some features (such as access permissions or ownership).");
			s_messages[VanishedSourceFile] = tr("The rsync process failed because a source file or directory vanished while rsync was transferring its contents.");
			s_messages[MaximumDeletionsExceeded] = tr("The rsync process aborted because the maximum number of deletions was exceeded.");
			s_messages[DataTransmissionTimeout] = tr("The rsync process failed because it had to wait too long for data to be transmitted.");
			s_messages[ConnectionTimeout] = tr("The rsync process failed because its network connection timed out.");
		}

		return s_messages.value(code);
	}


	/**
	 * \fn Process::process(void)
	 * \brief Retrieve the actual process object.
	 *
	 * The process object can be \b null if the process has yet to be
	 * started. It usually remains non-null after the process has finished
	 * but this is not guaranteed.
	 *
	 * \return The process.
	 */


	/**
	 * \fn Process::command(void) const
	 * \brief Retrieve the \b rsync program path.
	 *
	 * \return The \b rsync path.
	 */


	/**
	 * \fn Process::arguments(void) const
	 * \brief Retrieve the \b rsync arguments.
	 *
	 * \return The arguments.
	 */


	/**
	 * \fn Process::logFile(void) const
	 * \brief Retrieve the output log file path.
	 *
	 * \return The output log file path.
	 */


	/**
	 * \brief Start the process.
	 *
	 * Once the process has started, the started() signal will be emitted
	 * and other progress and information signals will start to be emitted.
	 */
	void Process::start(void) {
		Q_ASSERT(m_process);
		qDebug() << "starting rsync with options:\n"
					<< m_args.join(' ');
		m_process->start(m_command, m_args);

		if(!m_logFileName.isEmpty()) {
			m_logFile.setFileName(m_logFileName);
			m_logFile.open(QIODevice::WriteOnly);
		}

		m_stdoutConnection = connect(m_process.get(), &QProcess::readyReadStandardOutput, this, &Process::parseStdout);
		connect(m_process.get(), &QProcess::readyReadStandardError, this, &Process::parseStderr);
		connect(m_process.get(), static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &Process::processFinished);
		Q_EMIT started();
	}


	/**
	 * \brief Attempt to stop the process.
	 *
	 * If the process has been started, this will attempt to interrupt it.
	 * If the attempt is successful, the interrupted() signal will be
	 * emitted. No error messages will be generated by a call to this slot.
	 */
	void Process::stop(void) {
		Q_ASSERT(m_process);
		if(m_process->state() != QProcess::NotRunning) {
			m_process->close();
			Q_EMIT interrupted("");
			disconnect(m_stdoutConnection);
		}
	}


	void Process::parseStderr(void) {
		Q_ASSERT(m_process);

		//	QString data = m_process->readAllStandardError();
		//	Q_EMIT error(data));
	}


	void Process::parseStdout(void) {
		static QRegularExpression progressLine(" *(\\d+|\\d+(?:,\\d{3})*) *(\\d+)% *(\\d+\\.\\d{2})(.)B/s *(\\d+):(\\d{2}):(\\d{2}).*(?:\\(.*, (?:to|ir)\\-chk=(\\d+)/(\\d+)\\))?");
		static QRegularExpression newItemLine("f(.*) (\\d+)");
		static QRegularExpression completedLine("sent (\\d+|\\d+(?:,\\d{3})*) bytes *received (\\d+|\\d+(?:,\\d{3})*) bytes *((?:\\d+|\\d+(?:,\\d{3})*)(?:\\.(\\d{2}))?) bytes/sec");

		Q_ASSERT(m_process);
		//if(m_process->state() == QProcess::NotRunning) return;
		qDebug() << __PRETTY_FUNCTION__ << "reading stdout from process";
		QString data = m_process->readAllStandardOutput();

		if(m_logFile.isOpen()) {
			m_logFile.write(data.toUtf8());
		}

		if(!data.isEmpty()) {
			Q_EMIT standardOutputUpdated(data);
			m_outputCache.append(data);

			/* this parses the output of rsync for lines indicating progress */
			m_outputCache = m_outputCache.replace('\r', '\n');

			QStringList lines(m_outputCache.split("\n"));

			for(int i = 0; i < lines.size() - 1; i++) {
				data = lines[i];

				if(data.isEmpty()) {
					continue;
				}

				/* WARNING c++17 feature: if initialisation statements */
				if(QRegularExpressionMatch progressLineMatch = progressLine.match(data); progressLineMatch.hasMatch()) {
					/* TODO can the thousands separator be ' or . depending on locale? */
					QStringList caps = progressLineMatch.capturedTexts();

					int itemBytes = caps[1].replace(',', "").toInt();
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

					//[[maybe_unused]] int hours = caps[5].toInt();
					//[[maybe_unused]] int mins = caps[6].toInt();
					//[[maybe_unused]] int secs = caps[7].toInt();

					Q_EMIT transferSpeed(xferSpeed);
					Q_EMIT itemProgressBytes(itemBytes);
					Q_EMIT itemProgress(itemPc);

					qDebug() << "line has" << caps.size() << "captures";

					if(10 <= caps.size()) {
						int remainingItems = caps[8].toInt();
						int totalItems = caps[9].toInt();
						int completedItems = totalItems - remainingItems;
						float overallPc = static_cast<float>((completedItems * 100.0) / totalItems);
						qDebug() << "overall progress:" << completedItems << "/" << totalItems << "=" << overallPc << "%";
						Q_EMIT overallProgress(static_cast<int>(overallPc));
					}
				}
				else if(QRegularExpressionMatch newItemMatch = newItemLine.match(data); newItemMatch.hasMatch()) {
					qDebug() << "line is start of new item";
					Q_EMIT newItemStarted(newItemMatch.captured(1));
				}
				else if(QRegularExpressionMatch myMatch = completedLine.match(data); myMatch.hasMatch()) {
					qDebug() << "line indicates overall completion";
					qDebug() << "overall transfer speed:" << myMatch.captured(3);
					Q_EMIT transferSpeed(myMatch.captured(3).replace(',', "").toFloat());
				}
			}

			if(lines.size() > 0) {
				m_outputCache = lines.at(lines.size() - 1);
			}
		}
	}


	void Process::processFinished(void) {
		Q_ASSERT(m_process);
		m_logFile.close();
		ExitCode code = static_cast<ExitCode>(m_process->exitCode());
		Q_EMIT finished(code);

		switch(code) {
			case Success:
			case PartialTransferError:
			case UnableToWriteLogFile:
				Q_EMIT finished(Process::defaultExitCodeMessage(code));
				break;

			case InterruptReceived:
				Q_EMIT interrupted(Process::defaultExitCodeMessage(code));
				break;

			case SyntaxError:
			case ProtocolIncompatibility:
			case FileSelectionError:
			case UnsupportedAction:
			case TransmissionProtocolStartupError:
			case SocketIoError:
			case FileIoError:
			case DataStreamError:
			case ProgramDiagnosticsError:
			case IpcCodeError:
			case WaitpidError:
			case MemoryAllocationError:
			case VanishedSourceFile:
			case MaximumDeletionsExceeded:
			case DataTransmissionTimeout:
			case ConnectionTimeout:
				Q_EMIT failed(Process::defaultExitCodeMessage(code));
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


	/**
	 * \fn Process::standardOutputUpdated(QString);
	 * \brief Emitted when more data has been written to \b rsync's
	 * standard output stream.
	 *
	 * \param data is the data written to standard output since this signal
	 * was last emitted, or since the process started if this is the first
	 * time the signal has been emitted.
	 */


}  // namespace Qync
