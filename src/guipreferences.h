/**
 * @file guipreferences.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the GuiPreferences class.
 */

#ifndef QYNC_GUIPREFERENCES_H
#define QYNC_GUIPREFERENCES_H

#include "preferences.h"
#include <optional>
#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>

namespace Qync {

	class GuiPreferences
	:   public Preferences {
	public:
		explicit GuiPreferences(const QString & = QStringLiteral());
		~GuiPreferences() override;

		[[nodiscard]] inline bool useSimpleUi() const
		{
			return m_simpleUi;
		}

		inline void setUseSimpleUi(bool use)
		{
			m_simpleUi = use;
		}

		[[nodiscard]] inline bool showPresetsToolBar() const
		{
			return m_presetsToolbar;
		}

		[[nodiscard]] inline bool showSynchroniseToolBar() const
		{
			return m_syncToolbar;
		}

		inline void setShowPresetsToolBar(bool show)
		{
			m_presetsToolbar = show;
		}

		inline void setShowSynchroniseToolBar(bool show)
		{
			m_syncToolbar = show;
		}

		[[nodiscard]] inline Qt::ToolButtonStyle toolBarButtonStyle() const
		{
			return m_toolButtonStyle;
		}

		inline bool setToolBarButtonStyle(const Qt::ToolButtonStyle & style) {
			m_toolButtonStyle = style;
			return true;
		}

	protected:
		static std::optional<Qt::ToolButtonStyle> parseToolButtonStyleText(const QString &);

		void setDefaults() override;
		bool parseXmlElement(QXmlStreamReader &) override;
		bool parseGuiPreferencesXml(QXmlStreamReader &);

		bool emitXmlElements(QXmlStreamWriter &) const override;
		bool emitSimpleUiXml(QXmlStreamWriter &) const;
		bool emitPresetsToolbarXml(QXmlStreamWriter &) const;
		bool emitSynchroniseToolbarXml(QXmlStreamWriter &) const;
		bool emitToolBarButtonStyleXml(QXmlStreamWriter &) const;

	private:
		bool m_simpleUi;
		bool m_presetsToolbar;
		bool m_syncToolbar;
		Qt::ToolButtonStyle m_toolButtonStyle;
	};

}  // namespace Qync

#endif  // QYNC_GUIPREFERENCES_H
