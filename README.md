# Pagination
 A Pagination Widget based on Qt

### Usage Demo
``` c++
    QWidget *widget = new QWidget(this);
    widget->setGeometry(0, 0, 560, 50);

    QVBoxLayout *vbox = new QVBoxLayout(widget);

    setCentralWidget(widget);

    QLabel *label = new QLabel(this);
    vbox->addWidget(label);
    label->setStyleSheet("font-size: 50px;\
                         font-weight: 75;\
                         background:transparent;\
                         color:#15cfbb;");
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QPagination *pagination = new QPagination(widget);
    vbox->addWidget(pagination);

    connect(pagination, &QPagination::onPageChange, [=](int value) {
        label->setText(QString::number(value) + " of " + QString::number(pagination->getTotalPages()));
    });

    pagination->setStyleSheet("QPushButton{\
                                margin: 0 2px;\
                                font-size: 20px;\
                                font-weight: 50;\
                                background:#777;\
                                color:white;\
                                border-radius:4px;\
                            }\
                            QPushButton:disabled{\
                                background: #15cfbb;\
                                border: 4px solid #fff;\
                                font-size: 26px;\
                                color:black;\
                                font-weight: 75;\
                            }\
                            QPushButton:hover{\
                                background:#ddd;\
                                color:black;\
                                border: 4px solid #fff;\
                            }\
                            QPushButton:pressed{\
                                color: #15cfbb;\
                                background:#fff;\
                            }\
                            ");
    pagination->setTotalPages(100);
    pagination->setCurrentPage(50);
    pagination->setButtonsSize(QSize(64, 64));
    pagination->show();

    label->setText(QString::number(pagination->getCurrentPage()) + " of " + QString::number(pagination->getTotalPages()));
```
