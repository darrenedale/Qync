/**
 * \file QyncGuiPreferences.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the QyncGuiPreferences class.
 *
 * \dep
 * - QyncGuiPreferences.h
 * - common.h
 * - QDebug
 *
 * \todo
 * Nothing.
 */

#include "guipreferences.h"
#include "functions.h"

#include <QDebug>


namespace Qync {


	GuiPreferences::GuiPreferences(const QString & fileName)
	  : Preferences(fileName) {
		/* must call this here as well as in base class constructor - when base
	 * class constructor is called, the GuiPreferences object is not
	 * initialised and therefore load() uses the copy of parseXml() in the base
	 * class even though it is a virtual method because the copy of parseXml()
	 * in this class is not yet available. the extra call here ensures load()
	 * is called again when the copy of parseXml() in this class is available
	 * and that the full set of preferences, including gui settings, is loaded.
	 */
		load();
	}


	void GuiPreferences::setDefaults(void) {
		Preferences::setDefaults();
		setShowPresetsToolBar(true);
		setShowSynchroniseToolBar(true);
		setToolBarButtonStyle(Qt::ToolButtonFollowStyle);
	}


	Qt::ToolButtonStyle GuiPreferences::parseToolButtonStyleText(const QString & style, bool * ok) {
		if("icononly" == style.trimmed().toLower()) {
			if(ok)
				*ok = true;
			return Qt::ToolButtonIconOnly;
		}
		if("textonly" == style.trimmed().toLower()) {
			if(ok)
				*ok = true;
			return Qt::ToolButtonTextOnly;
		}
		else if("textbesideicon" == style.trimmed().toLower()) {
			if(ok)
				*ok = true;
			return Qt::ToolButtonTextBesideIcon;
		}
		else if("textundericon" == style.trimmed().toLower()) {
			if(ok)
				*ok = true;
			return Qt::ToolButtonTextUnderIcon;
		}
		else if("styledefault" == style.trimmed().toLower()) {
			if(ok)
				*ok = true;
			return Qt::ToolButtonFollowStyle;
		}

		if(ok)
			*ok = false;
		return Qt::ToolButtonFollowStyle;
	}


	bool GuiPreferences::emitXmlElements(QXmlStreamWriter & xml) const {
		Preferences::emitXmlElements(xml);
		xml.writeStartElement("guipreferences");
		emitPresetsToolbarXml(xml);
		emitSynchroniseToolbarXml(xml);
		emitToolBarButtonStyleXml(xml);
		xml.writeEndElement(); /* guipreferences */
		return true;
	}


	bool GuiPreferences::emitPresetsToolbarXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("presetstoolbar");
		xml.writeCharacters(showPresetsToolBar() ? "true" : "false");
		xml.writeEndElement();
		return true;
	}


	bool GuiPreferences::emitSynchroniseToolbarXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("synchronisetoolbar");
		xml.writeCharacters(showSynchroniseToolBar() ? "true" : "false");
		xml.writeEndElement();
		return true;
	}


	bool GuiPreferences::emitToolBarButtonStyleXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("toolbarbuttonstyle");

		switch(toolBarButtonStyle()) {
			case Qt::ToolButtonIconOnly:
				xml.writeCharacters("IconOnly");
				break;

			case Qt::ToolButtonTextOnly:
				xml.writeCharacters("TextOnly");
				break;

			case Qt::ToolButtonTextBesideIcon:
				xml.writeCharacters("TextBesideIcon");
				break;

			case Qt::ToolButtonTextUnderIcon:
				xml.writeCharacters("TextUnderIcon");
				break;

			default:
			case Qt::ToolButtonFollowStyle:
				xml.writeCharacters("StyleDefault");
				break;
		}

		xml.writeEndElement();
		return true;
	}


	bool GuiPreferences::parseXmlElement(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement());

		if("guipreferences" == xml.name())
			return parseGuiPreferencesXml(xml);
		return Preferences::parseXmlElement(xml);
	}


	bool GuiPreferences::parseGuiPreferencesXml(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement() && "guipreferences" == xml.name());

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement())
				break;

			if(xml.isCharacters()) {
				if(!xml.isWhitespace())
					qDebug() << "GuiPreferences::parseGuiPreferencesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();

				/* ignore extraneous characters */
				continue;
			}

			if("presetstoolbar" == xml.name()) {
				bool ok;
				bool value = Preferences::parseBooleanText(xml.readElementText(), &ok);
				if(ok)
					setShowPresetsToolBar(value);
			}
			else if("synchronisetoolbar" == xml.name()) {
				bool ok;
				bool value = Preferences::parseBooleanText(xml.readElementText(), &ok);
				if(ok)
					setShowSynchroniseToolBar(value);
			}
			else if("toolbarbuttonstyle" == xml.name()) {
				bool ok;
				Qt::ToolButtonStyle value = GuiPreferences::parseToolButtonStyleText(xml.readElementText(), &ok);
				if(ok)
					this->setToolBarButtonStyle(value);
			}
			else
				Qync::parseUnknownElementXml(xml);
		}

		return true;
	}


	bool GuiPreferences::showPresetsToolBar(void) const {
		return m_presetsToolbar;
	}


	bool GuiPreferences::showSynchroniseToolBar(void) const {
		return m_syncToolbar;
	}


	void GuiPreferences::setShowPresetsToolBar(bool show) {
		m_presetsToolbar = show;
	}


	void GuiPreferences::setShowSynchroniseToolBar(bool show) {
		m_syncToolbar = show;
	}


	Qt::ToolButtonStyle GuiPreferences::toolBarButtonStyle(void) const {
		return m_toolButtonStyle;
	}


	bool GuiPreferences::setToolBarButtonStyle(const Qt::ToolButtonStyle & style) {
		m_toolButtonStyle = style;
		return true;
	}


}  // namespace Qync
