#include "ashurawind.h"
#include "ui_ashurawind.h"

AshuraWind::AshuraWind(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AshuraWind)
{
    ui->setupUi(this);

    tabifyDockWidget(ui->dockWidget, ui->dockWidget_2);
    ui->dockWidget->raise();

    OnRestore();
    MenuUpdate();
}

AshuraWind::~AshuraWind()
{
    OnClose();
    delete ui;
}

void AshuraWind::OnClose()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Ashura", "TextChanger");

    settings.beginGroup("History of text changes");
    int numberFromEnd = 9;
    for (auto &s : mHistoryOfTextChanges) {
        settings.setValue(QString("from the end ") + QString::number(numberFromEnd--), s);
    }
    settings.endGroup();

    settings.beginGroup("History of text inputs");
    numberFromEnd = 9;
    for (auto &s : mHistoryOfTextInput) {
        settings.setValue(QString("from the end ") + QString::number(numberFromEnd--), s);
    }
    settings.endGroup();

    settings.beginGroup("History of text outputs");
    numberFromEnd = 9;
    for (auto &s : mHistoryOfTextOutput) {
        settings.setValue(QString("from the end ") + QString::number(numberFromEnd--), s);
    }
    settings.endGroup();

    settings.beginGroup("History of paths");
    numberFromEnd = 0;
    for (auto &s : mLastFilePath) {
        settings.setValue(QString("from the end ") + QString::number(numberFromEnd++), s);
    }
    settings.endGroup();

    settings.beginGroup("Patterns");
    settings.setValue(QString("Count"), mPatterns.count());
    numberFromEnd = 0;
    for (auto &s : mPatterns) {
        settings.setValue(QString("Pattern ") + QString::number(numberFromEnd++), s);
    }
    settings.endGroup();
}

void AshuraWind::OnRestore()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Ashura", "TextChanger");

    settings.beginGroup("History of text changes");
    int numberFromEnd = 9;
    while (numberFromEnd>=0) {
        mHistoryOfTextChanges.push_back(settings.value(QString("from the end ") + QString::number(numberFromEnd--), "").toString());
    }
    settings.endGroup();

    settings.beginGroup("History of text inputs");
    numberFromEnd = 9;
    while (numberFromEnd>=0) {
        mHistoryOfTextInput.push_back(settings.value(QString("from the end ") + QString::number(numberFromEnd--), "").toString());
    }
    settings.endGroup();

    settings.beginGroup("History of text outputs");
    numberFromEnd = 9;
    while (numberFromEnd>=0) {
        mHistoryOfTextOutput.push_back(settings.value(QString("from the end ") + QString::number(numberFromEnd--), "").toString());
    }
    settings.endGroup();

    settings.beginGroup("History of paths");
    numberFromEnd = 0;
    for (auto &s : mLastFilePath) {
        s = settings.value(QString("from the end ") + QString::number(numberFromEnd++), "").toString();
    }
    settings.endGroup();

    settings.beginGroup("Patterns");
    numberFromEnd = settings.value(QString("Count"), 0).toInt()-1;
    while (numberFromEnd>=0) {
        mPatterns.push_front( settings.value(QString("Pattern ") + QString::number(numberFromEnd--), "").toString() );
    }
    settings.endGroup();
}

