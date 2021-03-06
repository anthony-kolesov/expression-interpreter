/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QProcess>

#include "mainwindow.h"

//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupFileMenu();
    setupHelpMenu();

    menuBar()->addAction(tr("&Run"), this, SLOT(run()));

    this->verticalWidget = new QWidget(this);

    // Setup vbox layout
    this->vboxLayout = new QVBoxLayout(this->verticalWidget);
    this->verticalWidget->setLayout(this->vboxLayout);

    // Editor
    setupEditor(this->verticalWidget);
    this->vboxLayout->addWidget(this->editor);

    // errmsg
    this->errWindow = new QPlainTextEdit(this->verticalWidget);
    this->errWindow->setReadOnly(true);
    this->vboxLayout->addWidget(this->errWindow);

    setCentralWidget(this->verticalWidget);
    setWindowTitle(tr("Syntax Highlighter"));
}
//! [0]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
                tr("<p>The <b>Syntax Highlighter</b> example shows how " \
                   "to perform simple syntax highlighting by subclassing " \
                   "the QSyntaxHighlighter class and describing " \
                   "highlighting rules using regular expressions.</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Any file(*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
            editor->setPlainText(file.readAll());
    }
}

//! [1]
void MainWindow::setupEditor(QWidget *parent)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit(parent);
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());

    QFile file("mainwindow.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}
//! [1]

void MainWindow::showErrors(const QString &err) {
    auto errors = err.split("\n", QString::SplitBehavior::SkipEmptyParts);

    QTextDocument *doc = editor->document();

    QTextCharFormat errFormat;
    errFormat.setUnderlineColor(QColor(Qt::red));
    errFormat.setUnderlineStyle(QTextCharFormat::UnderlineStyle::SpellCheckUnderline);

    // Clear previous errors.
    for (auto block = doc->begin(); !block.isValid(); block = block.next()) {
        delete block.userData();
        block.setUserData(nullptr);
    }

    for (auto &&e : errors) {
        auto errorParts = e.split(":");
        auto posInfo = errorParts[1].split("-");
        QString beginInfo = posInfo[0];
        QString endInfo = posInfo.size() > 1 ? posInfo[1] : "";
        int beginLine = beginInfo.split(",")[0].toInt();
        int beginCol = beginInfo.split(",")[1].toInt();
        int endCol = beginCol;
        if (posInfo.size() > 1) {
            endCol = posInfo[1].split(",")[1].toInt();
        }

        auto data = new ErrorData(beginLine, beginCol, beginLine, endCol);
        doc->findBlockByLineNumber(beginLine - 1).setUserData(data);
    }

    this->highlighter->rehighlight();
    this->editor->repaint();
}

void MainWindow::run()
{
    auto process = new QProcess(this);
    QString exeFile = QFileInfo( QCoreApplication::applicationFilePath() ).dir().absolutePath()
                + "/../../interpreter/interpreter.exe";
    process->start(exeFile);

    if (!process->waitForStarted()) {
        throw std::invalid_argument("not started");
    }

    auto programTextString = this->editor->toPlainText().toStdString();
    process->write(programTextString.c_str());
    process->closeWriteChannel();

    if (!process->waitForFinished()) {
        throw std::invalid_argument("not finished");
    }

    auto resultData = process->readAll();
    auto stderrOutput = QString(process->readAllStandardError());
    auto resultString = QString(resultData) + stderrOutput;
    this->errWindow->setPlainText(resultString);

    this->showErrors(stderrOutput);
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}
