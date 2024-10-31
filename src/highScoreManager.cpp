/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "highScoreManager.h"

#include <KConfig>
#include <KSharedConfig>

#include "settings.h"

static QSet<HighScoreManager *> s_allHSM;

/* HighScoreManager */

HighScoreManager::HighScoreManager()
{
	s_allHSM << this;
	update();
}

HighScoreManager::~HighScoreManager()
{
	s_allHSM.remove(this);
}

bool HighScoreManager::scoreGoodEnough(int level, int score)
{
	level--;
	QList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) ++it;
	
	return (it != itEnd);
}

void HighScoreManager::addScore(int level, int score, const QString &name)
{
	level--;
	QList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) ++it;
	
	if (it != itEnd)
	{
		m_scores[level].insert(it, qMakePair(score, name));
		m_scores[level].erase(--m_scores[level].end());
		
		KConfigGroup cfg(KSharedConfig::openConfig(), QStringLiteral("Level%1").arg(level + 1));
		int j;
		for (it = m_scores[level].begin(), j = 1; it != m_scores[level].end(); ++it, j++)
		{
			cfg.writeEntry(QStringLiteral("Score%1").arg(j), (*it).first);
			cfg.writeEntry(QStringLiteral("Name%1").arg(j), (*it).second);
		}
		cfg.sync();

		for (HighScoreManager *hsm : std::as_const(s_allHSM))
		{
			if (hsm != this)
			{
				hsm->update();
			}
		}
	}
}

void HighScoreManager::update()
{
	for (int i = 0; i < 3; ++i)
	{
		m_scores[i].clear();
	}
	for (int i = 1; i <= 3; i++)
	{
		KConfigGroup cfg(KSharedConfig::openConfig(), QStringLiteral("Level%1").arg(i));
		for (int j = 1; j <= 5; j++)
		{
			m_scores[i-1].append(qMakePair(cfg.readEntry(QStringLiteral("Score%1").arg(j),QVariant(0)).toInt(),cfg.readEntry(QStringLiteral("Name%1").arg(j),QString())));
		}
	}
}

QList< QPair<int, QString> > HighScoreManager::scores(int level) const
{
	return m_scores[level];
}

int HighScoreManager::score(int level, int position) const
{
	return m_scores[level][position].first;
}

QString HighScoreManager::name(int level, int position) const
{
	return m_scores[level][position].second;
}

#include "moc_highScoreManager.cpp"
