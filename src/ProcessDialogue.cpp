/**
 * \file ProcessDialogue.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the ProcessDialogue class.
 *
 * \dep
 * - ProcessDialogue.h
 * - Process.h
 * - common.h
 * - QDebug
 * - QString
 * - QStringList
 * - QLabel
 * - QProgressBar
 * - QTextEdit
 * - QDialogButtonBox
  * - QPushButton
 * - QVBoxLayout
 * - QProcess
 * - QCloseEvent
 * - QFileDialog
 * - QMessageBox
 *
 * \todo
 * Nothing.
 */

#include "ProcessDialogue.h"

#include "Process.h"
#include "common.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>


namespace Qync {


	ProcessDialogue::ProcessDialogue(Process * process, QWidget * parent)
	  : QDialog(parent),
		 m_process(process) {
		qDebug() << "ProcessDialogue::ProcessDialogue()";
		setMinimumWidth(400);
		createWidgets();
		connect(m_process, SIGNAL(started()), this, SLOT(processStarted()));
		connect(m_process, SIGNAL(finished(QString)), this, SLOT(processFinished(QString)));
		connect(m_process, SIGNAL(interrupted(QString)), this, SLOT(processInterrupted(QString)));
		connect(m_process, SIGNAL(failed(QString)), this, SLOT(processFailed(QString)));
		connect(m_process, SIGNAL(itemProgress(int)), this, SLOT(updateItemProgress(int)));
		connect(m_process, SIGNAL(overallProgress(int)), this, SLOT(updateOverallProgress(int)));
		connect(m_process, SIGNAL(newItemStarted(QString)), this, SLOT(updateItemInProgress(QString)));
		connect(m_process, SIGNAL(error(QString)), this, SLOT(showError(QString)));
		connect(m_toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetailedText()));
		qDebug() << "done";
	}


	ProcessDialogue::~ProcessDialogue(void) {
		qDebug() << "deleting rsync process";
		if(m_process) {
			m_process->disconnect(this);
			m_toggleDetailsButton->disconnect(this);
			delete m_process;
		}

		m_process = nullptr;
	}


	void ProcessDialogue::createWidgets(void) {
		m_itemName = new QLabel();
		m_itemProgress = new QProgressBar();
		m_overallProgress = new QProgressBar();
		m_output = new QTextEdit();
		m_output->setReadOnly(true);
		m_output->append(m_process->command() + " " + m_process->arguments().join(" "));

		m_toggleDetailsButton = new QPushButton(QIcon(":/icons/buttons/toggle_up"), tr("Hide details"));

		m_buttons = new QDialogButtonBox();
		m_stopButton = new QPushButton(QIcon(":/icons/buttons/stop"), tr("Stop"));
		m_stopButton->setToolTip(tr("Stop the rsync command."));
		connect(m_stopButton, SIGNAL(clicked()), m_process, SLOT(stop()));

		m_saveButton = new QPushButton(QIcon(":/icons/buttons/save"), tr("Save"));
		m_saveButton->setToolTip(tr("Save the output."));
		connect(m_saveButton, SIGNAL(clicked()), this, SLOT(saveOutput()));

		m_buttons->addButton(m_stopButton, QDialogButtonBox::ActionRole);
		m_buttons->addButton(m_saveButton, QDialogButtonBox::ActionRole);
		QPushButton * b = new QPushButton(QIcon(":/icons/buttons/close"), tr("Close"));
		b->setToolTip(tr("Close this window (this will cause the rsycn process to stop if it is currently still running)."));
		connect(b, SIGNAL(clicked()), this, SLOT(close()));
		m_buttons->addButton(b, QDialogButtonBox::RejectRole);

		QVBoxLayout * vLayout = new QVBoxLayout();
		vLayout->addWidget(m_itemName);
		vLayout->addWidget(m_itemProgress);
		vLayout->addWidget(m_overallProgress);

		QFrame * line = new QFrame();
		line->setFrameShape(QFrame::HLine);
		vLayout->addWidget(line);

		QHBoxLayout * toggleDetailsLayout = new QHBoxLayout();
		toggleDetailsLayout->addWidget(m_toggleDetailsButton, 1);
		toggleDetailsLayout->addStretch(3);
		vLayout->addLayout(toggleDetailsLayout);

#if defined(QYNC_DEBUG)
		m_stdout = new QTextEdit();
		vLayout->addWidget(m_stdout);
		connect(m_process, SIGNAL(standardOutputUpdated(QString)), this, SLOT(updateStdout(QString)));
#endif

		vLayout->addWidget(m_output);
		vLayout->addWidget(m_buttons);

		setLayout(vLayout);
	}


	void ProcessDialogue::closeEvent(QCloseEvent * e) {
		QDialog::closeEvent(e);
		e->accept();
		this->deleteLater();
	}


	void ProcessDialogue::toggleDetailedText(void) {
		if(m_output->isVisible()) {
			hideDetailedText();
		}
		else {
			showDetailedText();
		}
	}

	void ProcessDialogue::showDetailedText(void) {
		m_output->show();
		m_toggleDetailsButton->setText(tr("Hide details"));
		m_toggleDetailsButton->setIcon(QIcon(":icons/buttons/toggle_up"));
		adjustSize();
	}

	void ProcessDialogue::hideDetailedText(void) {
		m_output->hide();
		m_toggleDetailsButton->setText(tr("Show details"));
		m_toggleDetailsButton->setIcon(QIcon(":icons/buttons/toggle_down"));
		adjustSize();
	}

	void ProcessDialogue::updateItemProgress(int pc) {
		m_itemProgress->setValue(pc);
	}


	void ProcessDialogue::updateItemInProgress(const QString & item) {
		m_itemName->setText(item);
		m_output->append(item);
	}


	void ProcessDialogue::updateOverallProgress(int pc) {
		m_overallProgress->setValue(pc);
	}


	void ProcessDialogue::saveOutput(void) {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save %1 output").arg(QYNC_APP_NAME));

		if(!fileName.isEmpty()) {
			QFile f(fileName);
			f.open(QIODevice::WriteOnly);

			if(!f.isOpen()) {
				QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The output could not be saved to %1.").arg(fileName));
				return;
			}

			f.write(m_output->toPlainText().toUtf8());
			f.close();
		}
	}


	void ProcessDialogue::processStarted(void) {
		m_itemName->setText("");
		m_itemProgress->setValue(0);
		m_overallProgress->setValue(0);
		m_stopButton->setEnabled(true);
		m_saveButton->setEnabled(false);
	}


	void ProcessDialogue::processFinished(const QString & msg) {
		m_stopButton->setEnabled(false);
		m_saveButton->setEnabled(true);
		m_itemProgress->setValue(100);
		m_overallProgress->setValue(100);
		m_itemName->setText("");
		QMessageBox::information(this, tr("%1 Message").arg(QYNC_APP_NAME), (msg.isEmpty() ? tr("The process completed successfully.") : msg), QMessageBox::Ok);
	}


	void ProcessDialogue::processInterrupted(const QString & msg) {
		m_stopButton->setEnabled(false);
		m_saveButton->setEnabled(true);
		m_itemProgress->setValue(0);
		m_overallProgress->setValue(0);
		m_itemName->setText("");
		QMessageBox::critical(this, tr("%1 Error").arg(QYNC_APP_NAME), (msg.isEmpty() ? tr("The process was interrupted.") : msg), QMessageBox::Ok);
	}


	void ProcessDialogue::processFailed(const QString & msg) {
		m_stopButton->setEnabled(false);
		m_saveButton->setEnabled(true);
		m_itemProgress->setValue(0);
		m_overallProgress->setValue(0);
		m_itemName->setText("");
		QMessageBox::critical(this, tr("%1 Error").arg(QYNC_APP_NAME), (msg.isEmpty() ? tr("The process failed.") : msg), QMessageBox::Ok);
	}


	void ProcessDialogue::updateStdout(const QString & s) {
#if defined(QYNC_DEBUG)
		if(m_stdout)
			m_stdout->append(s);
#endif
		(void) s;
	}


	void ProcessDialogue::showError(const QString & err) {
		QMessageBox::warning(this, tr("%1 Warning").arg(QYNC_APP_NAME), tr("The following error occurred in rsync:\n\n%1").arg(err), QMessageBox::Ok);
	}


}  // namespace Qync
