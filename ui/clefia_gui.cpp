#include <QApplication>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QWidget>
#include <QMessageBox>
#include <QTimer>

#include "clefia.hpp"
#include "key_schedule.hpp"
#include "utils.hpp"
#include "f_functions.hpp"
#include "sboxes.hpp"
#include "gf_mul.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <fstream>

using namespace std;
using namespace clefia;

class ClefiaGui : public QWidget {
    Q_OBJECT

public:
    ClefiaGui(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("CLEFIA Encryptor / Decryptor");

        encryptRadio = new QRadioButton("Encrypt File");
        decryptRadio = new QRadioButton("Decrypt File");
        encryptRadio->setChecked(true);
        auto *opBox = new QGroupBox("Operation");
        auto *opLayout = new QVBoxLayout;
        opLayout->addWidget(encryptRadio);
        opLayout->addWidget(decryptRadio);
        opBox->setLayout(opLayout);

        key128 = new QRadioButton("128-bit");
        key192 = new QRadioButton("192-bit");
        key256 = new QRadioButton("256-bit");
        key128->setChecked(true);
        auto *keyBox = new QGroupBox("CLEFIA Key Length");
        auto *keyLayout = new QVBoxLayout;
        keyLayout->addWidget(key128);
        keyLayout->addWidget(key192);
        keyLayout->addWidget(key256);
        keyBox->setLayout(keyLayout);

        inputEdit = new QLineEdit;
        outputEdit = new QLineEdit;
        keyEdit = new QLineEdit;

        inputEdit->setAlignment(Qt::AlignLeft);
        outputEdit->setAlignment(Qt::AlignLeft);
        keyEdit->setAlignment(Qt::AlignLeft);

        auto *inputBrowse = new QPushButton("Browse...");
        auto *outputBrowse = new QPushButton("Browse...");
        auto *keyBrowse = new QPushButton("Browse...");

        connect(inputBrowse, &QPushButton::clicked, this, [=] {
            inputEdit->setText(QFileDialog::getOpenFileName(this, "Select Input File"));
        });
        connect(outputBrowse, &QPushButton::clicked, this, [=] {
            outputEdit->setText(QFileDialog::getSaveFileName(this, "Select Output File"));
        });
        connect(keyBrowse, &QPushButton::clicked, this, [=] {
            keyEdit->setText(QFileDialog::getOpenFileName(this, "Select Key File"));
        });

        auto *leftCol = new QVBoxLayout;
        leftCol->addWidget(opBox);
        leftCol->addWidget(keyBox);

        auto *formLayout = new QVBoxLayout;

        auto *inputLabel = new QLabel("Input File:");
        inputLabel->setFixedWidth(80);
        inputLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        auto *inputRow = new QHBoxLayout;
        inputRow->addWidget(inputLabel);
        inputRow->addWidget(inputEdit);
        inputRow->addWidget(inputBrowse);
        auto *inputRowWrapper = new QHBoxLayout;
        inputRowWrapper->addSpacing(0);
        inputRowWrapper->addLayout(inputRow);
        formLayout->addLayout(inputRowWrapper);

        auto *outputLabel = new QLabel("Output File:");
        outputLabel->setFixedWidth(80);
        outputLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        auto *outputRow = new QHBoxLayout;
        outputRow->addWidget(outputLabel);
        outputRow->addWidget(outputEdit);
        outputRow->addWidget(outputBrowse);
        auto *outputRowWrapper = new QHBoxLayout;
        outputRowWrapper->addSpacing(0);
        outputRowWrapper->addLayout(outputRow);
        formLayout->addLayout(outputRowWrapper);

        auto *createOutputCheck = new QCheckBox("Create output file automatically");
        auto *outputExtra = new QHBoxLayout;
        outputExtra->addSpacing(80);
        outputExtra->addWidget(createOutputCheck);
        formLayout->addLayout(outputExtra);

        connect(createOutputCheck, &QCheckBox::toggled, this, [=](bool checked) {
            outputEdit->setEnabled(!checked);
            outputBrowse->setEnabled(!checked);
        });

        auto *keyLabel = new QLabel("Key File:");
        keyLabel->setFixedWidth(80);
        keyLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        auto *keyRow = new QHBoxLayout;
        keyRow->addWidget(keyLabel);
        keyRow->addWidget(keyEdit);
        keyRow->addWidget(keyBrowse);
        auto *keyRowWrapper = new QHBoxLayout;
        keyRowWrapper->addSpacing(0);
        keyRowWrapper->addLayout(keyRow);
        formLayout->addLayout(keyRowWrapper);

        auto *rightCol = new QVBoxLayout;
        rightCol->addLayout(formLayout);

        auto *mainLayout = new QHBoxLayout;
        mainLayout->addLayout(leftCol);
        mainLayout->addLayout(rightCol);

        auto *runBtn = new QPushButton("Run");
        auto *clearBtn = new QPushButton("Clear");
        auto *exitBtn = new QPushButton("Exit");

        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
        connect(runBtn, &QPushButton::clicked, this, &ClefiaGui::runOperation);
        connect(clearBtn, &QPushButton::clicked, this, [=] {
            inputEdit->clear();
            outputEdit->clear();
            keyEdit->clear();
        });

        auto *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(runBtn);
        btnLayout->addWidget(clearBtn);
        btnLayout->addWidget(exitBtn);

        auto *layout = new QVBoxLayout;
        layout->addLayout(mainLayout);
        layout->addSpacing(10);
        layout->addLayout(btnLayout);

        setLayout(layout);

        this->createOutputCheck = createOutputCheck;
    }

private:
    QLineEdit *inputEdit, *outputEdit, *keyEdit;
    QRadioButton *encryptRadio, *decryptRadio;
    QRadioButton *key128, *key192, *key256;
    QCheckBox *createOutputCheck;
    void runOperation();
};

