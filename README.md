# Pagination
 a paging widget based on Qt

### Usage Demo
``` c++
QWidget *widget = new QWidget(this);
widget->setGeometry(0, 0, 560, 50);

QHBoxLayout *hbox = new QHBoxLayout(widget);

QPagination *pagination = new QPagination(widget);
hbox->addWidget(pagination);

QLabel *label = new QLabel(this);
label->setText(QString::number(pagination->getCurrentPage()));

connect(pagination, &QPagination::onPageChange, [=](int value) {
    label->setText(QString::number(value));
});

pagination->setStyleSheet("QPushButton { margin: 0 2px; color: #000; background: #fff; border: 1px solid #f00; border-radius: 5px; } QPushButton:disabled { background: #ccc; } QPushButton:hover { background:transparent; }");
pagination->setTotalPages(20);
pagination->setCurrentPage(17);
pagination->setButtonsSize(QSize(30, 30));
pagination->show();

pagination->updateTotalPages(10);
```
