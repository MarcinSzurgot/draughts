#include "BoardView.hpp"

#include "SwitchException.hpp"

namespace
{

const auto blackTileColor     = sf::Color(80, 80, 0);
const auto blackPieceColor    = sf::Color(0, 0, 0);
const auto whiteTileColor     = sf::Color(220, 220, 0);
const auto whitePieceColor    = sf::Color(220, 220, 255);
const auto moveColor          = sf::Color(0, 255, 0);
const auto jumpColor          = sf::Color(255, 0, 0);
const auto promotedPieceColor = sf::Color(255, 255, 0);
const auto noPieceColor       = sf::Color(0, 0, 0, 0);

sf::Color tileColor(int col, int row)
{
    return (col + row) % 2 == 1 ? blackTileColor : whiteTileColor;
}

int index(sf::Vector2i size, sf::Vector2i position)
{
    return size.y * position.y + position.x;
}

void resetTiles(std::vector<sf::RectangleShape>& tiles, sf::Vector2i size)
{
    for (auto row = 0; row < size.y; ++row)
    {
        for (auto col = 0; col < size.x; ++col)
        {
            auto& tile = tiles[index(size, {col, row})];
            tile.setFillColor(tileColor(col, row));
            tile.setOutlineColor(tile.getFillColor());
        }
    }
}

void update(sf::CircleShape& pieceView, const Tile& piece)
{
    if (piece)
    {
        pieceView.setOutlineColor([&]{
            switch (piece->pieceColor)
            {
                case PieceColor::Black: return blackPieceColor;
                case PieceColor::White: return whitePieceColor;
            }
            throw SwitchException("PieceColor", piece->pieceColor);
        } ());
        pieceView.setFillColor([&]{
            switch (piece->pieceClass)
            {
                case PieceClass::Man:  return pieceView.getOutlineColor();
                case PieceClass::King: return promotedPieceColor;
            }
            throw SwitchException("PieceClass", piece->pieceClass);
        } ());
    }
    else
    {
        pieceView.setFillColor(noPieceColor);
        pieceView.setOutlineColor(noPieceColor);
    }
}

}

BoardView::BoardView(sf::Vector2i boardSize, sf::Vector2f viewSize)
: size_(boardSize)
{
    tiles_.reserve(size_.x * size_.y);
    pieces_.reserve(tiles_.capacity());

    const auto tileSize = [&]{
        const auto tx = viewSize.x / boardSize.x;
        const auto ty = viewSize.y / boardSize.y;
        return tx * boardSize.y <= viewSize.y ? tx : ty;
    } ();

    const auto tileOutlineRatio = 0.05f;
    const auto tileOutlineThick = tileOutlineRatio * tileSize;

    const auto pieceToTileRatio = 0.9f;
    const auto pieceRadius = tileSize / 2.0f;
    const auto pieceOutlineRatio = 0.3f;
    const auto pieceOutlineThick = pieceRadius * pieceOutlineRatio;

    for (auto row = 0; row < size_.y; ++row)
    {
        for (auto col = 0; col < size_.x; ++col)
        {
            auto tile = sf::RectangleShape();
            tile.setSize({tileSize, tileSize});
            tile.setOutlineThickness(-tileOutlineThick);
            tile.setPosition(col * tileSize, row * tileSize);
            tiles_.push_back(tile);

            auto piece = sf::CircleShape();
            piece.setRadius(pieceRadius);
            piece.setOrigin(pieceRadius, pieceRadius);
            piece.setOutlineThickness(-pieceOutlineThick);
            piece.setPosition({tile.getPosition().x + tileSize / 2, tile.getPosition().y + tileSize / 2});
            piece.setScale(pieceToTileRatio, pieceToTileRatio);
            piece.setFillColor(noPieceColor);
            piece.setOutlineColor(noPieceColor);
            pieces_.push_back(piece);
        }
    }

    resetTiles(tiles_, size_);
}

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const auto& tile : tiles_)
    {
        target.draw(tile, states);
    }

    for (const auto& piece : pieces_)
    {
        target.draw(piece, states);
    }
}

void BoardView::update(const Board& board)
{
    for (auto row = 0; row < board.size().y; ++row)
    {
        for (auto col = 0; col < board.size().x; ++col)
        {
            ::update(pieces_[index(size_, {col, row})], board[{col, row}]);
        }
    }
}


void BoardView::drawMoves(std::span<const Move> moves)
{
    resetTiles(tiles_, size_);

    for (const auto& move : moves)
    {
        const auto indexFrom = index(size_, move.from);
        const auto indexTo   = index(size_, move.to);

        tiles_[indexFrom].setFillColor(moveColor);
        tiles_[indexFrom].setOutlineColor(moveColor);
        tiles_[indexTo].setOutlineColor(moveColor);

        if (move.jump)
        {
            const auto indexJump = index(size_, *move.jump);
            tiles_[indexJump].setOutlineColor(jumpColor);
        }
    }
}

void BoardView::drawMovablePieces(std::span<const Move> moves)
{
    resetTiles(tiles_, size_);
    if (empty(moves))
    {
        return;
    }

    const auto markColor = moves.front().jump ? jumpColor : moveColor;
    for (const auto& move : moves)
    {
        tiles_[index(size_, move.from)].setOutlineColor(markColor);
    }
}

std::optional<sf::Vector2i> BoardView::tile(sf::Vector2f mousePos) const
{
    const auto localMousePos = getInverseTransform().transformPoint(mousePos);
    const auto tileSize = tiles_.front().getSize().x;
    const auto position = sf::Vector2i(static_cast<int>(localMousePos.x / tileSize),
                                       static_cast<int>(localMousePos.y / tileSize));
    if (position.x >= 0
        && position.x < size_.x
        && position.y >= 0
        && position.y < size_.y)
    {
        return position;
    }
    return std::nullopt;
}