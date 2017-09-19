/**
 * \file QyncProcessDialogue.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the QyncProcessDialogue class.
 *
 * \dep
 * - common.h
 * - QDialog
 * - QString
 * - QTextEdit
 *
 * \todo
 * Nothing.
 */
#ifndef QYNC_PROCESSDIALOGUE_H
#define QYNC_PROCESSDIALOGUE_H

#include <memory>

#include <QDialog>
#include <QString>
#include <QTextEdit>

#include "functions.h"
#include "ui_processdialogue.h"

class QCloseEvent;
class QLabel;
class QProgressBar;
class QProcess;
class QDialogButtonBox;
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

		void saveOutput(void);

		void onProcessStarted(void);
		void onProcessFinished(const QString & msg = {});
		void onProcessInterrupted(const QString & msg = {});
		void onProcessFailed(const QString & msg = {});

		void showError(const QString & err);

	protected:
		virtual void closeEvent(QCloseEvent *) override;

	private:
		Process * m_process;
		QString m_outputCache;

		std::unique_ptr<Ui::ProcessDialogue> m_ui;
	};

}  // namespace Qync

#endif  // QYNC_PROCESSDIALOGUE_H
