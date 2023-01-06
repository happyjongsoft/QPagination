#include "qpagination.h"

#include <QPainter>
#include <QStyleOption>
#include <QSizePolicy>

QPagination::QPagination(QWidget *parent, int height) : QWidget(parent),
    height(height) {

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(6, 0, 6, 0);
    layout->setSpacing(0);

    buttonPrevious = new QPushButton(this);
    buttonNext = new QPushButton(this);

    setMinimumHeight(height);

    previousPage = -1;
    currentPage = 1;
    totalButtons = 7;
}

int QPagination::getCurrentPage() {
    return currentPage;
}

void QPagination::setButtonsSize(int width, int height) {
    buttonSize = QSize(width, height);
}

void QPagination::setButtonsSize(const QSize &size) {
    buttonSize = size;
}

void QPagination::setNextButton(const QPixmap &icon, const QString &text, const QSize &size) {
    buttonNext->setText(text);
    buttonNext->setIcon(icon);
    buttonNext->setMinimumSize(size);
    buttonNext->setMaximumSize(size);
}

void QPagination::setNextButton(const QString &text, const QSize &size) {
    setNextButton(QPixmap(), text, size);
}

void QPagination::setPreviousButton(const QPixmap &icon, const QString &text, const QSize &size) {
    buttonPrevious->setText(text);
    buttonPrevious->setIcon(icon);
    buttonPrevious->setMinimumSize(size);
    buttonPrevious->setMaximumSize(size);
}

void QPagination::setPreviousButton(const QString &text, const QSize &size) {
    setNextButton(QPixmap(), text, size);
}

void QPagination::setCurrentPage(int currentPage) {
    if (currentPage > totalPages) {
        currentPage = 1;
    }
    this->currentPage = currentPage;
}

void QPagination::setTotalPages(int totalPages) {
    this->totalPages = totalPages;
    currentPage = 1;
}

void QPagination::setStyleSheet(const QString &styleSheet) {
    this->styleSheet = styleSheet;
}

void QPagination::updateTotalPages(int totalPages) {
    if (this->totalPages == totalPages) {
        return;
    }

    this->totalPages = totalPages;
    if (currentPage > totalPages) {
        currentPage = 1;
    }
    previousPage = -1;
    totalButtons = 7;

    buttons.clear();

    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    buttonPrevious = new QPushButton(this);
    buttonNext = new QPushButton(this);

    show();
}

void QPagination::show() {

    QSpacerItem *horizontalSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addItem(horizontalSpacer1);

    buttonPrevious->setText("<");
    buttonPrevious->setEnabled(false);
    buttonPrevious->setVisible(false);
    connect(buttonPrevious, &QPushButton::clicked, [&](){
        if(currentPage > 1){
            previousPage = currentPage;
            currentPage -= 1;

            calculate();

            emit onPageChange(currentPage);
        }
    });
    applyStyleSheet(buttonPrevious);
    applySize(buttonPrevious);

    buttonNext->setText(">");
    buttonNext->setEnabled(false);
    buttonNext->setVisible(false);
    connect(buttonNext, &QPushButton::clicked, [&]() {
        if(currentPage < totalPages){
            previousPage = currentPage;
            currentPage += 1;

            calculate();

            emit onPageChange(currentPage);
        }
    });
    applyStyleSheet(buttonNext);
    applySize(buttonNext);

    if (totalPages > 1) {
        buttonPrevious->setVisible(true);
        layout->addWidget(buttonPrevious);
    }

    if (totalButtons > totalPages) {
        totalButtons = totalPages;
    }

    for (int i = 0; i < totalButtons; i++) {
        QPushButton *button = new QPushButton(this);
        button->setMinimumSize(24, 24);
        applyStyleSheet(button);
        applySize(button);

        int pageNumber = -1;
        if (totalPages >= totalButtons + 1) {
            if (i == 0) {
                button->setProperty("page", 1);
                pageNumber = 1;
            } else if (i == totalButtons - 1) {
                button->setProperty("page", totalPages);
                pageNumber = totalPages;
            } else {
                if (i <= 4) {
                    pageNumber = i + 1;
                }
            }
        } else {
            pageNumber = i + 1;
        }

        button->setProperty("page", pageNumber);
        if (pageNumber > 0) {
            button->setText(QString::number(pageNumber));
        } else {
            button->setText("...");
        }

        connect(button, &QPushButton::clicked, [this, button]() {
            QVariant variant = button->property("page");
            int pageNumber = variant.toInt();
            int newCurrentPage = currentPage;

            if (pageNumber > 0) {
                newCurrentPage = pageNumber;
            } else if (pageNumber == -1) {
                newCurrentPage = currentPage + 3;
            } else if (pageNumber == -2) {
                newCurrentPage = currentPage - 3;
            }

            if (newCurrentPage < 1) {
                newCurrentPage = 1;
            } else if (newCurrentPage > totalPages) {
                newCurrentPage = totalPages;
            }

            previousPage = currentPage;
            currentPage = newCurrentPage;

            calculate();

            emit onPageChange(currentPage);
        });
        layout->addWidget(button);
        buttons.append(button);
    }

    if (totalPages > 1) {
        buttonNext->setVisible(true);
        buttonNext->setEnabled(true);
        layout->addWidget(buttonNext);
    }
    QSpacerItem *horizontalSpacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addItem(horizontalSpacer2);

    calculate();
}

void QPagination::calculate() {

    if (totalPages > totalButtons) {
        bool button1 = false;
        bool button5 = false;
        if (currentPage - 1 > 3) {
            button1 = true;
            setButton(1, -2);
        } else {
            setButton(1, 2);
            setButton(2, 3);
            setButton(3, 4);
            setButton(4, 5);
        }

        if (totalPages - currentPage > 3) {
            button5 = true;
            setButton(5, -1);
        } else {
            setButton(2, totalPages - 4);
            setButton(3, totalPages - 3);
            setButton(4, totalPages - 2);
            setButton(5, totalPages - 1);
        }

        if (button1 && button5) {
            setButton(2, currentPage - 1);
            setButton(3, currentPage);
            setButton(4, currentPage + 1);
        }
    }

    for (QPushButton *button : buttons) {
        int page = button->property("page").toInt();
        if (currentPage == page) {
            button->setEnabled(false);
        } else {
            button->setEnabled(true);
        }
    }

    buttonPrevious->setEnabled(currentPage >= 1);
    buttonNext->setEnabled(currentPage <= totalPages);
}

void QPagination::paintEvent(QPaintEvent *) {
    QStyleOption styleOption;
    styleOption.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

void QPagination::applySize(QPushButton *button) {
    if (buttonSize.isValid()) {
        button->setMinimumSize(buttonSize);
        button->setMaximumSize(buttonSize);
    }
}

void QPagination::applyStyleSheet(QPushButton *button) {
    if (!styleSheet.isEmpty()) {
        button->setStyleSheet(styleSheet);
    }
}

void QPagination::setButton(int index, int pageNumber) {

    QString text;
    if (pageNumber > 0) {
        text = QString::number(pageNumber);
    } else {
        text = "...";
    }

    QPushButton *button = buttons.at(index);
    button->setProperty("page", pageNumber);
    button->setText(text);
}
