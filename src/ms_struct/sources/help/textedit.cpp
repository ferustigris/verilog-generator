#include "textedit.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QTextCursor>
#include <QUrl>
#include <QTime>

const QString rsrcPath = ":/textedit/images";

TextEdit::TextEdit(QWidget *parent)
    : QWidget(parent), layout(parent), toolBar1(parent), toolBar2(parent)
{
    setupEditActions();
    setupTextActions();

    {
	setLayout(&layout);
	layout.addItem(&toolBar1);
	layout.addItem(&toolBar2);
    }
    textEdit = new QTextEdit(this);
    connect(textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
	    this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    connect(textEdit, SIGNAL(cursorPositionChanged()),
	    this, SLOT(cursorPositionChanged()));

    layout.addWidget(textEdit);
    textEdit->setFocus();

    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
	    this, SLOT(setWindowModified(bool)));
    connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
	    actionUndo, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(redoAvailable(bool)),
	    actionRedo, SLOT(setEnabled(bool)));

    setWindowModified(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

    connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);

    connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));

    QString initialFile = ":/example.html";
    const QStringList args = QCoreApplication::arguments();
    if (args.count() == 2)
	initialFile = args.at(1);
}

void TextEdit::closeEvent(QCloseEvent *e)
{
	e->accept();
}

void TextEdit::setupEditActions()
{

    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Edit Actions"));
    toolBar1.addWidget(tb);

    QAction *a;
    a = new QAction(QIcon(rsrcPath + "/fileopen.png"), tr("&Open image"), this);
    a->setShortcut(QKeySequence::Open);
    connect(a, SIGNAL(triggered()), this, SLOT(imageOpen()));
    tb->addAction(a);
    a = actionUndo = new QAction(QIcon(rsrcPath + "/editundo.png"), tr("Undo"), this);
    a->setShortcut(QKeySequence::Undo);
    tb->addAction(a);
    a = actionRedo = new QAction(QIcon(rsrcPath + "/editredo.png"), tr("&Redo"), this);
    a->setShortcut(QKeySequence::Redo);
    tb->addAction(a);
    a = actionCut = new QAction(QIcon(rsrcPath + "/editcut.png"), tr("Cu&t"), this);
    a->setShortcut(QKeySequence::Cut);
    tb->addAction(a);
    a = actionCopy = new QAction(QIcon(rsrcPath + "/editcopy.png"), tr("&Copy"), this);
    a->setShortcut(QKeySequence::Copy);
    tb->addAction(a);
    a = actionPaste = new QAction(QIcon(rsrcPath + "/editpaste.png"), tr("&Paste"), this);
    a->setShortcut(QKeySequence::Paste);
    tb->addAction(a);
    actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void TextEdit::setupTextActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Format Actions"));
    toolBar1.addWidget(tb);

    actionTextBold = new QAction(QIcon(rsrcPath + "/textbold.png"), tr("&Bold"), this);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    tb->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(QIcon(rsrcPath + "/textitalic.png"), tr("&Italic"), this);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    tb->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(QIcon(rsrcPath + "/textunder.png"), tr("&Underline"), this);
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    tb->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(textAlign(QAction *)));

    // Make sure the alignLeft  is always left of the alignRight
    if (QApplication::isLeftToRight()) {
	actionAlignLeft = new QAction(QIcon(rsrcPath + "/textleft.png"), tr("&Left"), grp);
	actionAlignCenter = new QAction(QIcon(rsrcPath + "/textcenter.png"), tr("C&enter"), grp);
	actionAlignRight = new QAction(QIcon(rsrcPath + "/textright.png"), tr("&Right"), grp);
    } else {
	actionAlignRight = new QAction(QIcon(rsrcPath + "/textright.png"), tr("&Right"), grp);
	actionAlignCenter = new QAction(QIcon(rsrcPath + "/textcenter.png"), tr("C&enter"), grp);
	actionAlignLeft = new QAction(QIcon(rsrcPath + "/textleft.png"), tr("&Left"), grp);
    }
    actionAlignJustify = new QAction(QIcon(rsrcPath + "/textjustify.png"), tr("&Justify"), grp);

    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);

    tb->addActions(grp->actions());

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, tr("&Color..."), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    tb->addAction(actionTextColor);

    tb = new QToolBar(this);
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    tb->setWindowTitle(tr("Format Actions"));
    toolBar2.addWidget(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    connect(comboStyle, SIGNAL(activated(int)),
	    this, SLOT(textStyle(int)));

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, SIGNAL(activated(const QString &)),
	    this, SLOT(textFamily(const QString &)));

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
	comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(const QString &)),
	    this, SLOT(textSize(const QString &)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
								   .pointSize())));
}

