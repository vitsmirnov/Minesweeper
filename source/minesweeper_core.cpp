/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#include "minesweeper_core.hpp"

#include <cstdlib>
#include <ctime>
//#include <random>

//#include "vs_math.h"


namespace minesweeper_game_core // (Core | Engine) MSGE | MGE
{

  /// const ///

  // Where do constexprs have to be (hpp or cpp)?
  /*constexpr Size GameField::DefaultFieldSize;// = {10, 10};
  constexpr Size GameField::MinFieldSize;// = {4, 4};//{3, 3};
  constexpr Size GameField::MaxFieldSize;// = {1000, 1000};
  constexpr Cell GameField::DefaultCell;// = {false, csClosed};

  constexpr Quantity GameEngine::DefaultMinesCount;// = 10;//auto??
  constexpr Quantity GameEngine::MinFreeCells;// = 9;
  constexpr Quantity GameEngine::MinMinesCount;// = 1;//*/

  /*constexpr std::array<LevelData, 3> DefaultLevels2 {
    LevelData{{9, 9}, 10},
    LevelData{{16, 16}, 40},
    LevelData{{30, 16}, 99}
  };//*/


  /// class GameField ///

  bool GameField::DoWithAround(const Point& p, std::function<void(const Point&)> f)// const;
  {
    if (!DoesPosExist(p) || f == nullptr)
      return false;

    Rect r = GetRoundRect(p);
    for (auto i = r.left; i <= r.right; ++i)
      for (auto j = r.top; j <= r.bottom; ++j)
        if (p.x != i || p.y != j)
          f({i, j});
    return true;
  }


  /// class GameEngine ///

  void GameEngine::NewGame()
  {
    // Don't do check for (state == GameState::IsReady)! It's bad idea.
    //if (_state == GameState::IsReady) //? It's bad!
      //return;
    _field.Fill({false, Cell::Status::Closed});
    _mine_marked_count = 0;
    _not_opened_cells_count = (Quantity)(_field.GetArea());
    _game_time = 0;//??
    _state = GameState::IsReady;
  }

  bool GameEngine::SetFieldSize(const Size& new_size, bool start_new_game)
  {
    if (GetFieldSize() == new_size)
      return true;
    // Check for size is done in _field.SetSize(new_size), but..
    // It returns true even if current size equals new_size and nothing happened, so..
    bool result = _field.SetSize(new_size); // So, practically result is always true
    if (_mines_count > GetMaxMinesCount() && result)
      _mines_count = GetMaxMinesCount(); // Fit _mines_count if needed
    if (start_new_game && result)
      NewGame();
    return result;
  }

  template<typename _Type> // FitValueInRange
  const _Type& FitValue(const _Type& value, const _Type& min, const _Type& max)
  //_Type FitValue(_Type value, _Type min, _Type max)
  {
    if (value >= min && value <= max)
      return value;
    else if (value < min)
      return min;
    else // value > max
      return max;
  }

  /*template<typename _Type>
  _Type FitValue(_Type value, _Type max)
  {
    return FitValue(value, (_Type)0, max);
  }//*/

  // It always returns true. It isn't smart.
  bool GameEngine::SetMinesCount(Quantity value, bool start_new_game)
  {
    if (_mines_count == value)
      return true;

    /*auto max_mines = GetMaxMinesCount();
    if (value >= MinMinesCount && value <= max_mines)
      _mines_count = value;
    else if (value < MinMinesCount)
      _mines_count = MinMinesCount;
    else // value > GetMaxMinesCount()
      _mines_count = max_mines;//*/
    _mines_count = FitValue(value, MinMinesCount, GetMaxMinesCount());

    if (start_new_game)
      NewGame();
    return true;
  }

  bool GameEngine::SetSettings(const Size& field_size, Quantity mines_count)
  {
    bool result = SetFieldSize(field_size, false) && SetMinesCount(mines_count, false);
    if (result)
      NewGame();
    return result;
  }

  bool GameEngine::OpenCell(const Point &pos)
  {
    if (!CanCellBeOpened(pos))
      return false;

    if (_state == GameState::IsReady)
      StartGame(pos);

    _field[pos].Open();
    --_not_opened_cells_count;

    if (_field[pos].has_mine) {
      FinishGame(GameState::HasLost);
      return true;
    }

    // Should it be at the end (after OpenSurroundingCellsImpl)?
    if (_not_opened_cells_count == _mines_count) {
      FinishGame(GameState::HasWon);
      return true;
    }

    if (_field.MinesAround(pos) == 0)
      OpenSurroundingCellsImpl(pos);
    return true;
  }

  bool GameEngine::MineMarkCell(const Point &pos)
  {
    //if(!IsGameRunning() || !IsPosValid(pos))
    if(IsGameOver() || !IsPosValid(pos))
      return false;
    bool result = _field[pos].MineMarking();
    if (result)
      _mine_marked_count += _field[pos].IsMineMarked() ? 1 : -1;
    return result;
  }

  void GameEngine::OpenSurroundingCells(const Point &pos)
  {
    if (!IsGameRunning() || !IsPosValid(pos) || !_field[pos].IsOpened())// || _field[pos].has_mine) // Check for has_mine is doubtful
      return;
    int closed{0}, mines{0}, marked{0};
    auto l = [&](const Point& p) { // default capture [&] is safe here. The lambda is used only once.
      if (_field[p].IsClosed()) ++closed;
      if (_field[p].has_mine) ++mines;
      if (_field[p].IsMineMarked()) ++marked;
    };
    _field.DoWithAround(pos, l);

    if (closed > 0 && mines == marked)
      OpenSurroundingCellsImpl(pos);
  }

  static inline int Random(int max, int min = 0)
  {
    return min + (int)((double)(max - min + 1) * rand() / (RAND_MAX + 1.0));
  }

  bool GameEngine::PutMines(const Point& start_pos)
  {
    if (!IsPosValid(start_pos))
      return false;

    //if (_mines_count > GetFieldSize().Area() - MinFreeCells) { //?
    if (_mines_count >= (Quantity)GetFieldSize().Area()) {
      _field.Fill({true, Cell::Status::Closed});
      return false;
    }

    _field.Fill({false, Cell::Status::Closed});

    Rect r = _field.GetRoundRect(start_pos);
    auto range = GetFieldSize().Area();
    for (auto mines_left = _mines_count; mines_left > 0; ) {
      auto n = Random(range - 1);
      if (!_field[n].has_mine && !r.IsPointInRect(PosToPoint(n))) {
        _field[n].has_mine = true;
        --mines_left;
      }
    }
    return true;
  }

  Quantity GameEngine::GetCurrentGameTime() const
  {
    switch (_state) {
      case GameState::IsReady:
        return 0;
      case GameState::IsRunning:
        return time(nullptr) - _game_time;
      case GameState::HasLost:
      case GameState::HasWon:
        return _game_time;
      default:
        return 0;
    }
  }

  void GameEngine::FinishGame(GameState state, bool open_mines)
  {
    if (state != GameState::HasLost && state != GameState::HasWon)
      return;
    _state = state;
    _game_time = time(nullptr) - _game_time;
    if (open_mines)
      OpenMines();//OpenAll();
  }


}
