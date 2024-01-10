#include "hottestwidget.h"

#include "ui_hottestwidget.h"

// #define TEST_MAPPING

HotTestWidget::HotTestWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::HotTestWidget), hotkey_1(new QHotkey(this)), hotkey_2(new QHotkey(this)),
      hotkey_3(new QHotkey(this)), hotkey_4(new QHotkey(NULL)), hotkey_5(new QHotkey(NULL)),
      thread4(new QThread(this)), thread5(new QThread(this)), testHotkeys(), nativeHotkey(new QHotkey(this))
{
    ui->setupUi(this);
    thread4->start();
    thread5->start();

#ifdef TEST_MAPPING
    // shortcut mapping override
    QHotkey::addGlobalMapping(
        QKeySequence("X"),
        QHotkey::NativeShortcut()); // add invalid mapping to test if the overwrite works for all platforms
#endif

    // 1
    connect(ui->hotkeyCheckbox_1, &QCheckBox::toggled, hotkey_1, &QHotkey::setRegistered);
    connect(ui->hotkeySequenceEdit_1, &QKeySequenceEdit::keySequenceChanged, this,
            &HotTestWidget::setShortcut_1);
    connect(hotkey_1, &QHotkey::activated, this, &HotTestWidget::increase_1);

    // 2
    connect(ui->hotkeyCheckbox_2, &QCheckBox::toggled, hotkey_2, &QHotkey::setRegistered);
    connect(ui->hotkeySequenceEdit_2, &QKeySequenceEdit::keySequenceChanged, this,
            &HotTestWidget::setShortcut_2);
    connect(hotkey_2, &QHotkey::activated, this, &HotTestWidget::increase_2);

    // 3
    connect(ui->hotkeyCheckbox_3, &QCheckBox::toggled, hotkey_3, &QHotkey::setRegistered);
    connect(ui->hotkeySequenceEdit_3, &QKeySequenceEdit::keySequenceChanged, this,
            &HotTestWidget::setShortcut_3);
    connect(hotkey_3, &QHotkey::activated, this, &HotTestWidget::increase_3);

    // 4
    connect(ui->hotkeyCheckbox_4, &QCheckBox::toggled, hotkey_4, &QHotkey::setRegistered);
    connect(ui->hotkeySequenceEdit_4, &QKeySequenceEdit::keySequenceChanged, this,
            &HotTestWidget::setShortcut_4);
    connect(hotkey_4, &QHotkey::activated, this, &HotTestWidget::increase_4);

    // 5
    connect(ui->hotkeyCheckbox_5, &QCheckBox::toggled, hotkey_5, &QHotkey::setRegistered);
    connect(ui->hotkeySequenceEdit_5, &QKeySequenceEdit::keySequenceChanged, this,
            &HotTestWidget::setShortcut_5);
    connect(hotkey_5, &QHotkey::activated, this, &HotTestWidget::increase_5);

    // test connections
    testHotkeys += new QHotkey(Qt::Key_F, Qt::NoModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyFCheckBox, &QCheckBox::toggle);
    testHotkeys +=
        new QHotkey(Qt::Key_F12, Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyCtrlAltMetaF12CheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_Cancel, Qt::ControlModifier | Qt::ShiftModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyCtrlShiftCancelCheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_Delete, Qt::MetaModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyMetaDelCheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_NumLock, Qt::NoModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyNumlockCheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_5, Qt::ControlModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyCtrl5CheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_Tab, Qt::ShiftModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyShiftTabCheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_Comma, Qt::ShiftModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyShiftCheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_Semicolon, Qt::ShiftModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyShiftCheckBox_2, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_K, Qt::ShiftModifier | Qt::AltModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyShiftAltKCheckBox, &QCheckBox::toggle);
    testHotkeys += new QHotkey(Qt::Key_K, Qt::ShiftModifier | Qt::AltModifier, false, this);
    connect(testHotkeys.last(), &QHotkey::activated, ui->hotkeyShiftAltKCheckBox_2, &QCheckBox::toggle);

    // native
    connect(nativeHotkey, &QHotkey::activated, this, &HotTestWidget::increase_native);
}

