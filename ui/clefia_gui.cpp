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

class ClefiaGui : public QWidget {
    Q_OBJECT

public:
    ClefiaGui(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("CLEFIA Encryptor / Decryptor");

        // === Operation Radio ===
        auto *encryptRadio = new QRadioButton("Encrypt File");
        auto *decryptRadio = new QRadioButton("Decrypt File");
        encryptRadio->setChecked(true);
        auto *opBox = new QGroupBox("Operation");
        auto *opLayout = new QVBoxLayout;
        opLayout->addWidget(encryptRadio);
        opLayout->addWidget(decryptRadio);
        opBox->setLayout(opLayout);

        // === Key Length Radio ===
        auto *key128 = new QRadioButton("128-bit");
        auto *key192 = new QRadioButton("192-bit");
        auto *key256 = new QRadioButton("256-bit");
        key128->setChecked(true);
        auto *keyBox = new QGroupBox("CLEFIA Key Length");
        auto *keyLayout = new QVBoxLayout;
        keyLayout->addWidget(key128);
        keyLayout->addWidget(key192);
        keyLayout->addWidget(key256);
        keyBox->setLayout(keyLayout);

        // === File Inputs ===
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

        // === Left Column
        auto *leftCol = new QVBoxLayout;
        leftCol->addWidget(opBox);
        leftCol->addWidget(keyBox);

        // === Right Column
        auto *formLayout = new QVBoxLayout;

        // Input
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

        // Output
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

        // Create output checkbox
        auto *createOutputCheck = new QCheckBox("Create output file automatically");
        auto *outputExtra = new QHBoxLayout;
        outputExtra->addSpacing(80);
        outputExtra->addWidget(createOutputCheck);
        formLayout->addLayout(outputExtra);

        connect(createOutputCheck, &QCheckBox::toggled, this, [=](bool checked) {
            outputEdit->setEnabled(!checked);
            outputBrowse->setEnabled(!checked);
        });

        // Key
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

        // === Combine Left and Right
        auto *mainLayout = new QHBoxLayout;
        mainLayout->addLayout(leftCol);
        mainLayout->addLayout(rightCol);

        // === Buttons
        auto *runBtn = new QPushButton("Run");
        auto *clearBtn = new QPushButton("Clear");
        auto *exitBtn = new QPushButton("Exit");

        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);

        auto *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(runBtn);
        btnLayout->addWidget(clearBtn);
        btnLayout->addWidget(exitBtn);

        // === Final Layout
        auto *layout = new QVBoxLayout;
        layout->addLayout(mainLayout);
        layout->addSpacing(10);
        layout->addLayout(btnLayout);

        setLayout(layout);
    }

private:
    QLineEdit *inputEdit, *outputEdit, *keyEdit;
};

#include "clefia_gui.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ClefiaGui window;
    window.show();
    return app.exec();
}