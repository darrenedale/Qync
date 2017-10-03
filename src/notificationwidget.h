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

		QString message(void) const;
		void setMessage(const QString & message);

		inline NotificationType type(void) const {
			return m_type;
		}

		void setType(NotificationType type);

	public Q_SLOTS:
		void show(void);
		void hide(void);

	private:
		std::unique_ptr<Ui::NotificationWidget> m_ui;
		std::unique_ptr<QPropertyAnimation> m_showAnim;
		std::unique_ptr<QPropertyAnimation> m_hideAnim;
		NotificationType m_type;
	};


}  // namespace Qync
#endif  // QYNC_NOTIFICATIONWIDGET_H
