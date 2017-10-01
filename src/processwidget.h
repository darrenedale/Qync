#ifndef QYNC_PROCESSWIDGET_H
#define QYNC_PROCESSWIDGET_H

#include <memory>

#include <QWidget>

namespace Qync {

	namespace Ui {
		class ProcessWidget;
	}

	class Process;

	class ProcessWidget
	: public QWidget {
		Q_OBJECT

	public:
		explicit ProcessWidget(QWidget * parent = nullptr, const std::shared_ptr<Process> & process = nullptr);
		~ProcessWidget(void);

		void setProcess(const std::shared_ptr<Process> & process);

	private Q_SLOTS:
		void updateItemProgress(int pc);
		void onNewItemStarted(const QString & item);
		void updateOverallProgress(int pc);
		void updateTransferSpeed(float bytesPerSecond);

		void onProcessStarted(void);
		void onProcessFinished(const QString & msg = {});
		void onProcessInterrupted(const QString & msg = {});
		void onProcessFailed(const QString & msg = {});

		void showError(const QString & err);

	private:
		std::unique_ptr<Ui::ProcessWidget> m_ui;
		std::shared_ptr<Process> m_process;
	};


}  // namespace Qync
#endif  // QYNC_PROCESSWIDGET_H
