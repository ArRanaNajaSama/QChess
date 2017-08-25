import QtQuick 2.0

function getImgPath(type, piece)
{
    var imgPath = ""

    switch (type) {
        case Figure.FIGURE_BLACK: imgPath += "/images/black"; break;
        case Figure.FIGURE_WHITE:
            default:
        imgPath += "/images/white"; break;
    }

        switch (piece) {
            case Figure.FIGURE_KING: imgPath += "_king.svg"; break;
            case Figure.FIGURE_QUEEN: imgPath += "_queen.svg"; break;
            case Figure.FIGURE_ROOK: imgPath += "_rook.svg"; break;
            case Figure.FIGURE_KNIGHT: imgPath += "_knight.svg"; break;
            case Figure.FIGURE_BISHOP: imgPath += "_bishop.svg"; break;
            case Figure.FIGURE_PAWN:
                default:
            imgPath += "_pawn.svg"; break;
        }

            return imgPath
        }

