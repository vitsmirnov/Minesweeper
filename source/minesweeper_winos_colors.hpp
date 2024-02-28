#ifndef MINESWEEPER_WINOS_COLORS_HPP_INCLUDED
#define MINESWEEPER_WINOS_COLORS_HPP_INCLUDED


#include <windef.h>

#include "vs_win_colors.hpp"


using Color = COLORREF;

struct FigureColor {
public:
  Color frame{clBlack};//pen
  Color body{clWhite};//brush
};

struct CellColorSet { //rename to Skin? //GameFieldCellColorSet
public:
  FigureColor closed_cell{clBrightGreen, clGreen};
  FigureColor opened_cell{clBrightGreen, clOrange};
  FigureColor flag{clOrange, clLightRed};//mine_marked;
  FigureColor mine{clDarkGray, clGray};//has_mine
  Color number{clLightBlue};//font
};


constexpr CellColorSet BlackAndWhiteColorSet{
  {clBlack, clWhite},
  {clWhite, clBlack},
  {clWhite, clBlack},
  {clBlack, clWhite},
  clWhite
};


#endif // MINESWEEPER_WINOS_COLORS_HPP_INCLUDED
