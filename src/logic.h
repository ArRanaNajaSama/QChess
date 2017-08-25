#pragma once
#include <memory>
#include <QAbstractListModel>

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

    enum FigureColor
    {
        WHITE,
        BLACK
    };

    enum FigureType
    {
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING,
        PAWN
    };

    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Color = Qt::UserRole,
        Type,
        PositionX,
        PositionY,
    };


    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT)
    int boardSize() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void startNewGame();
    Q_INVOKABLE void loadGame();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
