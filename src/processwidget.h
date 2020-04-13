/**
 * @file processwidget.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the ProcessWidget class.
 */

#ifndef QYNC_PROCESSWIDGET_H
#define QYNC_PROCESSWIDGET_H

#include <memory>

#include <QtWidgets/QWidget>

namespace Qync {

	namespace Ui {
		class ProcessWidget;
	}

	class Process;

	class ProcessWidget
	:   public QWidget {
		Q_OBJECT

	public:
		explicit ProcessWidget(QWidget * = nullptr, const std::shared_ptr<Process> & = nullptr);
		~ProcessWidget() override;

		void setProcess(const std::shared_ptr<Process> &);

	private Q_SLOTS:
		void updateItemProgress(int);
		void onNewItemStarted(const QString &);
		void updateOverallProgress(int);
		void updateTransferSpeed(float);

		void onProcessStarted();
		void onProcessFinished(const QString & = QStringLiteral());
		void onProcessInterrupted(const QString & = QStringLiteral());
		void onProcessFailed(const QString & = QStringLiteral());

	private:
		std::unique_ptr<Ui::ProcessWidget> m_ui;
		std::shared_ptr<Process> m_process;
	};

}  // namespace Qync

#endif  // QYNC_PROCESSWIDGET_H