bool TextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
	return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
	return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str))
    {
	textEdit->setHtml(str);
    } else
    {
	str = QString::fromLocal8Bit(data);
	textEdit->setPlainText(str);
    }
    return true;
}

QString TextEdit::getCurrentFileName()
{
	return fileName;
}
//! set file name
void TextEdit::setCurrentFileName(const QString &curPath)
{
	path = curPath + "/help/";
	QDir dir;
	dir.mkpath(path);
	textEdit->document()->setModified(false);
	dir.setCurrent(path);
}

void TextEdit::fileOpen(const QString &curFile) {
    QDir dir;
	fileName = curFile;
	if(!QFile::exists(path + fileName)||fileName.isEmpty())
	{
		QString newFile("");
		do
		{
			newFile = "page_" + QTime::currentTime().toString("mmsszzz") + ".html";
		}
		while (QFile::exists(path + newFile));
		fileName = newFile;
	}
	name = fileName;
	name.replace(".html","");
	QString imagesPath = path + name;
	dir.mkpath(imagesPath);
	load(path + fileName);
}

void TextEdit::imageOpen()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
	QString(), tr("images (*.png *.bmp *.jpg *.jpeg);;All Files (*)"));
	if (!fn.isEmpty()) {
		QMimeData mimeData;
		mimeData.setImageData(QImage(fn));
		QTextCursor cursor = this->textEdit->textCursor();
		QTextDocument *document = this->textEdit->document();
		QString newfile, imageName;
		do {
			imageName = name + "/img_" + QTime::currentTime().toString("mmsszzz.") + fn.section(".",-1);
			newfile = path + imageName;
		} while(QFile::exists(newfile));
		QFile::copy(fn, newfile);
		document->addResource(QTextDocument::ImageResource, newfile, QImage(newfile));
		cursor.insertImage(imageName);
		listAddedImages<<newfile;
	}
}
QStringList TextEdit::getAddedImages() const
{
	return listAddedImages;
}
//! save our file
QString TextEdit::fileSave()
{
	QFile file(path + fileName);
	file.open(QIODevice::WriteOnly);
	QTextStream text(&file);
	text<<textEdit->document()->toHtml();
	file.close();
	return textEdit->document()->toHtml();
}

void TextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
	QTextCharFormat fmt;
	fmt.setFontPointSize(pointSize);
	mergeFormatOnWordOrSelection(fmt);
    }
}

void TextEdit::textStyle(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();

    if (styleIndex != 0) {
	QTextListFormat::Style style = QTextListFormat::ListDisc;

	switch (styleIndex) {
	    default:
	    case 1:
		style = QTextListFormat::ListDisc;
		break;
	    case 2:
		style = QTextListFormat::ListCircle;
		break;
	    case 3:
		style = QTextListFormat::ListSquare;
		break;
	    case 4:
		style = QTextListFormat::ListDecimal;
		break;
	    case 5:
		style = QTextListFormat::ListLowerAlpha;
		break;
	    case 6:
		style = QTextListFormat::ListUpperAlpha;
		break;
	}

	cursor.beginEditBlock();

	QTextBlockFormat blockFmt = cursor.blockFormat();

	QTextListFormat listFmt;

	if (cursor.currentList()) {
	    listFmt = cursor.currentList()->format();
	} else {
	    listFmt.setIndent(blockFmt.indent() + 1);
	    blockFmt.setIndent(0);
	    cursor.setBlockFormat(blockFmt);
	}

	listFmt.setStyle(style);

	cursor.createList(listFmt);

	cursor.endEditBlock();
    } else {
	// ####
	QTextBlockFormat bfmt;
	bfmt.setObjectIndex(-1);
	cursor.mergeBlockFormat(bfmt);
    }
}

void TextEdit::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
	return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void TextEdit::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
	textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
	textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
	textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
	textEdit->setAlignment(Qt::AlignJustify);
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEdit::cursorPositionChanged()
{
    alignmentChanged(textEdit->alignment());
}

void TextEdit::clipboardDataChanged()
{
    actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
	cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
	actionAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
	actionAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
	actionAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
	actionAlignJustify->setChecked(true);
    }
}
 void TextEdit::insertFromMimeData( const QMimeData *source )
 {
     if (source->hasImage())
     {
	 QImage image = qvariant_cast<QImage>(source->imageData());
	 QTextCursor cursor = this->textEdit->textCursor();
	 QTextDocument *document = this->textEdit->document();
	 document->addResource(QTextDocument::ImageResource, QUrl("image"), image);
	 cursor.insertImage("image");
     }
 }