#include "clefia_gui.moc"

void ClefiaGui::runOperation() {
    QString keyPath = keyEdit->text();
    QString inputPath = inputEdit->text();
    QString outputPath = outputEdit->text();

    if (keyPath.isEmpty() || inputPath.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Please select input and key files.");
        return;
    }

    if (createOutputCheck->isChecked()) {
        QFileInfo inputInfo(inputPath);
        QString base = inputInfo.completeBaseName();
        QString suffix = encryptRadio->isChecked() ? "_cipher.txt" : "_plain.txt";
        outputPath = inputInfo.dir().filePath(base + suffix);
        outputEdit->setText(outputPath);
    } else if (outputPath.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Please select output file or enable auto-creation.");
        return;
    }

    string hex_key, hex_input;
    ifstream keyFile(keyPath.toStdString());
    ifstream inFile(inputPath.toStdString());
    if (!keyFile || !inFile) {
        QMessageBox::critical(this, "File Error", "Failed to open input or key file.");
        return;
    }
    keyFile >> hex_key;
    inFile >> hex_input;

    if (decryptRadio->isChecked() && hex_input.length() % 32 != 0) {
        QMessageBox::critical(this, "Input Error", "Ciphertext length must be a multiple of 128 bits (32 hex chars).\nInvalid input length: " + QString::number(hex_input.length()));
        return;
    }

    KeyLength len = KeyLength::Bits128;
    if (key192->isChecked()) len = KeyLength::Bits192;
    else if (key256->isChecked()) len = KeyLength::Bits256;

    CipherParameters params = get_params(len);
    vector<Word> rk(params.numRoundKeys);
    Word kw[4];

    int count = 0;
    Byte keyBytes[32] = {0};
    hex_to_bytes(hex_key, keyBytes, count);

    switch (len) {
        case KeyLength::Bits128:
            GenerateRoundKeys128(keyBytes, kw, rk.data(), params.rounds);
            break;
        case KeyLength::Bits192:
            GenerateRoundKeys192(keyBytes, kw, rk.data());
            break;
        case KeyLength::Bits256:
            GenerateRoundKeys256(keyBytes, kw, rk.data());
            break;
    }

    if (encryptRadio->isChecked() && hex_input.length() % 32 != 0) {
        int pad = 32 - (hex_input.length() % 32);
        hex_input = string(pad, '0') + hex_input;
    }

    if (encryptRadio->isChecked()) {
        EncryptText(hex_input, kw, rk, params.rounds);
    } else {
        DecryptText(hex_input, kw, rk, params.rounds);
    }

    QTimer::singleShot(100, this, [=]() {
        ofstream outFile(outputPath.toStdString());
        if (!outFile) {
            QMessageBox::critical(this, "File Error", "Cannot open output file for writing.");
            return;
        }
        if (encryptRadio->isChecked()) {
            ifstream cipherOut("Ciphertext.txt");
            string text; cipherOut >> text;
            outFile << text;
        } else {
            ifstream plainOut("Decrypt.txt");
            string text; plainOut >> text;
            outFile << text;
        }
        QMessageBox::information(this, "Done", "Operation completed successfully.");
    });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ClefiaGui window;
    window.show();
    return app.exec();
}
