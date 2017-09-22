/**
 * \file processdialogue.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Declaration of the ProcessDialogue class.
 *
 * \dep
 * - memory
 * - QDialog
 * - QString
 * - functions.h
 */
#ifndef QYNC_PROCESSDIALOGUE_H
#define QYNC_PROCESSDIALOGUE_H

#include <memory>

#include <QDialog>
#include <QString>

#include "functions.h"

class QCloseEvent;
class QPushButton;

namespace Qync {

	namespace Ui {
		class ProcessDialogue;
	}

	class Process;

	class ProcessDialogue
	: public QDialog {
		Q_OBJECT

	public:
		explicit ProcessDialogue(Process * process, QWidget * parent = nullptr);
		virtual ~ProcessDialogue(void);

	public Q_SLOTS:
		void toggleDetailedText(void);
		void showDetailedText(void);
		void hideDetailedText(void);

	private Q_SLOTS:
		void updateItemProgress(int pc);
		void updateItemInProgress(const QString & item);
		void updateOverallProgress(int pc);
		void updateTransferSpeed(float bytesPerSecond);

		void saveOutput(void);

		void onProcessStarted(void);
		void onProcessFinished(const QString & msg = {});
		void onProcessInterrupted(const QString & msg = {});
		void onProcessFailed(const QString & msg = {});

		void showError(const QString & err);

	protected:
		virtual void closeEvent(QCloseEvent *) override;

	private:
		std::unique_ptr<Process> m_process;
		QString m_outputCache;

		std::unique_ptr<Ui::ProcessDialogue> m_ui;

		/* pointers to these are kept for convenience. they are valid for as long
		 * as m_ui is valid (i.e. the lifetime of the object) */
		QPushButton * m_saveButton;
		QPushButton * m_abortButton;
	};

}  // namespace Qync

#endif  // QYNC_PROCESSDIALOGUE_H
