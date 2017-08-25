#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>

struct Figure
{
  Logic::FigureColor color;
  Logic::FigureType type;
  int x;
  int y;
};

struct Logic::Impl
{
  QList<Figure> figures;

  int findByPosition(int x, int y);
};

int Logic::Impl::findByPosition(int x, int y) {
  for (int i(0); i<figures.size(); ++i) {
    if (figures[i].x != x || figures[i].y != y ) { 
      continue; 
    }
    return i;    
  }
  return -1;
}


Logic::Logic(QObject *parent):QAbstractListModel(parent), impl(new Impl())
{}

Logic::~Logic() {
}

int Logic::boardSize() const {
  return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const {
  return impl->figures.size(); 
}

QHash<int, QByteArray> Logic::roleNames() const { 
  QHash<int, QByteArray> names;
  names.insert(Roles::Color     , "color");
  names.insert(Roles::Type      , "type");
  names.insert(Roles::PositionX , "positionX");
  names.insert(Roles::PositionY , "positionY");
  return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
  if (!modelIndex.isValid()) {
    return QVariant();
  }
    
  int index = static_cast<int>(modelIndex.row());
  
  if (index >= impl->figures.size() || index < 0) {
    return QVariant();
  }

  Figure & figure = impl->figures[index];
    
  switch (role) {
    case Roles::Color    : return figure.color;
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
  }
  return QVariant(); 
}

void Logic::clear() {
  beginResetModel();
  impl->figures.clear();
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
}

bool Logic::move(int fromX, int fromY, int toX, int toY) {
  int index = impl->findByPosition(fromX, fromY);

  if (index < 0) return false;
  
  if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE || impl->findByPosition(toX, toY) >= 0) {
    return false;
  }
  impl->figures[index].x = toX;
  impl->figures[index].y = toY;
  QModelIndex topLeft = createIndex(index, 0);
  QModelIndex bottomRight = createIndex(index, 0);
  emit dataChanged(topLeft, bottomRight);
  return true;
}
