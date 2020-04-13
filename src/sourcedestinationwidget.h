/**
 * \file sourcedestinationwidget.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the SourceDestinationWidget class.
 *
 * \dep
 * - memory
 * - QWidget
 */

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
		explicit SourceDestinationWidget(QWidget * = nullptr);
		~SourceDestinationWidget();

		QString source() const;
		QString destination() const;

		void setSourceLabel(const QString &);
		void setDestinationLabel(const QString &);

	Q_SIGNALS:
		void sourceChanged(const QString &);
		void destinationChanged(const QString &);

	public Q_SLOTS:
		void setSource(const QString &);
		void setDestination(const QString &);

		void swapSourceAndDestination();

	private Q_SLOTS:
		void chooseSourceDirectory();
		void chooseDestinationDirectory();

	private:
		std::unique_ptr<Ui::SourceDestinationWidget> m_ui;
	};


}  // namespace Qync
#endif  // QYNC_SOURCEDESTINATIONWIDGET_H
