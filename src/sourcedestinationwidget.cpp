/**
 * \file sourcedestinationwidget.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the SourceDestinationWidget class.
 *
 * \dep
 * - sourcedestinationwidget.h
 * - sourcedestinationwidget.ui
 * - QFileDialog
 */

#include "sourcedestinationwidget.h"
#include "ui_sourcedestinationwidget.h"

#include <QFileDialog>


namespace Qync {


	SourceDestinationWidget::SourceDestinationWidget(QWidget * parent)
	: QWidget(parent),
	  m_ui(new Ui::SourceDestinationWidget) {
		m_ui->setupUi(this);
		adjustSize();
		setMinimumHeight(height());
		setMaximumHeight(height());

		/* designer/.ui file can't yet connect signals to signals */
		connect(m_ui->source, &QLineEdit::textEdited, this, &SourceDestinationWidget::sourceChanged);
		connect(m_ui->destination, &QLineEdit::textEdited, this, &SourceDestinationWidget::destinationChanged);
	}


	QString SourceDestinationWidget::source() const {
		return m_ui->source->text();
	}


	void SourceDestinationWidget::setSource(const QString & src) {
		if(src != m_ui->source->text()) {
			m_ui->source->setText(src);
			Q_EMIT sourceChanged(src);
		}
	}


	QString SourceDestinationWidget::destination() const {
		return m_ui->destination->text();
	}


	void SourceDestinationWidget::setSourceLabel(const QString & label) {
		m_ui->sourceLabel->setText(label);
	}


	void SourceDestinationWidget::setDestinationLabel(const QString & label) {
		m_ui->destinationLabel->setText(label);
	}


	void SourceDestinationWidget::setDestination(const QString & dest) {
		if(dest != m_ui->destination->text()) {
			m_ui->destination->setText(dest);
			Q_EMIT destinationChanged(dest);
		}
	}


	void SourceDestinationWidget::swapSourceAndDestination(void) {
		QString t = m_ui->source->text();
		m_ui->source->setText(m_ui->destination->text());
		m_ui->destination->setText(t);
	}


	SourceDestinationWidget::~SourceDestinationWidget(void) = default;


	//	/**
	//	 * \brief Choose a local file for the rsync source.
	//	 *
	//	 * A local file browser is presented for the user to to choose a source
	//	 * for rsync. If the user does not cancel the dialogue, the chosen file
	//	 * is set as the text in the source line edit.
	//	 */
	//	void SourceDestinationWidget::chooseSourceFile(void) {
	//		QString newSource = QFileDialog::getOpenFileName(this, tr("Choose source"), m_ui->source->text());

	//		if(!newSource.isNull()) {
	//			m_ui->source->setText(newSource);
	//		}
	//	}


	//	/**
	//	 * \brief Choose a local file for the rsync destination.
	//	 *
	//	 * A local file browser is presented for the user to to choose a
	//	 * destination for rsync. If the user does not cancel the dialogue, the
	//	 * chosen file is set as the text in the destination line edit.
	//	 */
	//	void SourceDestinationWidget::chooseDestinationFile(void) {
	//		QString newDest = QFileDialog::getOpenFileName(this, tr("Choose destination"), m_ui->destination->text());

	//		if(!newDest.isNull()) {
	//			m_ui->destination->setText(newDest);
	//		}
	//	}


	/**
	 * \brief Choose a local directory for the rsync source.
	 *
	 * A local directory browser is presented for the user to to choose a
	 * source for rsync. If the user does not cancel the dialogue, the
	 * chosen directory is set as the text in the source line edit.
	 */
	void SourceDestinationWidget::chooseSourceDirectory(void) {
		QString newSource = QFileDialog::getExistingDirectory(this, tr("Choose source"), m_ui->source->text());

		if(!newSource.isNull()) {
			if(!newSource.endsWith(("/"))) {
				newSource.append("/");
			}

			m_ui->source->setText(newSource);
		}
	}


	/**
	 * \brief Choose a local directory for the rsync destination.
	 *
	 * A local directory browser is presented for the user to to choose a
	 * destination for rsync. If the user does not cancel the dialogue, the
	 * chosen directory is set as the text in the destination line edit.
	 */
	void SourceDestinationWidget::chooseDestinationDirectory(void) {
		QString newDest = QFileDialog::getExistingDirectory(this, tr("Choose destination"), m_ui->destination->text());

		if(!newDest.isNull()) {
			if(!newDest.endsWith(("/"))) {
				newDest.append("/");
			}

			m_ui->destination->setText(newDest);
		}
	}


}  // namespace Qync
