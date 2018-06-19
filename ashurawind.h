#ifndef ASHURAWIND_H
#define ASHURAWIND_H

#include <QtWidgets>
#include <boost/circular_buffer.hpp>


namespace Ui {
class AshuraWind;
}

class AshuraWind : public QMainWindow
{
    Q_OBJECT

public:
    explicit AshuraWind(QWidget *parent = 0);
    ~AshuraWind();

public slots:
    void OnClose();
    void OnRestore();
    void MenuUpdate();

private slots:
    void on_actionWczytaj_tekst_zmian_triggered();
    void on_actionWczytaj_tekst_wej_ciowy_triggered();
    void on_actionZapisz_tekst_wyj_ciowy_triggered();
    void on_qgenerate_clicked();

    void slot_HistoryOfTextChanges();
    void slot_HistoryOfTextInput();
    void slot_Historyoftextoutput();
    void on_qexpression_textEdited(const QString &arg1);
    void on_qcheck_save_toggled(bool checked);
    void slot_PatternChose();

    void on_qmodifier_textChanged();

private:
    Ui::AshuraWind *ui;
    boost::circular_buffer<QString> mHistoryOfTextChanges{10};
    boost::circular_buffer<QString> mHistoryOfTextInput{10};
    boost::circular_buffer<QString> mHistoryOfTextOutput{10};
    QStringList mPatterns;
    QString mLastFilePath[3];
};

#endif // ASHURAWIND_H
