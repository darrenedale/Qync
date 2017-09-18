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

#include "common.h"

#include <QDialog>
#include <QString>
#include <QTextEdit>

class QCloseEvent;
class QLabel;
class QProgressBar;
class QProcess;
class QDialogButtonBox;
class QPushButton;

namespace Qync {

	class Process;

	/**
 * \class ProcessDialogue
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
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
	class ProcessDialogue
	  : public QDialog {
		Q_OBJECT

	private:
		Process * m_process;
		QString m_outputCache;

		QLabel * m_itemName;
		QProgressBar * m_itemProgress;
		QProgressBar * m_overallProgress;
		QPushButton * m_toggleDetailsButton;
		QTextEdit * m_output;
		QDialogButtonBox * m_buttons;
		QPushButton * m_stopButton;
		QPushButton * m_saveButton;

		void createWidgets(void);

#if defined(QYNC_DEBUG)
		QTextEdit * m_stdout;
#endif

	private Q_SLOTS:
		/**
		 * \brief Update the captured standard output stream with some text.
		 *
		 * \param s is the text to append to the standard output cache.
		 *
		 * The standard output cache has the string provided appended to it.
		 *
		 * \note This slot is only available for debugging purposes. It does not
		 * do anything in release code.
		 */
		void updateStdout(const QString & s);

	protected:
		/**
		 * \brief Handle a request to close the window.
		 *
		 * \param e is the event details.
		 *
		 * The dialogue is closed as normal and is scheduled for deletion.
		 */
		virtual void closeEvent(QCloseEvent * e) override;

	public:
		/**
		 * \brief Create a new ProcessDialogue
		 *
		 * \param process is the process to monitor.
		 * \param parent is the parent widget.
		 */
		explicit ProcessDialogue(Process * process, QWidget * parent = nullptr);

		/**
		 * \brief Destroy the ProcessDialogue
		 */
		virtual ~ProcessDialogue(void);

	signals:

	private Q_SLOTS:
		/**
		 * \brief Updates the item progress.
		 *
		 * \param pc is the percent progress for the current item.
		 *
		 * The current item progress bar is updated to the value provided.
		 */
		void updateItemProgress(int pc);

		/**
		 * \brief Changes the current item in progress.
		 *
		 * \param item is the path of the new current item.
		 *
		 * The current item line edit is updated to show the new item path.
		 */
		void updateItemInProgress(const QString & item);

		/**
		 * \brief Updates the overall progress.
		 *
		 * \param pc is the percent overall progress.
		 *
		 * The overall progress bar is updated to the value provided.
		 */
		void updateOverallProgress(int pc);

		/**
		 * \brief Save the current content of the output widget.
		 *
		 * A file dialogue is presented to the user, and if s/he does not cancel
		 * the dialogue, the file chosen in overwritten with the content of
		 * the output text edit.
		 */
		void saveOutput(void);

		/**
		 * \brief Indicate to the dialogue that the process has started.
		 *
		 * The progress widgets are reset and the stop button is enabled.
		 */
		void processStarted(void);

		/**
		 * \brief Indicate to the dialogue that the process has successfully
		 * completed.
		 *
		 * The progress widgets are maxed out, and the stop button is disabled.
		 * and the save button is enabled.
		 */
		void processFinished(const QString & msg = {});

		/**
		 * \brief Indicate to the dialogue that the process was interrupted.
		 *
		 * The progress widgets are maxed out, and the stop button is disabled.
		 * and the save button is enabled.
		 */
		void processInterrupted(const QString & msg = {});

		/**
		 * \brief Indicate to the dialogue that the process has unsuccessfully
		 * completed.
		 *
		 * \param msg is the error message to display.
		 *
		 * The progress widgets are maxed out, and the stop button is disabled.
		 * and the save button is enabled.
		 */
		void processFailed(const QString & msg = {});

		/**
		 * \brief Show an error message.
		 *
		 * \param err is the error message to display.
		 *
		 * The error message is shown in a warning dialogue.
		 */
		void showError(const QString & err);

	public Q_SLOTS:
		/**
         * \brief Toggle the state of the detailed info text.
         *
         * If it is currently visible the detailed info text will be hidden; if
         * it is currently hiddent it will be made visible.
         */
		void toggleDetailedText(void);

		/**
         * \brief Show the detailed info text.
         *
         * The detailed info text widget will be made visible.
         */
		void showDetailedText(void);

		/**
         * \brief Hide the detailed info text.
         *
         * The detailed info text widget will be hidden.
         */
		void hideDetailedText(void);
	};

}  // namespace Qync

#endif  // QYNC_PROCESSDIALOGUE_H
