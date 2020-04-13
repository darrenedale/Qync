/**
 * @file notificationwidget.cpp
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Implementation of the NotificationWidget class.
 */

#include "notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QDebug>
#include <QPropertyAnimation>

namespace Qync {

	/**
	 * @class NotificationWidget
	 * @author Darren Edale
	 * @date April 2020
	 * @version 1.1.1
	 *
	 * @brief Widget to show unobtrusive notifications.
	 *
	 * This widget provides a pop-up widget that animates gracefully into view when it is shown and animates gracefully
	 * out again when dismissed. This provides the user with an experience that is not as jarring as popping up a
	 * dialogue window with a message immediately in front of the user. It does not get in the way of the existing UI or
	 * steal focus from it, and does not suddenly change the visuals in front of the user.
	 *
	 * It supports three types of notification: information, warning and error. Information messages are unobtrusive,
	 * just appearing like a normal label text; warning messages appear with a yellow background; error messages with a
	 * red background. The colours are relatively subtle, just enough to draw attention without jarring.
	 *
	 * The intended us for this widget is as a usually-hidden widget embedded in a layout, which is made visible when it
	 * has a message to show. It is suitable for relatively simple, short messages that the user can dismiss. They do
	 * not support any other user interaction, such as questions or alternative responses. The messages can be used with
	 * Qt's HTML-like markup for rich content.
	 *
	 * To use the widget call setMessage() then call show(). If the widget is already visible, it will be updated to
	 * show the provided content. If it is not visible, it will have the content set and will be gently scrolled into
	 * view.
	 */

	/**
	 * @brief Create a NotificationWidget
	 *
	 * @param parent The owning parent for the created widget.
	 */
	NotificationWidget::NotificationWidget(QWidget * parent)
	:   QWidget(parent),
	    m_ui(std::make_unique<Ui::NotificationWidget>()),
	    m_showAnim(std::make_unique<QPropertyAnimation>(this, QByteArrayLiteral("maximumHeight"))),
	    m_hideAnim(std::make_unique<QPropertyAnimation>(this, QByteArrayLiteral("maximumHeight"))),
	    m_type(NotificationType::Information) {
		m_ui->setupUi(this);
		connect(m_ui->close, &QPushButton::clicked, this, &NotificationWidget::hide);
		m_showAnim->setStartValue(0);
		m_showAnim->setEndValue(50);
		m_hideAnim->setEndValue(0);
		m_showAnim->setDuration(300);
		m_hideAnim->setDuration(300);
		m_showAnim->setEasingCurve(QEasingCurve::InOutQuad);
		m_hideAnim->setEasingCurve(QEasingCurve::InOutQuad);

		connect(m_hideAnim.get(), &QPropertyAnimation::finished, [this]() {
			QWidget::hide();
		});
	}

	/** @brief Destroy the widget. */
	NotificationWidget::~NotificationWidget() = default;

	/**
	 * @brief Fetch the message currently shown in the widget.
	 *
	 * @return The message.
	 */
	QString NotificationWidget::message() const {
		return m_ui->message->text();
	}

	/**
	 * @brief Set the message currently shown in the widget.
	 *
	 * @param message The message.
	 *
	 * Setting the message does not automatically render the widget visible. To do this,
	 * call show().
	 */
	void NotificationWidget::setMessage(const QString & message) {
		m_ui->message->setText(message);
	}

	/**
	 * @brief Set the type of notifivation.
	 *
	 * @param type The type.
	 *
	 * The type will be set and the visuals of the widget will immediately be updated
	 * appropriately for the new type.
	 */
	void NotificationWidget::setType(NotificationType type) {
		if(type != m_type) {
			m_type = type;

			// update visuals
			switch(type) {
				case NotificationType::Information:
					m_ui->notificationFrame->setStyleSheet("");
					break;

				case NotificationType::Warning:
					m_ui->notificationFrame->setStyleSheet("background: rgb(192, 192, 128);");
					break;

				case NotificationType::Error:
					m_ui->notificationFrame->setStyleSheet("background: rgb(192, 128, 128);");
					break;
			}
		}
	}

	/**
	 * @brief Make the notification widget visible.
	 *
	 * If the widget is not currently visible, it will be gently scrolled into view. If it is already visible, or if the
	 * animation to scroll a previous message into view is in progress (i.e. show() has been called recently and its
	 * work has not yet finished), no action will be taken.
	 *
	 * This method is reimplemented from its base class but it is not virtual. This means that if the widget object is
	 * referenced by a base-class pointer or reference, the show() method as defined in that base class will be called
	 * not this reimplementation.
	 */
	void NotificationWidget::show() {
		if(m_showAnim->state() != QPropertyAnimation::Stopped) {
			qWarning() << "show animation is already running";
			return;
		}

		if(isVisible()) {
			return;
		}

		setMaximumHeight(QWIDGETSIZE_MAX);
		adjustSize();
		m_showAnim->setEndValue(height());
		setMaximumHeight(0);
		QWidget::show();
		m_showAnim->start();
	}

	/**
	 * @brief Hide the notification widget.
	 *
	 * If the widget is currently visible, it will be gently scrolled out of view. If it is already hidden, or if the
	 * animation to scroll it out of view is in progress (i.e. hide() has been called recently and its work has not yet
	 * finished), no action will be taken.
	 *
	 * This method is reimplemented from its base class but it is not virtual. This means that if the widget object is
	 * referenced by a base-class pointer or reference, the hide() method as defined in that base class will be called,
	 * not this reimplementation.
	 */
	void NotificationWidget::hide() {
		if(m_hideAnim->state() != QPropertyAnimation::Stopped) {
			qWarning() << "hide animation is already running";
			return;
		}

		if(isHidden()) {
			return;
		}

		m_hideAnim->start();
	}

}  // namespace Qync
