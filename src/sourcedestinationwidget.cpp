/**
 * \file sourcedestinationwidget.cpp
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Implementation of the SourceDestinationWidget class.
 *
 * \dep
 * - sourcedestinationwidget.h
 * - sourcedestinationwidget.ui
 * - QFileDialog
 * - QCompleter
 * - QFileSystemModel
 */

#include "sourcedestinationwidget.h"
#include "ui_sourcedestinationwidget.h"

#include <QFileDialog>
#include <QCompleter>
#include <QFileSystemModel>


namespace Qync {


	/**
	 * \class SourceDestinationWidget
	 * \author Darren Edale
	 * \date October 2017
	 * \version 1.1.0
	 *
	 * \brief A custom widget to enable the user to set the source and destination
	 * for a sync.
	 *
	 * The user can manually type the source and destination, or use the file-select
	 * buttons to choose a file using a standard system file dialogue. The text entry
	 * components do not yet support inline completion (for local files/dirs), but
	 * this is a planned feature.
	 *
	 * The source and destination can be set using setSource()/setDestination() and
	 * read using source()/destination(). You can also customise the labels for these
	 * components using setSourceLabel()/setDestinationLabel().
	 */


	/**
	 * \brief Create a SourceDestinationWidget.
	 *
	 * \param parent The owning parent widget.
	 */
	SourceDestinationWidget::SourceDestinationWidget(QWidget * parent)
	: QWidget(parent),
	  m_ui(new Ui::SourceDestinationWidget) {
		m_ui->setupUi(this);
		adjustSize();
		setMinimumHeight(height());
		setMaximumHeight(height());

		// completer is destroyed in when SourceDestinationWidget
		// is destroyed; fsMOdel destroyed when completer destroyed
		auto * completer = new QCompleter(this);
		auto * fsModel = new QFileSystemModel(completer);
		fsModel->setRootPath("");
		completer->setModel(fsModel);
		m_ui->source->setCompleter(completer);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

		// ...ditto...
		completer = new QCompleter(this);
		fsModel = new QFileSystemModel(completer);
		fsModel->setRootPath("");
		completer->setModel(fsModel);
		m_ui->destination->setCompleter(completer);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

		/* designer/.ui file can't yet connect signals to signals */
		connect(m_ui->source, &QLineEdit::textEdited, this, &SourceDestinationWidget::sourceChanged);
		connect(m_ui->destination, &QLineEdit::textEdited, this, &SourceDestinationWidget::destinationChanged);
	}


	/**
	 * \brief Fetch the current source.
	 *
	 * \return The source.
	 */
	QString SourceDestinationWidget::source() const {
		return m_ui->source->text();
	}


	/**
	 * \brief Set the current source.
	 *
	 * \param src The source.
	 */
	void SourceDestinationWidget::setSource(const QString & src) {
		if(src != m_ui->source->text()) {
			m_ui->source->setText(src);
			qobject_cast<QFileSystemModel *>(m_ui->source->completer()->model())->setRootPath(QFileInfo(src).absolutePath());
			Q_EMIT sourceChanged(src);
		}
	}


	/**
	 * \brief Fetch the current destination.
	 *
	 * \return The destination.
	 */
	QString SourceDestinationWidget::destination() const {
		return m_ui->destination->text();
	}


	/**
	 * \brief Set the current destination.
	 *
	 * \param dest The destination.
	 */
	void SourceDestinationWidget::setDestination(const QString & dest) {
		if(dest != m_ui->destination->text()) {
			m_ui->destination->setText(dest);
			qobject_cast<QFileSystemModel *>(m_ui->destination->completer()->model())->setRootPath(QFileInfo(dest).absolutePath());
			Q_EMIT destinationChanged(dest);
		}
	}


	/**
	 * \brief Set the label for the source widget.
	 *
	 * \param label The source widget label.
	 */
	void SourceDestinationWidget::setSourceLabel(const QString & label) {
		m_ui->sourceLabel->setText(label);
	}


	/**
	 * \brief Set the label for the destination widget.
	 *
	 * \param label The destination widget label.
	 */
	void SourceDestinationWidget::setDestinationLabel(const QString & label) {
		m_ui->destinationLabel->setText(label);
	}


	/**
	 * \brief Swap the source and destination. */
	void SourceDestinationWidget::swapSourceAndDestination(void) {
		QString t = m_ui->source->text();
		m_ui->source->setText(m_ui->destination->text());
		m_ui->destination->setText(t);
	}


	/** \brief Destroy the SourceDestinationWidget. */
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
