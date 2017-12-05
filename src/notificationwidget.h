/**
 * \file notificationwidget.h
 * \author Darren Edale
 * \date October, 2017
 * \version 1.1.0
 *
 * \brief Definition of the NotificationWidget class.
 *
 * \dep
 * - QWidget
 * - memory
 * - types.h
 */

#ifndef QYNC_NOTIFICATIONWIDGET_H
#define QYNC_NOTIFICATIONWIDGET_H

#include <QWidget>

#include <memory>

#include "types.h"

class QPropertyAnimation;

namespace Qync {

	namespace Ui {
		class NotificationWidget;
	}

	class NotificationWidget
	: public QWidget {
		Q_OBJECT

	public:
		explicit NotificationWidget(QWidget * parent = nullptr);
		~NotificationWidget();

		QString message() const;
		void setMessage(const QString & message);

		inline NotificationType type() const {
			return m_type;
		}

		void setType(NotificationType type);

	public Q_SLOTS:
		void show();
		void hide();

	private:
		std::unique_ptr<Ui::NotificationWidget> m_ui;
		std::unique_ptr<QPropertyAnimation> m_showAnim;
		std::unique_ptr<QPropertyAnimation> m_hideAnim;
		NotificationType m_type;
	};


}  // namespace Qync

#endif  // QYNC_NOTIFICATIONWIDGET_H
