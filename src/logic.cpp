#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDebug>

struct Figure
{
    Logic::FigureColor color;
    Logic::FigureType type;
    int x;
    int y;
};

struct Step
{
    int fromX;
    int fromY;
    int toX;
    int toY;
};

struct Logic::Impl
{
    QList<Figure> figures;
    QVector<Step> steps;

    int findByPosition(int x, int y);
    int turn = 0;
    bool checkMove(Figure figure,int fromX, int fromY, int toX, int toY);
    bool checkPawnMove(int color, int fromX, int fromY, int toX, int toY);
};

int Logic::Impl::findByPosition(int x, int y)
{
    for (int i(0); i<figures.size(); ++i)
    {
        if (figures[i].x != x || figures[i].y != y )
        {
            continue;
        }
        return i;
    }
    return -1;
}

bool::Logic::Impl::checkMove(Figure figure,int fromX, int fromY, int toX, int toY)
{
    qDebug() << "checkMove: Current figure" << figure.type;
    switch (figure.type)
    {
    case PAWN:
        qDebug() << checkPawnMove(figure.color, fromX, fromY, toX, toY);
        return(checkPawnMove(figure.color, fromX, fromY, toX, toY));
        break;
    case ROOK:
        break;
    default:
        return false;
        break;
    }
    return false;
}

bool::Logic::Impl::checkPawnMove(int color, int fromX, int fromY, int toX, int toY)
{
    int step = ((color == WHITE) ? -1 : 1);
    int nextIndex = findByPosition(toX, toY);
    //The pawn can move forward to the unoccupied square immediately in front of it on the same file
    if (((fromY - toY) == step) && (!(nextIndex >= 0)) && ((fromX - toX) == 0))
        return true;
    //or on its first move it can advance two squares along the same file,
    if (((fromY == 1 && (color == WHITE)) || (fromY == 6 && (color == BLACK)))
            && ((fromY - toY) == (2 * step)))
        return true;
    //the pawn can capture an opponent's piece on a square diagonally in front of it on an adjacent file,
    if ((fromY - toY) && abs(fromX - toX) == abs(step) && (nextIndex >= 0))
        return true;
    return false;
}

Logic::Logic(QObject *parent):QAbstractListModel(parent), impl(new Impl())
{}

Logic::~Logic()
{}

