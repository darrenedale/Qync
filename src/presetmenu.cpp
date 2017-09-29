/**
 * \file presetmenu.cpp
 * \author Darren Edale
 * \date September, 2017
 * \version 1.0.0
 *
 * \brief Declaration of the PresetMenu class.
 *
 * \dep
 * - presetmenu.h
 * - application.h
 * - preset.h
 */

#include "presetmenu.h"

#include "application.h"
#include "preset.h"


namespace Qync {


	PresetMenu::PresetMenu(QWidget * parent)
	: QMenu(parent) {
		refresh();
		connect(this, &QMenu::triggered, [this](QAction * action) {
			int idx = action->data().toInt();
			Q_EMIT presetTriggered(qyncApp->preset(idx));
			Q_EMIT presetIndexTriggered(idx);
		});

		connect(qyncApp, &Application::presetsChanged, this, &PresetMenu::refresh);
	}


	void PresetMenu::refresh(void) {
		QMenu::clear();
		int i = 0;

		for(const auto & preset : qyncApp->presets()) {
			QAction * action = QMenu::addAction(preset->name());
			action->setData(i);
			++i;
		}
	}


}  // namespace Qync
