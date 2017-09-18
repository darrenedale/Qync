/**
 * \file Process.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the Process class.
 *
 * \dep
 * - Process.h
 * - QDebug
 * - QProcess
 * - QFile
 * - QRegExp
 *
 * \todo
 * - Consider whether to re-introduce parsing of stderr.
 */

#include "Process.h"
#include "Preset.h"

#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QRegExp>


namespace Qync {


	Process::Process(const QString & cmd, const QStringList & args, const QString & logFile)
	  : QObject(),
		 m_process(nullptr),
		 m_command(cmd),
		 m_args(args),
		 m_logFile(nullptr) {
		m_process = new QProcess();
		m_logFileName = logFile;
	}


	Process::Process(const QString & cmd, const Preset * preset)
	  : Process(cmd, QStringList(), QString()) {
		if(preset) {
			m_logFileName = preset->logFile();
			m_args = Process::rsyncArguments(preset);
		}
	}


	Process::~Process(void) {
		delete m_process;
		m_process = nullptr;
		disposeLogFile();
	}


	QStringList Process::rsyncArguments(const Preset * preset, const QStringList & forceOptions) {
		if(preset->source().isEmpty()) {
			qDebug() << "source is empty";
			return QStringList();
		}

		if(preset->destination().isEmpty()) {
			qDebug() << "destination is empty";
			return QStringList();
		}

		QStringList args(forceOptions);
		args << "--recursive"
			  << "--progress"
			  << "--verbose";

		/* basic settings */
		if(preset->preserveTime())
			args << "--times";
		if(preset->preservePermissions())
			args << "--perms";
		if(preset->preserveOwner())
			args << "--owner";
		if(preset->preserveGroup())
			args << "--group";

		if(preset->windowsCompatability())
			args << "--modify-window=1";
		if(preset->honourDeletions())
			args << "--delete";

		/* advanced settings */
		if(preset->alwaysCompareChecksums())
			args << "--checksum";
		if(preset->preserveDevices())
			args << "--devices";
		if(preset->keepPartialTransfers())
			args << "--partial";
		if(preset->copySymlinksAsSymlinks())
			args << "--links";
		if(preset->makeBackups())
			args << "--backup";

		if(preset->useTransferCompression())
			args << "--compress";
		if(preset->onlyUpdateExistingEntries())
			args << "--existing";
		if(preset->dontUpdateExistingEntries())
			args << "--ignore-existing";
		if(preset->dontMapUsersAndGroups())
			args << "--numeric-ids";
		if(preset->copyHardlinksAsHardlinks())
			args << "--hard-links";
		if(preset->showItemisedChanges())
			args << "--itemize-changes";

		/* source and dest */
		args << preset->source();
		args << preset->destination();

		return args;
	}


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


	QProcess * Process::process(void) {
		return m_process;
	}


	QString Process::command(void) const {
		return m_command;
	}


	QStringList Process::arguments(void) const {
		return m_args;
	}


	QString Process::logFile(void) const {
		return m_logFileName;
	}


	void Process::start(void) {
		Q_ASSERT(m_process);
		m_process->start(m_command, m_args);

		if(!m_logFileName.isEmpty()) {
			m_logFile = new QFile(m_logFileName);
			m_logFile->open(QIODevice::WriteOnly);

			if(!m_logFile->isOpen())
				disposeLogFile();
		}

		m_stdoutConnection = connect(m_process, &QProcess::readyReadStandardOutput, this, &Process::parseStdout);
		connect(m_process, &QProcess::readyReadStandardError, this, &Process::parseStderr);
		connect(m_process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &Process::processFinished);
		Q_EMIT started();
	}


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
		Q_ASSERT(m_process);
		//if(m_process->state() == QProcess::NotRunning) return;

		QString data = m_process->readAllStandardOutput();

		if(m_logFile && m_logFile->isOpen()) {
			m_logFile->write(data.toUtf8());
		}

		if(!data.isEmpty()) {
			Q_EMIT standardOutputUpdated(data);
			m_outputCache.append(data);

			/* this parses the output of rsync for lines indicating progress */
			m_outputCache = m_outputCache.replace('\r', '\n');

			QStringList lines(m_outputCache.split("\n"));
			/* don't need it, but would be nice to capture duration:
		   (\\d:\\d{2,2}:\\d{2,2}) doesn't seem to do it */
			//QRegExp progressLine(" *(\\d+) *(\\d+)% *(\\d:\\d{2,2}:\\d{2,2}) *\\(.*, to\\-check=(\\d+)/(\\d+)\\)");
			QRegExp progressLine(" *(\\d+) *(\\d+)%.*\\(.*, to\\-check=(\\d+)/(\\d+)\\)");

			for(int i = 0; i < lines.size() - 1; i++) {
				data = lines.at(i);

				if(data.isEmpty()) {
					continue;
				}

				if(progressLine.indexIn(data) != -1) {
					qDebug() << "item progress:" << progressLine.cap(1).toInt() << "bytes (" << progressLine.cap(2) << "%)";
					qDebug() << "overall progress: 100 - " << progressLine.cap(3).toInt() << "* 100 /" << progressLine.cap(4).toInt() << "=" << (100 - progressLine.cap(3).toInt() * 100 / progressLine.cap(4).toInt());

					Q_EMIT itemProgressBytes(progressLine.cap(1).toInt());
					Q_EMIT itemProgress(progressLine.cap(2).toInt());
					Q_EMIT overallProgress(100 - progressLine.cap(3).toInt() * 100 / progressLine.cap(4).toInt());
				}

				/*			if(data.startsWith(' ')) {
				QStringList progress = data.trimmed().split(" ", QString::SkipEmptyParts);

				//qDebug() << "line items:" << progress;

				if(progress.size() > 1) {
					Q_EMIT itemProgressBytes(progress.at(0).toInt()));
					Q_EMIT itemProgress(progress.at(1).left(progress.at(1).size() - 1).toInt()));
				}
			}*/
				else if(data.size() > 1 && 'f' == data.at(1)) {
					Q_EMIT newItemStarted(data.right(data.size() - 12));
				}
			}

			if(lines.size() > 0) {
				m_outputCache = lines.at(lines.size() - 1);
			}
		}
	}


	void Process::disposeLogFile(void) {
		if(m_logFile) {
			m_logFile->close();
			delete m_logFile;
			m_logFile = nullptr;
		}
	}


	void Process::processFinished(void) {
		Q_ASSERT(m_process);

		disposeLogFile();
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


}  // namespace Qync