int Logic::boardSize() const
{
    return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const
{
    return impl->figures.size();
}

QHash<int, QByteArray> Logic::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(Roles::Color     , "color");
    names.insert(Roles::Type      , "type");
    names.insert(Roles::PositionX , "positionX");
    names.insert(Roles::PositionY , "positionY");
    return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const
{
    if (!modelIndex.isValid())
    {
        return QVariant();
    }
    
    int index = static_cast<int>(modelIndex.row());

    if (index >= impl->figures.size() || index < 0)
    {
        return QVariant();
    }

    Figure & figure = impl->figures[index];
    
    switch (role)
    {
    case Roles::Color    : return figure.color;
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
    }
    return QVariant();
}

void Logic::clear()
{
    beginResetModel();
    impl->figures.clear();
    impl->steps.clear();
    impl->turn = 0;
    endResetModel();
}

void Logic::startNewGame()
{
    beginInsertRows(QModelIndex(), 0, 31);
    for (int i = 0; i < 8; i++)
    {
        impl->figures << Figure{WHITE, PAWN, i, 1 };
        impl->figures << Figure{BLACK, PAWN, i, 6 };
    }
    impl->figures << Figure{WHITE, ROOK, 0, 0};
    impl->figures << Figure{WHITE, KNIGHT, 1, 0};
    impl->figures << Figure{WHITE, BISHOP, 2, 0};
    impl->figures << Figure{WHITE, QUEEN, 3, 0};
    impl->figures << Figure{WHITE, KING, 4, 0};
    impl->figures << Figure{WHITE, BISHOP, 5, 0};
    impl->figures << Figure{WHITE, KNIGHT, 6, 0};
    impl->figures << Figure{WHITE, ROOK, 7, 0};
    impl->figures << Figure{BLACK, ROOK, 0, 7};
    impl->figures << Figure{BLACK, KNIGHT, 1, 7};
    impl->figures << Figure{BLACK, BISHOP, 2, 7};
    impl->figures << Figure{BLACK, QUEEN, 3, 7};
    impl->figures << Figure{BLACK, KING, 4, 7};
    impl->figures << Figure{BLACK, BISHOP, 5, 7};
    impl->figures << Figure{BLACK, KNIGHT, 6, 7};
    impl->figures << Figure{BLACK, ROOK, 7, 7};
    endInsertRows();
}

void Logic::loadGame()
{
    beginInsertRows(QModelIndex(), 0, 31);
    for (int i = 0; i < 8; i++)
    {
        impl->figures << Figure{WHITE, PAWN, i, 1 };
        impl->figures << Figure{BLACK, PAWN, i, 6 };
    }
    impl->figures << Figure{WHITE, ROOK, 0, 0};
    impl->figures << Figure{WHITE, KNIGHT, 1, 0};
    impl->figures << Figure{WHITE, BISHOP, 2, 0};
    impl->figures << Figure{WHITE, QUEEN, 3, 0};
    impl->figures << Figure{WHITE, KING, 4, 0};
    impl->figures << Figure{WHITE, BISHOP, 5, 0};
    impl->figures << Figure{WHITE, KNIGHT, 6, 0};
    impl->figures << Figure{WHITE, ROOK, 7, 0};
    impl->figures << Figure{BLACK, ROOK, 0, 7};
    impl->figures << Figure{BLACK, KNIGHT, 1, 7};
    impl->figures << Figure{BLACK, BISHOP, 2, 7};
    impl->figures << Figure{BLACK, QUEEN, 3, 7};
    impl->figures << Figure{BLACK, KING, 4, 7};
    impl->figures << Figure{BLACK, BISHOP, 5, 7};
    impl->figures << Figure{BLACK, KNIGHT, 6, 7};
    impl->figures << Figure{BLACK, ROOK, 7, 7};
    endInsertRows();

    QFile file("out.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Load Game: Something went wrong....\n";
        return;
    }
    QTextStream in(&file);
    //       QString line = in.readLine();
    //       while (!line.isNull())
    //       {
    //           process_line(line);
    //           line = in.readLine();
    //       }
}

void Logic::saveGame()
{
    QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Save Game: Something went wrong....\n";
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < impl->steps.size(); i++)
    {
        out << impl->steps[i].fromX <<" "<<impl->steps[i].fromY<<" "
            <<impl->steps[i].toX<<" "<<impl->steps[i].toY<<" ";
    }
    file.close();
}

bool Logic::move(int fromX, int fromY, int toX, int toY)
{
    qDebug() << "fromX" << fromX;
    qDebug() << "fromY" << fromY;
    qDebug() << "toX" << toX;
    qDebug() << "toY" << toY;
    int index = impl->findByPosition(fromX, fromY);
    qDebug() << "Index" << index;
    qDebug() << "Current figure" << impl->figures[index].type;
    if (index < 0)
        return false;
    if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE)
        return false;
    if ((impl->turn % 2) != impl->figures[index].color)
        return false;
    int  nextIndex = impl->findByPosition(toX, toY);
    qDebug() << "nextIndex" << nextIndex;
    bool moveAvailible = impl->checkMove(impl->figures[index], fromX, fromY, toX, toY);
    if (!moveAvailible)
    {
        qDebug() << "Unavailible move! Try again.";
        return false;
    }
    qDebug() << "moveAvailible" << moveAvailible;
    if (nextIndex >= 0)
    {
        if (impl->figures[index].color == impl->figures[nextIndex].color)
            return false;
        qDebug() << "attack!";
        impl->figures.removeAt(nextIndex);
        beginRemoveRows(QModelIndex(), nextIndex, nextIndex);
        endRemoveRows();
        if (nextIndex < index)
        {
            --index;
        }
    }
    impl->steps << Step {fromX, fromY, toX, toY};
    impl->turn++;
    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    return true;
}
