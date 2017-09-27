#ifndef SYNCHRONISEWHATCOMBO_H
#define SYNCHRONISEWHATCOMBO_H

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

#endif  // SYNCHRONISEWHATCOMBO_H