void AshuraWind::MenuUpdate()
{
    for (auto &s : ui->menuOstatnie_teksty_zmian->actions()) {
        ui->menuOstatnie_teksty_zmian->removeAction(s);
        delete s;
    }
    for (auto &s : ui->menuOstatnie_teksty_wej_ciowe->actions()) {
        ui->menuOstatnie_teksty_wej_ciowe->removeAction(s);
        delete s;
    }
    for (auto &s : ui->menuOstatnio_zapisane_pliki->actions()) {
        ui->menuOstatnio_zapisane_pliki->removeAction(s);
        delete s;
    }
    for (auto &s : ui->menuSzablony->actions()) {
        ui->menuSzablony->removeAction(s);
        delete s;
    }

    QList<QAction*> lista;
    for (auto &s : mHistoryOfTextChanges) {
        QAction *act = new QAction(s);
        lista.push_front(act);
        connect(act, SIGNAL(triggered()), this, SLOT(slot_HistoryOfTextChanges()));
    }
    ui->menuOstatnie_teksty_zmian->addActions(lista);

    lista.clear();
    for (auto &s : mHistoryOfTextInput) {
        QAction *act = new QAction(s);
        lista.push_front(act);
        connect(act, SIGNAL(triggered()), this, SLOT(slot_HistoryOfTextInput()));
    }
    ui->menuOstatnie_teksty_wej_ciowe->addActions(lista);

    lista.clear();
    for (auto &s : mHistoryOfTextOutput) {
        QAction *act = new QAction(s);
        lista.push_front(act);
        connect(act, SIGNAL(triggered()), this, SLOT(slot_Historyoftextoutput()));
    }
    ui->menuOstatnio_zapisane_pliki->addActions(lista);

    lista.clear();
    for (auto &s : mPatterns) {
        QAction *act = new QAction(s);
        lista.push_front(act);
        connect(act, SIGNAL(triggered()), this, SLOT(slot_PatternChose()));
    }
    ui->menuSzablony->addActions(lista);
}

#define WYBIERZ_TEKST(idx, fileName) \
    QString path = (mLastFilePath[idx].isEmpty())?(qApp->applicationDirPath()):(mLastFilePath[idx]);                                            \
    fileName = QFileDialog::getOpenFileName(NULL, "Wybierz plik", path, "Plik tekstowy (*.*)", 0, QFileDialog::DontUseNativeDialog);


#define WCZYTAJ_TEKST(fileName, idx, text_obj, container) \
    if (!fileName.isEmpty()) {                                                                                                              \
        QFileInfo fi (fileName);                                                                                                            \
        if (fi.exists()) {                                                                                                                  \
            if (container.back().compare(fileName)!=0) \
                container.push_back(fileName);                                                                                      \
            mLastFilePath[idx] = fi.absoluteFilePath();                                                                                       \
            QFile file(fileName);                                                                                                           \
            if (file.open(QFile::ReadOnly)) {                                                                                               \
                text_obj->setPlainText(file.readAll());                                                                                \
                file.close();                                                                                                               \
            }                                                                                                                               \
        }                                                                                                                                   \
    }

void AshuraWind::on_actionWczytaj_tekst_zmian_triggered()
{
    QString fileName;
    WYBIERZ_TEKST(0, fileName);
    WCZYTAJ_TEKST(fileName, 0, ui->qmodifier, mHistoryOfTextChanges);
    MenuUpdate();
}

void AshuraWind::on_actionWczytaj_tekst_wej_ciowy_triggered()
{
    QString fileName;
    WYBIERZ_TEKST(1, fileName);
    WCZYTAJ_TEKST(fileName, 1, ui->qtextinput, mHistoryOfTextInput);
    MenuUpdate();
}

void AshuraWind::on_actionZapisz_tekst_wyj_ciowy_triggered()
{
    QString path = (mLastFilePath[2].isEmpty())?(qApp->applicationDirPath()):(mLastFilePath[2]);
    QString fileName = QFileDialog::getSaveFileName(NULL, "Wybierz plik", path, "Plik tekstowy (*.*)", 0, QFileDialog::DontUseNativeDialog);

    if (!fileName.isEmpty()) {
        QFileInfo fi (fileName);
        if (fi.exists()) {
            mHistoryOfTextOutput.push_back(fileName);
            mLastFilePath[0] = fi.absoluteFilePath();
            QFile file(fileName);
            if (file.open(QFile::WriteOnly)) {
                QTextStream stream(&file);
                stream << ui->qtextoutput->toPlainText();
                file.close();
            }
        }
    }
    MenuUpdate();
}

