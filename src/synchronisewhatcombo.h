/**
 * \file synchronisewhatcombo.h
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Declaration of the SynchroniseWhatCombo widget class.
 *
 * \dep
 * - QComboBox
 */

#ifndef QYNC_SYNCHRONISEWHATCOMBO_H
#define QYNC_SYNCHRONISEWHATCOMBO_H

#include <QComboBox>

namespace Qync {

	class SynchroniseWhatCombo
	: public QComboBox {
		Q_OBJECT

	public:
		enum class What : unsigned char {
			Everything = 1,
			OnlyPreExisting,
			OnlyNonExistant,
		};

		explicit SynchroniseWhatCombo(QWidget * parent = nullptr);

		What what(void) const;

	public Q_SLOTS:
		void setWhat(const What & newWhat);

	Q_SIGNALS:
		void whatChanged(const What & currentWhat);

	private:
		static constexpr int WhatRole = Qt::UserRole + 1;
	};

}  // namespace Qync

#endif  // QYNC_SYNCHRONISEWHATCOMBO_H
