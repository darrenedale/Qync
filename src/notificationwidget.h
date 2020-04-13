/**
 * @file notificationwidget.h
 * @author Darren Edale
 * @date April, 2020
 * @version 1.1.1
 *
 * @brief Definition of the NotificationWidget class.
 */

#ifndef QYNC_NOTIFICATIONWIDGET_H
#define QYNC_NOTIFICATIONWIDGET_H

#include <QtWidgets/QWidget>

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
		explicit NotificationWidget(QWidget * = nullptr);
		~NotificationWidget() override;

		[[nodiscard]] QString message() const;
		void setMessage(const QString &);

		inline NotificationType type() const {
			return m_type;
		}

		void setType(NotificationType);

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
