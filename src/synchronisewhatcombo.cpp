/**
 * \file synchronisewhatcombo.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the SynchroniseWhatCombo widget class.
 *
 * \dep
 * - synchronisewhatcombo.h
 * - QComboBox
 */

#include "synchronisewhatcombo.h"

#include <QComboBox>


Q_DECLARE_METATYPE(Qync::SynchroniseWhatCombo::What)


namespace Qync {


	/**
	 * \class SynchroniseWhatCombo
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
	 *
	 * \brief A custom combo box for choosing what to synchronise.
	 *
	 * This widget offers the user a mutually-exclusive choice between
	 * synchronising everything, only those files from the source that are
	 * already present in the destination or only those files from the
	 * source that are not already present in the destination. These
	 * correspond to a full update, a changes-only update and a new-files-
	 * only update.
	 */


	/**
	 * \brief Create a "what to synchronise" combo box.
	 *
	 * \param parent The parent of the created combo box.
	 */
	SynchroniseWhatCombo::SynchroniseWhatCombo(QWidget * parent)
	: QComboBox(parent) {
		insertItem(0, tr("Everything"));
		insertItem(1, tr("Only files and directories already present at the destination"));
		insertItem(2, tr("Only files and directories that are not already present at the destination"));
		setItemData(0, static_cast<unsigned char>(What::Everything), WhatRole);
		setItemData(1, static_cast<unsigned char>(What::OnlyPreExisting), WhatRole);
		setItemData(2, static_cast<unsigned char>(What::OnlyNonExistant), WhatRole);

		/* make it possible to use the whatChanged() signal in queued connections */
		qRegisterMetaType<Qync::SynchroniseWhatCombo::What>();
	}


	/**
	 * \brief The current "what to synchronise" chosen.
	 *
	 * \return The current "what to synchronise" value.
	 */
	SynchroniseWhatCombo::What SynchroniseWhatCombo::what(void) const {
		/* if not set, will return 0 which does not cast to a valid What
		 * enumeration */
		return currentData(WhatRole).value<What>();
	}


	/**
	 * \brief Set the current "what to synchronise".
	 *
	 * \param newWhat The new "what to synchronise".
	 */
	void SynchroniseWhatCombo::setWhat(const SynchroniseWhatCombo::What & newWhat) {
		if(newWhat != what()) {
			setCurrentIndex(findData(static_cast<unsigned char>(newWhat), WhatRole));
			Q_EMIT whatChanged(newWhat);
		}
	}


}  // namespace Qync
