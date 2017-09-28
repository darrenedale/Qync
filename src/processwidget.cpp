#include "processwidget.h"
#include "ui_processwidget.h"

#include <QMessageBox>

#include "application.h"
#include "process.h"
#include "units.h"


namespace Qync {


	ProcessWidget::ProcessWidget(QWidget * parent, Process * process)
	: QWidget(parent),
	  m_ui(new Ui::ProcessWidget),
	  m_process(nullptr) {
		m_ui->setupUi(this);

		if(process) {
			setProcess(process);
		}
	}


	void ProcessWidget::setProcess(Process * process) {
		if(m_process) {
			m_process->disconnect(this);
		}

		m_process.reset(process);

		connect(process, &Process::started, this, &ProcessWidget::onProcessStarted);
		connect(process, static_cast<void (Process::*)(QString)>(&Process::finished), this, &ProcessWidget::onProcessFinished);
		connect(process, &Process::interrupted, this, &ProcessWidget::onProcessInterrupted);
		connect(process, &Process::failed, this, &ProcessWidget::onProcessFailed);
		connect(process, &Process::itemProgress, this, &ProcessWidget::updateItemProgress);
		connect(process, &Process::transferSpeed, this, &ProcessWidget::updateTransferSpeed);
		connect(process, &Process::overallProgress, this, &ProcessWidget::updateOverallProgress);
		connect(process, &Process::newItemStarted, this, &ProcessWidget::updateItemInProgress);
		connect(process, &Process::error, this, &ProcessWidget::showError);
	}


	ProcessWidget::~ProcessWidget(void) = default;


	/**
	 * \brief Updates the item progress.
	 *
	 * \param pc is the percent progress for the current item.
	 *
	 * The current item progress bar is updated to the value provided.
	 */
	void ProcessWidget::updateItemProgress(int pc) {
		m_ui->itemProgress->setValue(pc);
	}


	/**
	 * \brief Changes the current item in progress.
	 *
	 * \param item is the path of the new current item.
	 *
	 * The current item line edit is updated to show the new item path.
	 */
	void ProcessWidget::updateItemInProgress(const QString & item) {
		m_ui->itemName->setText(item);
	}


	/**
	 * \brief Updates the overall progress.
	 *
	 * \param pc is the percent overall progress.
	 *
	 * The overall progress bar is updated to the value provided.
	 */
	void ProcessWidget::updateOverallProgress(int pc) {
		m_ui->overallProgress->setValue(pc);
	}


	/**
	 * \brief Updates the display of the transfer speed.
	 *
	 * @param speed The transfer speed to display, in bytes per second.
	 *
	 * Currently the speed is always displayed as bytes per second. It is likely
	 * to change to adapt the display unit based on the size of the transfer
	 * speed.
	 */
	void ProcessWidget::updateTransferSpeed(float speed) {
		QString unit = "B/s";

		if(static_cast<long double>(speed) > 2.0_gib) {
			speed /= 1.0_gib;
			unit = "GiB/s";
		}
		else if(static_cast<long double>(speed) > 2.0_mib) {
			speed /= 1.0_mib;
			unit = "MiB/s";
		}
		else if(static_cast<long double>(speed) > 2.0_kib) {
			speed /= 1.0_kib;
			unit = "KiB/s";
		}

		m_ui->transferSpeed->setText(QLocale().toString(static_cast<double>(speed), 'f', 2) + " " + unit);
	}


	/**
	 * \brief Indicate to the dialogue that the process has started.
	 *
	 * The progress widgets are reset and the stop button is enabled.
	 */
	void ProcessWidget::onProcessStarted(void) {
		m_ui->itemName->setText({});
		m_ui->transferSpeed->setText({});
		m_ui->itemProgress->setValue(0);
		m_ui->overallProgress->setValue(0);
	}


	/**
	 * \brief Indicate to the dialogue that the process has successfully
	 * completed.
	 *
	 * The progress widgets are maxed out, and the stop button is disabled.
	 * and the save button is enabled.
	 */
	void ProcessWidget::onProcessFinished(const QString & msg) {
		m_ui->itemProgress->setValue(100);
		m_ui->overallProgress->setValue(100);
		m_ui->itemName->setText(QString("<strong>%1</strong>").arg(tr("Synchronisation complete")));

		/* transfer speed will be set to the overall speed as emitted by rsync
		 * process in its stdout, so we don't clear it */

		if(!msg.isEmpty()) {
			QMessageBox::information(this, tr("%1 Message").arg(qyncApp->applicationDisplayName()), msg, QMessageBox::Ok);
		}

		m_process.reset(nullptr);
	}


	/**
	 * \brief Indicate to the dialogue that the process was interrupted.
	 *
	 * The progress widgets are maxed out, and the stop button is disabled.
	 * and the save button is enabled.
	 */
	void ProcessWidget::onProcessInterrupted(const QString & msg) {
		m_ui->itemProgress->setValue(0);
		m_ui->overallProgress->setValue(0);
		m_ui->itemName->setText({});
		m_ui->transferSpeed->setText({});
		QMessageBox::critical(this, tr("%1 Error").arg(qyncApp->applicationDisplayName()), (msg.isEmpty() ? tr("The process was interrupted.") : msg), QMessageBox::Ok);
		m_process.reset(nullptr);
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
	void ProcessWidget::onProcessFailed(const QString & msg) {
		m_ui->itemProgress->setValue(0);
		m_ui->overallProgress->setValue(0);
		m_ui->itemName->setText({});
		m_ui->transferSpeed->setText({});
		QMessageBox::critical(this, tr("%1 Error").arg(qyncApp->applicationDisplayName()), (msg.isEmpty() ? tr("The process failed.") : msg), QMessageBox::Ok);
		m_process.reset(nullptr);
	}


	/**
	 * \brief Show an error message.
	 *
	 * \param err is the error message to display.
	 *
	 * The error message is shown in a warning dialogue.
	 */
	void ProcessWidget::showError(const QString & err) {
		QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("The following error occurred in rsync:\n\n%1").arg(err), QMessageBox::Ok);
	}


}  // namespace Qync
