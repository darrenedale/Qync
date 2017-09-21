#include "src/aboutdialogue.h"
#include "ui_aboutdialogue.h"

#include <QString>

#include "application.h"


namespace Qync {


	AboutDialogue::AboutDialogue(void)
	  : QDialog(nullptr),
		 m_ui(new Ui::AboutDialogue) {
		m_ui->setupUi(this);

		m_ui->title->setText(processPlaceholders(m_ui->title->text()));
		m_ui->about->setText(processPlaceholders(m_ui->about->text()));
		m_ui->dependencies->setText(processPlaceholders(m_ui->dependencies->text()));
		m_ui->website->setText(processPlaceholders(m_ui->website->text()));
		m_ui->buildDetails->setText(processPlaceholders(m_ui->buildDetails->text()));

		QFont f = m_ui->title->font();
		f.setBold(true);
		f.setPointSizeF(f.pointSizeF() * 1.2);
		m_ui->title->setFont(f);

		f = m_ui->buildDetails->font();
		f.setPointSizeF(f.pointSizeF() * 0.85);
		m_ui->buildDetails->setFont(f);

		connect(m_ui->controls, &QDialogButtonBox::accepted, this, &QDialog::close);
		connect(m_ui->controls, &QDialogButtonBox::rejected, this, &QDialog::close);
	}


	AboutDialogue::~AboutDialogue(void) = default;


	QString & AboutDialogue::processPlaceholders(QString && content) {
		content.replace("{ApplicationDisplayName}", qyncApp->applicationDisplayName());
		content.replace("{ApplicationWebsite}", qyncApp->websiteUrl());
		content.replace("{ReleaseDate}", qyncApp->releaseDate());
		content.replace("{BuildId}", qyncApp->buildId());
		return content;
	}


}  // namespace Qync
