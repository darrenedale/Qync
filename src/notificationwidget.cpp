/**
 * \file notificationwidget.cpp
 * \author Darren Edale
 * \date October, 2017
 * \version 1.1.0
 *
 * \brief Implementation of the NotificationWidget class.
 *
 * \dep
 * - notificationwidget.h
 * - notificationwidget.ui
 * - QDebug
 * - QPropertyAnimation
 *
 * \todo Test that the warning/error colours are appropriate.
 * \todo Make warning/error colours configurable?
 * \todo use show/hide event methods rather than redefining show()/hide()
 */

#include "notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QDebug>
#include <QPropertyAnimation>


namespace Qync {


	NotificationWidget::NotificationWidget(QWidget * parent)
	: QWidget(parent),
	  m_ui(new Ui::NotificationWidget),
	  m_showAnim(std::make_unique<QPropertyAnimation>(this, "maximumHeight")),
	  m_hideAnim(std::make_unique<QPropertyAnimation>(this, "maximumHeight")),
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


	NotificationWidget::~NotificationWidget(void) = default;


	QString NotificationWidget::message() const {
		return m_ui->message->text();
	}


	void NotificationWidget::setMessage(const QString & message) {
		m_ui->message->setText(message);
	}


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


	void NotificationWidget::show(void) {
		if(m_showAnim->state() != QPropertyAnimation::Stopped) {
			qWarning() << "show animation is already running";
			return;
		}

		QWidget::hide();
		setMaximumHeight(INT_MAX);
		adjustSize();
		m_showAnim->setEndValue(height());
		setMaximumHeight(0);
		QWidget::show();
		m_showAnim->start();
	}


	void NotificationWidget::hide(void) {
		if(m_hideAnim->state() != QPropertyAnimation::Stopped) {
			qWarning() << "hide animation is already running";
			return;
		}

		m_hideAnim->start();
	}


}  // namespace Qync
