#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStack>

struct  Figure
{
    Logic::FigureColor color;
    Logic::FigureType type;
    int x;
    int y;
};

struct  Step
{
    int fromX;
    int fromY;
    int toX;
    int toY;
};

struct  Capture
{
    Figure figure;
    int turn;
};

struct  Logic::Impl
{
    QList<Figure> figures;
    QStack<Capture> captures;
    QVector<Step> steps;

    int findByPosition(int x, int y);
    int turn = 0;
    bool checkMove(Figure figure,int fromX, int fromY, int toX, int toY);
    bool checkPawnMove(int color, int fromX, int fromY, int toX, int toY);
    bool checkOverleap(int fromX, int fromY, int toX, int toY);
    bool checkRookMove(int fromX, int fromY, int toX, int toY);
    bool checkKnightMove(int fromX, int fromY, int toX, int toY);
    bool checkBishopMove(int fromX, int fromY, int toX, int toY);
    bool checkQueenMove(int fromX, int fromY, int toX, int toY);
    bool checkKingMove(int fromX, int fromY, int toX, int toY);
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

bool::Logic::Impl::checkOverleap(int fromX, int fromY, int toX, int toY)
{
    int x = (toX - fromX == 0) ? 0 : (toX - fromX) / abs(toX - fromX);
    int y = (toY - fromY == 0) ? 0 : (toY - fromY) / abs(toY - fromY);
    int max = abs(toX - fromX) > abs(toY - fromY) ? abs(toX - fromX) : abs(toY - fromY);
    int index;
    for(int i = 1; i < max; i++)
    {
        index = findByPosition(fromX + i * x, fromY + i * y);
        if(index >= 0)
            return true;
    }
    return false;
}

bool::Logic::Impl::checkMove(Figure figure,int fromX, int fromY, int toX, int toY)
{
    qDebug() << "checkMove: Current figure" << figure.type;
    switch (figure.type)
    {
    case ROOK:
        return(checkRookMove(fromX, fromY, toX, toY));
        break;
    case KNIGHT:
        return (checkKnightMove(fromX, fromY, toX, toY));
        break;
    case BISHOP:
        return (checkBishopMove(fromX, fromY, toX, toY));
        break;
    case QUEEN:
        return (checkQueenMove(fromX, fromY, toX, toY));
        break;
    case KING:
        return (checkKingMove(fromX, fromY, toX, toY));
        break;
    case PAWN:
        return(checkPawnMove(figure.color, fromX, fromY, toX, toY));
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

bool::Logic::Impl::checkRookMove(int fromX, int fromY, int toX, int toY)
{
    bool overleap = checkOverleap(fromX, fromY, toX, toY);
    if (abs(fromY - toY) < 1 && !overleap)
        return true;
    else if (abs(fromX - toX) < 1 && !overleap)
        return true;
    return false;
}

bool::Logic::Impl::checkKnightMove(int fromX, int fromY, int toX, int toY)
{
    //The knight moves forms an "L"-shape:
    //two squares vertically and one square horizontally,
    //or two squares horizontally and one square vertically.
    //The knight is the only piece that can leap over other pieces.
    if (abs(fromX - toX) == 2 && abs(fromY - toY) == 1)
        return true;
    if (abs(fromX - toX) == 1 && abs(fromY - toY) == 2)
        return true;
    return false;
}

bool::Logic::Impl::checkBishopMove(int fromX, int fromY, int toX, int toY)
{
    //The bishop can move any number of squares diagonally,
    //but cannot leap over other pieces.
    bool overleap = checkOverleap(fromX, fromY, toX, toY);
    if((abs(fromX - toX) == abs(fromY - toY)) && !overleap)
        return true;
    return false;
}

bool::Logic::Impl::checkQueenMove(int fromX, int fromY, int toX, int toY)
{
    bool overleap = checkOverleap(fromX, fromY, toX, toY);
    //Queen's diagonal move
    if((abs(fromX - toX) == abs(fromY - toY)) && !overleap)
        return true;
    else if ((abs(fromY - toY) < 1) && !overleap) // Horizontal move
        return true;
    else if ((abs(fromX - toX) < 1) && !overleap) // Vertical move
        return true;
    return false;
}

bool::Logic::Impl::checkKingMove(int fromX, int fromY, int toX, int toY)
{
    //The king moves one square in any direction.
    if(abs(fromX - toX) <= 1 && abs(fromY - toY) <= 1)
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

QString Logic::getWhoseTurn() const
{
    return m_whoseTurn;
}

void    Logic::setWhoseTurn(const QString &a)
{
    if (a != m_whoseTurn)
        m_whoseTurn = a;
    emit whoseTurnChanged();
}

bool    Logic::getMouse() const
{
    return m_mouse;
}

void    Logic::setMouse(const bool &m)
{
    if (m != m_mouse)
        m_mouse = m;
    emit mouseChanged();
}

bool    Logic::getEndGame() const
{
    return m_endGame;
}

void    Logic::setEndGame(const bool &m)
{
    if (m != m_endGame)
        m_endGame = m;
    emit endGameChanged();
}

bool Logic::getTurnAnnoun() const
{
    return m_turnAnnoun;
}

void Logic::setTurnAnnoun(const bool &m)
{
    if (m != m_turnAnnoun)
        m_turnAnnoun = m;
    emit turnAnnounChanged();
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
    endResetModel();
    impl->steps.clear();
    impl->turn = 0;
    setMouse(false);
    setTurnAnnoun(false);
}

void Logic::startNewGame()
{
    clear();
    setMouse(true);
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
    setTurnAnnoun(true);
    setPlayerTurn();
}

void Logic::loadGame()
{
    clear();
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
    setTurnAnnoun(true);
    setPlayerTurn();

    QFile file("out.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Load Game: Something went wrong....\n";
        return;
    }
    QTextStream in(&file);
    int fromX;
    int fromY;
    int toX;
    int toY;
    while (!in.atEnd())
    {
        in >> fromX;
        in >> fromY;
        in >> toX;
        in >> toY;
        impl->steps << Step{fromX, fromY, toX, toY};
    }
    file.close();
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

void Logic::nextMove()
{
    if(impl->turn >= (impl->steps.size() - 1))
        return ;
    changePosition(impl->steps[impl->turn].fromX,impl->steps[impl->turn].fromY,impl->steps[impl->turn].toX,impl->steps[impl->turn].toY);
    impl->turn++;
}

void Logic::prevMove()
{
    setEndGame(false);
    if(impl->turn > 0)
    {
        if(impl->captures.size() > 0)
        {
            if(impl->captures[(impl->captures.size() - 1)].turn == (impl->turn - 1))
            {
                beginInsertRows(QModelIndex(), impl->figures.size(), impl->figures.size());
                impl->figures << impl->captures.pop().figure;
                endInsertRows();
            }
        }
        changePosition(impl->steps[impl->turn - 1].toX, impl->steps[impl->turn - 1].toY,impl->steps[impl->turn - 1].fromX, impl->steps[impl->turn - 1].fromY);
        impl->turn--;
    }
}

bool Logic::move(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    if (index < 0)
        return false;
    if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE)
        return false;
    if ((impl->turn % 2) != impl->figures[index].color)
        return false;
    int  nextIndex = impl->findByPosition(toX, toY);
    if (index == nextIndex)
        return false;
    bool moveAvailible = impl->checkMove(impl->figures[index], fromX, fromY, toX, toY);
    if (!moveAvailible)
    {
        qDebug() << "Unavailible move! Try again.";
        return false;
    }
    if (nextIndex >= 0 && moveAvailible) // Capture figures
    {
        if (impl->figures[index].color == impl->figures[nextIndex].color)
            return false;
        impl->captures.push(Capture{impl->figures[nextIndex], impl->turn});
        if (impl->figures[nextIndex].type == KING)
            gameOver();
        beginRemoveRows(QModelIndex(), nextIndex, nextIndex);
        impl->figures.removeAt(nextIndex);
        endRemoveRows();
        if(nextIndex < index)
            --index;
    }
    impl->steps << Step{fromX, fromY, toX, toY};
    impl->turn++;
    setPlayerTurn();
    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    return true;
}

void Logic::changePosition(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int nextIndex = impl->findByPosition(toX, toY);
    if (nextIndex >= 0)
    {
        impl->captures.push(Capture{impl->figures[nextIndex], impl->turn});
        if (impl->figures[nextIndex].type == KING)
            gameOver();
        beginRemoveRows(QModelIndex(), nextIndex, nextIndex);
        impl->figures.removeAt(nextIndex);
        endRemoveRows();
    }
    setPlayerTurn();
    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
}

void Logic::setPlayerTurn()
{
    if(impl->turn % 2)
        setWhoseTurn("Black player it is your turn.");
    else
        setWhoseTurn("White player it is your turn.");
}

void Logic::gameOver()
{
    setTurnAnnoun(false);
    setMouse(false);
    setEndGame(true);
}