bool ExpressionConversion(QString &inOut) {
    inOut.replace(QString("\\r"), QString("\r"));
    inOut.replace(QString("\\\\"), QString("\\"));
    inOut.replace(QString("\\n"), QString("\n"));

    return true;
}

void AshuraWind::on_qgenerate_clicked()
{
    QString expression (ui->qexpression->text());
    QString modifier (ui->qmodifier->toPlainText());

    ExpressionConversion(expression);
    QRegularExpression re(expression);
    re.setPatternOptions(re.patternOptions() | QRegularExpression::DotMatchesEverythingOption | QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpressionMatchIterator i = re.globalMatch(modifier);

    QMap<QString, QString> changes;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString from = match.captured("from");
        QString to = match.captured("to");
        changes[from] = to;
    }

    ui->qtablechanges->clearContents();
    ui->qtablechanges->setRowCount(changes.size());
    int row=0;
    QStringList keys = changes.keys();
    for (auto &s : keys) {
        ui->qtablechanges->setItem(row, 0, new QTableWidgetItem(s));
        ui->qtablechanges->setItem(row, 1, new QTableWidgetItem(changes[s]));
        row++;
    }

    QString textin = ui->qtextinput->toPlainText();
    for (auto s = keys.rbegin(); s!=keys.rend(); s++) {
        QRegularExpression regexp("\\b"+(*s)+"\\b");
        textin.replace(regexp, changes[(*s)]);
    }

    ui->qtextoutput->setPlainText(textin);
}

void AshuraWind::slot_HistoryOfTextChanges()
{
    QAction *sndr = qobject_cast<QAction*>(sender());

    if (sndr!=NULL) {
        QString fileName = sndr->text();

        WCZYTAJ_TEKST(fileName, 0, ui->qmodifier, mHistoryOfTextChanges);
        MenuUpdate();
    }
}

void AshuraWind::slot_HistoryOfTextInput()
{
    QAction *sndr = qobject_cast<QAction*>(sender());

    if (sndr!=NULL) {
        QString fileName = sndr->text();

        WCZYTAJ_TEKST(fileName, 1, ui->qtextinput, mHistoryOfTextInput);
        MenuUpdate();
    }
}

void AshuraWind::slot_Historyoftextoutput()
{
    QAction *sndr = qobject_cast<QAction*>(sender());

    if (sndr!=NULL) {
        QString fileName = sndr->text();

        QDesktopServices::openUrl( QUrl::fromLocalFile( fileName ) );
    }
}

void AshuraWind::on_qexpression_textEdited(const QString &arg1)
{
    if (mPatterns.contains(arg1))
        ui->qcheck_save->setChecked(true);
    else
        ui->qcheck_save->setChecked(false);
}

void AshuraWind::on_qcheck_save_toggled(bool checked)
{
    if (checked) {
        QString pattern {ui->qexpression->text()};
        if (!mPatterns.contains(pattern)) {
            mPatterns.push_back(pattern);
            MenuUpdate();
        }
    } else {
        QString pattern {ui->qexpression->text()};
        if (mPatterns.removeAll(pattern)) {
            MenuUpdate();
        }
    }
}

void AshuraWind::slot_PatternChose()
{
    QAction *sndr = qobject_cast<QAction*>(sender());

    if (sndr!=NULL) {
        QString pattern = sndr->text();
        ui->qexpression->setText(pattern);
    }
}

void AshuraWind::on_qmodifier_textChanged()
{
    QTextDocument* doc = ui->qmodifier->document();
    ui->qspin_line_count->setValue(doc->lineCount());

    int wordCount = ui->qmodifier->toPlainText().split(QRegExp("(\\s|\\n|\\r)+")
                                                      , QString::SkipEmptyParts).count();
    ui->qspin_line_count->setValue(wordCount);

}
