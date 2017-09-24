#ifndef QYNC_SOURCEDESTINATIONWIDGET_H
#define QYNC_SOURCEDESTINATIONWIDGET_H

#include <memory>

#include <QWidget>

namespace Qync {
	namespace Ui {
		class SourceDestinationWidget;
	}

	class SourceDestinationWidget
	: public QWidget {
		Q_OBJECT

	public:
		explicit SourceDestinationWidget(QWidget * parent = nullptr);
		~SourceDestinationWidget(void);

		QString source(void) const;
		QString destination(void) const;

	Q_SIGNALS:
		void sourceChanged(const QString &);
		void destinationChanged(const QString &);

	public Q_SLOTS:
		void setSource(const QString & src);
		void setDestination(const QString & dest);

		void swapSourceAndDestination(void);

	private Q_SLOTS:
		void chooseSourceDirectory(void);
		void chooseDestinationDirectory(void);

	private:
		std::unique_ptr<Ui::SourceDestinationWidget> m_ui;
	};


}  // namespace Qync
#endif  // QYNC_SOURCEDESTINATIONWIDGET_H
