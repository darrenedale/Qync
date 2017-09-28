/**
 * \file processdialogue.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the ProcessDialogue class.
 *
 * \dep
 * - processdialogue.h
 * - processdialogue.ui
 * - QDebug
 * - QString
 * - QStringList
 * - QCloseEvent
 * - QFileDialog
 * - QMessageBox
 * - application.h
 * - process.h
 * - functions.h
 */

#include "processdialogue.h"
#include "ui_processdialogue.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "application.h"
#include "process.h"
#include "functions.h"
#include "units.h"


namespace Qync {


	/**
	 * \class ProcessDialogue
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
	 *
	 * \brief The window to interact with a spawned rsync process.
	 *
	 * The process dialogue provides an interface for the end user to monitor and
	 * interact with a running rsync process. It provides the user with the ability
	 * to interrupt a running process and save its output, as well as to see how the
	 * command is progressing. Progress is reported by showing a full list of the
	 * files and directories as they are being processed, and by showing which item
	 * is currently being processed, how close to completion it is, and how close to
	 * completion the whole process is.
	 */


	/**
	 * \brief Create a new ProcessDialogue
	 *
	 * \param process is the process to monitor.
	 * \param parent is the parent widget.
	 *
	 * The provided process is owned and will be deleted when the dialogue is
	 * destroyed.
	 */
	ProcessDialogue::ProcessDialogue(Process * process, QWidget * parent)
	: QDialog(parent),
	  m_ui{new Ui::ProcessDialogue},
	  m_saveButton{nullptr},
	  m_abortButton{nullptr} {
		Q_ASSERT_X(process, __PRETTY_FUNCTION__, "No process provided");
		m_ui->setupUi(this);
		m_ui->processWidget->setProcess(process);

		/* keep refs to these from the UI because we dis/enable them at various points */
		m_saveButton = m_ui->controls->button(QDialogButtonBox::Save);
		m_abortButton = m_ui->controls->button(QDialogButtonBox::Abort);

		connect(process, &Process::started, this, &ProcessDialogue::onProcessStarted);
		connect(process, static_cast<void (Process::*)(QString)>(&Process::finished), this, &ProcessDialogue::onProcessFinished);
		connect(process, &Process::interrupted, this, &ProcessDialogue::onProcessInterrupted);
		connect(process, &Process::failed, this, &ProcessDialogue::onProcessFailed);
		connect(process, &Process::newItemStarted, this, &ProcessDialogue::appendToDetails);
		connect(m_ui->detailsButton, &QPushButton::clicked, this, &ProcessDialogue::toggleDetailedText);
	}


	/**
	 * \brief Destroy the ProcessDialogue
	 */
	ProcessDialogue::~ProcessDialogue(void) {
		m_ui->detailsButton->disconnect(this);
		m_saveButton = nullptr;
		m_abortButton = nullptr;
	}


	/**
	 * \brief Handle a request to close the window.
	 *
	 * \param e is the event details.
	 *
	 * The dialogue is closed as normal and is scheduled for deletion.
	 */
	void ProcessDialogue::closeEvent(QCloseEvent * e) {
		QDialog::closeEvent(e);
		e->accept();
		this->deleteLater();
	}


	/**
	 * \brief Toggle the state of the detailed info text.
	 *
	 * If it is currently visible the detailed info text will be hidden; if
	 * it is currently hiddent it will be made visible.
	 */
	void ProcessDialogue::toggleDetailedText(void) {
		if(m_ui->details->isVisible()) {
			hideDetailedText();
		}
		else {
			showDetailedText();
		}
	}


	/**
	 * \brief Show the detailed info text.
	 *
	 * The detailed info text widget will be made visible.
	 */
	void ProcessDialogue::showDetailedText(void) {
		m_ui->details->show();
		m_ui->detailsButton->setText(tr("Hide details"));
		m_ui->detailsButton->setIcon(QIcon::fromTheme("arrow-up", QIcon(":icons/buttons/toggle_up")));
		adjustSize();
	}


	/**
	 * \brief Hide the detailed info text.
	 *
	 * The detailed info text widget will be hidden.
	 */
	void ProcessDialogue::hideDetailedText(void) {
		m_ui->details->hide();
		m_ui->detailsButton->setText(tr("Show details"));
		m_ui->detailsButton->setIcon(QIcon::fromTheme("arrow-down", QIcon(":icons/buttons/toggle_down")));
		adjustSize();
	}


	/**
	 * \brief Append text to the details text widget.
	 *
	 * \param txt The text to append.
	 */
	void ProcessDialogue::appendToDetails(const QString & txt) {
		m_ui->details->appendPlainText(txt);
	}


	/**
	 * \brief Save the current content of the output widget.
	 *
	 * A file dialogue is presented to the user, and if s/he does not cancel
	 * the dialogue, the file chosen in overwritten with the content of
	 * the output text edit.
	 */
	void ProcessDialogue::saveOutput(void) {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save %1 output").arg(qyncApp->applicationDisplayName()));

		if(fileName.isEmpty()) {
			return;
		}

		QFile f(fileName);

		if(!f.open(QIODevice::WriteOnly)) {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The output could not be saved to %1.").arg(fileName));
			return;
		}

		f.write(m_ui->details->toPlainText().toUtf8());
		f.close();
	}


	/**
	 * \brief Indicate to the dialogue that the process has started.
	 *
	 * The progress widgets are reset and the stop button is enabled.
	 */
	void ProcessDialogue::onProcessStarted(void) {
		m_abortButton->setEnabled(true);
		m_saveButton->setEnabled(false);
	}


	/**
	 * \brief Indicate to the dialogue that the process has successfully
	 * completed.
	 *
	 * The progress widgets are maxed out, and the stop button is disabled.
	 * and the save button is enabled.
	 */
	void ProcessDialogue::onProcessFinished(const QString &) {
		m_abortButton->setEnabled(false);
		m_saveButton->setEnabled(true);
	}


	/**
	 * \brief Indicate to the dialogue that the process was interrupted.
	 *
	 * The progress widgets are maxed out, and the stop button is disabled.
	 * and the save button is enabled.
	 */
	void ProcessDialogue::onProcessInterrupted(const QString &) {
		m_abortButton->setEnabled(false);
		m_saveButton->setEnabled(true);
	}


	/**
	 * \brief Indicate to the dialogue that the process has unsuccessfully
	 * completed.
	 *
	 * \param msg is the error message to display.
	 *
	 * The progress widgets are maxed out, and the stop button is disabled.
	 * and the save button is enabled.
	 */
	void ProcessDialogue::onProcessFailed(const QString &) {
		m_abortButton->setEnabled(false);
		m_saveButton->setEnabled(true);
	}


}  // namespace Qync
