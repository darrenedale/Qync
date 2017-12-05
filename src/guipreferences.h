/**
 * \file guipreferences.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the GuiPreferences class.
 *
 * \dep
 * - preferences.h
 * - QString
 * - QXmlStreamReader
 * - QXmlStreamWriter
 */

#ifndef QYNC_GUIPREFERENCES_H
#define QYNC_GUIPREFERENCES_H

#include "preferences.h"

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Qync {

	class GuiPreferences
	: public Preferences {
	public:
		explicit GuiPreferences(const QString & fileName = {});
		virtual ~GuiPreferences();

		inline bool useSimpleUi() const {
			return m_simpleUi;
		}

		inline void setUseSimpleUi(bool use) {
			m_simpleUi = use;
		}

		inline bool showPresetsToolBar() const {
			return m_presetsToolbar;
		}

		inline bool showSynchroniseToolBar() const {
			return m_syncToolbar;
		}

		inline void setShowPresetsToolBar(bool show) {
			m_presetsToolbar = show;
		}

		inline void setShowSynchroniseToolBar(bool show) {
			m_syncToolbar = show;
		}

		inline Qt::ToolButtonStyle toolBarButtonStyle() const {
			return m_toolButtonStyle;
		}

		inline bool setToolBarButtonStyle(const Qt::ToolButtonStyle & style) {
			m_toolButtonStyle = style;
			return true;
		}

	protected:
		static optional<Qt::ToolButtonStyle> parseToolButtonStyleText(const QString & style);

		virtual void setDefaults() override;
		virtual bool parseXmlElement(QXmlStreamReader & xml) override;
		bool parseGuiPreferencesXml(QXmlStreamReader & xml);

		virtual bool emitXmlElements(QXmlStreamWriter & xml) const override;
		bool emitSimpleUiXml(QXmlStreamWriter & xml) const;
		bool emitPresetsToolbarXml(QXmlStreamWriter & xml) const;
		bool emitSynchroniseToolbarXml(QXmlStreamWriter & xml) const;
		bool emitToolBarButtonStyleXml(QXmlStreamWriter & xml) const;

	private:
		bool m_simpleUi;
		bool m_presetsToolbar;
		bool m_syncToolbar;
		Qt::ToolButtonStyle m_toolButtonStyle;
	};

}  // namespace Qync

#endif  // QYNC_GUIPREFERENCES_H
