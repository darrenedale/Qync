/**
 * @file synchronisewhatcombo.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the SynchroniseWhatCombo widget class.
 */

#ifndef QYNC_SYNCHRONISEWHATCOMBO_H
#define QYNC_SYNCHRONISEWHATCOMBO_H

#include <QtWidgets/QComboBox>

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

		explicit SynchroniseWhatCombo(QWidget * = nullptr);

		[[nodiscard]] What what() const;

	public Q_SLOTS:
		void setWhat(const What & newWhat);

	Q_SIGNALS:
		void whatChanged(const What & currentWhat);
	};

}  // namespace Qync

#endif  // QYNC_SYNCHRONISEWHATCOMBO_H
