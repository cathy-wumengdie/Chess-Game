#ifndef ENUM_H
#define ENUM_H

namespace Color {
typedef int Color;
enum { White = 0, Black };
}  // namespace Color

namespace Mode {
typedef int Mode;
enum { Menu = 0, Setup, Playing, End };
}  // namespace Mode

namespace Piece {
typedef int Id;
enum { Pawn = 0, Rook, Knight, Bishop, Queen, King };
}  // namespace Piece

namespace Points {
typedef int Points;
enum { Pawn = 1, Knight = 3, Bishop = 3, Rook = 5, Queen = 9, King = 0 };
}  // namespace Points

namespace SideEffect {
typedef int SideEffect;
enum { Castle = 0, Enpassant, Capture, None };
}  // namespace SideEffect

#endif