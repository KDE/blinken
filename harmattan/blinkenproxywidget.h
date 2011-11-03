#ifndef BLINKENPROXYWIDGET_H
#define BLINKENPROXYWIDGET_H

#include "blinken.h"

#include <QGraphicsProxyWidget>

#ifdef HARMATTAN_BUILD
class BlinkenProxyWidget : public QGraphicsProxyWidget
{
Q_OBJECT
	public:
		BlinkenProxyWidget(QGraphicsItem* parent = 0);

		Q_INVOKABLE void setHighScore(const QString &name, int level, int score);

	signals:
		 void askForName(const QString &lastName, int level, int score);
		 void showHighScoreDialog(int level);
		 void showAbout();

private:
	blinken *widget;
};
#endif

#endif // BLINKENPROXYWIDGET_H
