/**
 * @file processdialogue.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the ProcessDialogue class.
 *
 * @dep
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
		explicit ProcessDialogue(const std::shared_ptr<Process> &, QWidget * = nullptr);
		~ProcessDialogue() override;

	public Q_SLOTS:
		void toggleDetailedText();
		void showDetailedText();
		void hideDetailedText();

	private Q_SLOTS:
		void appendToDetails(const QString &);
		void saveOutput();

		void onProcessStarted();
		void onProcessFinished(const QString & = QStringLiteral());
		void onProcessInterrupted(const QString & = QStringLiteral());
		void onProcessFailed(const QString & = QStringLiteral());

	protected:
		void closeEvent(QCloseEvent *) override;

	private:
		std::unique_ptr<Ui::ProcessDialogue> m_ui;

		/* pointers to these are kept for convenience. they are valid for as long
		 * as m_ui is valid (i.e. the lifetime of the object) */
		QPushButton * m_saveButton;
		QPushButton * m_abortButton;
	};

}  // namespace Qync

#endif  // QYNC_PROCESSDIALOGUE_H
