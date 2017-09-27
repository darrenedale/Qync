#include "synchronisewhatcombo.h"

#include <QComboBox>


Q_DECLARE_METATYPE(Qync::SynchroniseWhatCombo::What)


namespace Qync {


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


	SynchroniseWhatCombo::What SynchroniseWhatCombo::what(void) const {
		/* if not set, will return 0 which does not cast to a valid What
		 * enumeration */
		return currentData(WhatRole).value<What>();
	}


	void SynchroniseWhatCombo::setWhat(const SynchroniseWhatCombo::What & newWhat) {
		if(newWhat != what()) {
			setCurrentIndex(findData(static_cast<unsigned char>(newWhat), WhatRole));
			Q_EMIT whatChanged(newWhat);
		}
	}


}  // namespace Qync
