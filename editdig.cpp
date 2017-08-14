#include "editdig.hpp"
#include <qgraphicsview.h>
#include <QDirIterator>
#include <iostream>
editDig::editDig(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);
	dw = new QDesktopWidget();
	width = dw->geometry().width() / 2;
	height = dw->geometry().height() / 2;

	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setIconSize(QSize(200, 200));
	ui.listWidget->setResizeMode(QListWidget::Adjust);
	ui.listWidget->setSelectionMode(QListWidget::MultiSelection);

	//ui.tableWidget = new QTableWidget(this);
	//ui.tableWidget->setShowGrid(false);
	//ui.tableWidget->verticalHeader()->hide();
	//ui.tableWidget->horizontalHeader()->hide();
	//ui.tableWidget->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
	addImgItem();
	//	this->resize(width, height);
	this->show();

}

editDig::~editDig() {

}
void editDig::addImgItem() {
	QStringList strFilters;

	strFilters += "*.bmp";
	strFilters += "*.jpg";
	strFilters += "*.png";

	QString strDir("imgData");

	QDirIterator iterDir(strDir, strFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
	QListWidgetItem *list;
	while (iterDir.hasNext())
	{
		iterDir.next();
		list = new QListWidgetItem(QPixmap(iterDir.filePath()), "");
		list->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
		ui.listWidget->addItem(list);

	}
	

	/*int col = 0, row = 0, tem = 0;
	QDir dir = QDir("imgData");
	QTableWidgetItem *thumbnail;
	QString str2[10][5];
	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);

		if ((fileInfo.fileName().toStdString().compare(".") == 0) || (fileInfo.fileName().toStdString().compare("..") == 0))
			continue;

		if (fileInfo.isDir()) {
			QFileInfoList list2 = QDir(fileInfo.filePath()).entryInfoList();
			tem = 0;
			for (int j = 0; j < list2.size(); j++) {
				QFileInfo info2 = list2.at(j);
				if (info2.isFile())
				{
					str2[row][tem] = info2.filePath();
					tem++;
				}
			}
			row++;
			if (col < tem)
				col = tem;
		}

	}
	ui.tableWidget->setRowCount(row);
	ui.tableWidget->setColumnCount(col);

	for (int u = 0; u < row; u++)
		for (int t = 0; t < col; t++)
		{
			thumbnail = new QTableWidgetItem();
			thumbnail->setData(Qt::DecorationRole, QPixmap(str2[u][t]));
			ui.tableWidget->setItem(u, t, thumbnail);

		}


	for (int u = 0; u < row; u++)
		ui.tableWidget->setRowHeight(u, (height / row));

	for (int u = 0; u < row; u++)
		ui.tableWidget->setColumnWidth(u, (width / col));
*/
}

void editDig::checkItems(QListWidgetItem *item)
{
	QPixmap buf = item->icon().pixmap(QSize(32, 32));
	ui.label->setPixmap(buf);

	
//	if(item->isSelected())
	//std::cout << (item->text()).toStdString() << std::endl;
}

void editDig::enterItem(QTableWidgetItem * item) {
	QGraphicsScene  *sc = new  QGraphicsScene();
	QGraphicsView * grp = new QGraphicsView();
	grp->setScene(sc);
	QPixmap im = item->data(Qt::DecorationRole).value<QPixmap>();
	sc->addPixmap(im);
	grp->show();



}