HotTestWidget::~HotTestWidget()
{
    thread4->quit();
    thread4->wait();
    thread5->quit();
    thread5->wait();

    delete hotkey_4;
    delete hotkey_5;

    delete ui;
}

void HotTestWidget::setShortcut_1(const QKeySequence& sequence) { hotkey_1->setShortcut(sequence, false); }

void HotTestWidget::setShortcut_2(const QKeySequence& sequence) { hotkey_2->setShortcut(sequence, false); }

void HotTestWidget::setShortcut_3(const QKeySequence& sequence) { hotkey_3->setShortcut(sequence, false); }

void HotTestWidget::setShortcut_4(const QKeySequence& sequence) { hotkey_4->setShortcut(sequence, false); }

void HotTestWidget::setShortcut_5(const QKeySequence& sequence) { hotkey_5->setShortcut(sequence, false); }

void HotTestWidget::increase_1() { ui->hotkeyCount_1->display(ui->hotkeyCount_1->intValue() + 1); }

void HotTestWidget::increase_2() { ui->hotkeyCount_2->display(ui->hotkeyCount_2->intValue() + 1); }

void HotTestWidget::increase_3() { ui->hotkeyCount_3->display(ui->hotkeyCount_3->intValue() + 1); }

void HotTestWidget::increase_4() { ui->hotkeyCount_4->display(ui->hotkeyCount_4->intValue() + 1); }

void HotTestWidget::increase_5() { ui->hotkeyCount_5->display(ui->hotkeyCount_5->intValue() + 1); }

void HotTestWidget::on_resetButton_1_clicked() { ui->hotkeyCount_1->display(0); }

void HotTestWidget::on_resetButton_2_clicked() { ui->hotkeyCount_2->display(0); }

void HotTestWidget::on_resetButton_3_clicked() { ui->hotkeyCount_3->display(0); }

void HotTestWidget::on_resetButton_4_clicked() { ui->hotkeyCount_4->display(0); }

void HotTestWidget::on_resetButton_5_clicked() { ui->hotkeyCount_5->display(0); }

void HotTestWidget::on_groupBox_toggled(bool checked)
{
    for (QHotkey *hotkey : testHotkeys) hotkey->setRegistered(checked);
}

void HotTestWidget::on_threadEnableCheckBox_clicked()
{
    ui->threadEnableCheckBox->setEnabled(false);
    ui->hotkeyCheckbox_1->setChecked(false);
    ui->hotkeyCheckbox_2->setChecked(false);
    ui->hotkeyCheckbox_3->setChecked(false);
    ui->hotkeyCheckbox_4->setChecked(false);
    ui->hotkeyCheckbox_5->setChecked(false);

    QApplication::processEvents();

    Q_ASSERT(!hotkey_4->isRegistered());
    Q_ASSERT(!hotkey_5->isRegistered());

    hotkey_4->moveToThread(thread4);
    hotkey_5->moveToThread(thread5);

    QApplication::processEvents();
    Q_ASSERT(hotkey_4->thread() == thread4);
    Q_ASSERT(hotkey_5->thread() == thread5);

    connect(thread4, &QThread::finished, this, [this]() { hotkey_4->moveToThread(qApp->thread()); });
    connect(thread5, &QThread::finished, this, [this]() { hotkey_5->moveToThread(qApp->thread()); });

    ui->tabWidget->setCurrentIndex(0);
}

void HotTestWidget::on_registeredCheckBox_toggled(bool checked)
{
    if (checked) {
        nativeHotkey->setNativeShortcut(
            { (quint32)ui->nativeKeySpinBox->value(), (quint32)ui->nativeModifiersSpinBox->value() }, true);
    }
    else
        nativeHotkey->setRegistered(false);
}

void HotTestWidget::increase_native() { ui->nativeCount->display(ui->nativeCount->intValue() + 1); }
