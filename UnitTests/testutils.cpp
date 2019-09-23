#include "testutils.h"

#include <QDir>
#include <QFileInfo>

TestUtils::TestUtils()
{

}

QSet<QString> TestUtils::getTree(const QString &path, int limit, int depch) {
    QFileInfo info(path);

    if (limit > 0 && depch >= limit) {
        return {};
    }

    if (!info.exists()) {
        return {};
    }

    auto result = QSet<QString>{};

    if (!info.isDir()) {
        result.insert(getFilePath(info.absoluteFilePath()));
        return result;
    }

    QDir dir(info.absoluteFilePath());
    auto list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (auto &i: list) {
        result.unite(getTree(i.absoluteFilePath(), limit, depch + 1));
    }

    return result;
}

QString TestUtils::getFilePath(const QString& i) {
    auto file = QFileInfo(i).absoluteFilePath();
    auto delimiter = file.lastIndexOf("/");
    auto shared = file.indexOf(".so.", Qt::CaseInsensitive);

    if (shared > delimiter) {
        file = file.left(shared);
        file.push_back(".so");
    }

    return file;
}

QSet<QString> TestUtils::createTree(const QStringList &tree) {
    QSet<QString> res;
    for (auto &i : tree) {
        res.insert(getFilePath(i));
    }

    return res;
}

QHash<QString, int> TestUtils::compareTree(const QSet<QString> &leftTree, const QSet<QString> &rightTree) {
    QHash<QString, int> result;

    auto valuel = leftTree - rightTree;
    auto valuer = rightTree - leftTree;

    for(auto &i :valuel) {
        result.insert(i, 1);
    }

    for(auto &i :valuer) {
        result.insert(i, -1);
    }

    return result;
}

