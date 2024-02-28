/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#ifndef MINESWEEPER_CORE_HPP_INCLUDED
#define MINESWEEPER_CORE_HPP_INCLUDED

#include <cstdlib>
#include <ctime>

#include <array>
#include <functional>
//#include <random>

#include "vs_size.hpp"
#include "vs_point.hpp"
#include "vs_rect.hpp"
#include "vs_array2d.hpp"


namespace minesweeper_game_core // (Core | Engine) MSGE | MGE
{

  /// types and aliases ///

  using Coord = unsigned short;
  using Quantity = unsigned int;//size_t? //rename to Amount?

  using Size = vslib::Size<Coord>;
  using Point = vslib::Point<Coord>;
  using Rect = vslib::Rect<Coord>;


  /// classes list ///

  struct Cell;
  class GameField;
  class GameEngine; //rename to GameCore?
  struct LevelData;


  /// struct Cell ///

  struct Cell {
  public:
    // Should it be inside of Cell or outside of Cell?
    enum class Status: char {Closed, MineMarked, Opened};//, QuestionMarked};
  public:
    bool has_mine{false};
    Status status{Status::Closed};
    // To do:
    // Now this is a calculated parameter
    // It works fine, but it isn't effective/optimal implementation
    // We can optimize it via this field:
    //signed char mines_around{-1}; // -1 means that number isn't valid
  public:
    // It's useless I guess..
    void Setup(bool _has_mine, Status _status)
      {has_mine = _has_mine; status = _status;}
    void operator()(bool _has_mine, Status _status)
      {Setup(_has_mine, _status);}

    bool Open() { // "bool" probably isn't a good idea. Change to void?
      if (status == Status::Closed)
        status = Status::Opened;
      return status == Status::Opened;
    }
    bool MineMarking() {
      if (status != Status::Opened)
        status = status == Status::MineMarked ? Status::Closed : Status::MineMarked;
      return status != Status::Opened;
    }

    bool IsOpened() const {return status == Status::Opened;}
    bool IsClosed() const {return status == Status::Closed;}
    bool IsMineMarked() const {return status == Status::MineMarked;}

    bool CanBeOpened() const {return status == Status::Closed;}
  };


  /// GameField ///

  // The version of GameField class with composition (instead of inheritance)
  class GameField {
  public:
    static constexpr Size DefaultFieldSize{9, 9};
    static constexpr Size MinFieldSize{4, 4};
    static constexpr Size MaxFieldSize{1000, 1000};
    static constexpr Cell DefaultCell{false, Cell::Status::Closed};
  public:
    explicit GameField(const Size& size = DefaultFieldSize, Cell cell = DefaultCell):
      _cells{ValidatedSize(size), cell} {}
    ~GameField() {Clear();}

    void Clear() {_cells.Clear();}
    void Fill(Cell cell = DefaultCell) {_cells.Fill(cell);}
    bool Setup(const Size &size, Cell cell = DefaultCell)
      {return _cells.Setup(ValidatedSize(size), cell);}

    bool SetSize(const Size& new_size)
      {return _cells.SetSize(ValidatedSize(new_size));}
    bool SetSizeIfPossible(const Size& new_size) // Pretty useless
      {return IsSizeValid(new_size) && _cells.SetSize(new_size);}
    const Size& GetSize() const {return _cells.GetSize();}
    auto GetWidth() const {return _cells.GetWidth();}//Size::FieldType
    auto GetHeight() const {return _cells.GetHeight();}//Size::FieldType
    auto GetArea() const {return _cells.GetArea();} // rename to GetLength?

    bool DoesPosExist(const Point& pos) const {return _cells.DoesPosExist(pos.x, pos.y);}

    static Size ValidatedSize(Size size)
      {return size.FitInto(MinFieldSize, MaxFieldSize);}
    static bool IsSizeValid(const Size& value) {
      return
        value.width >= MinFieldSize.width && value.height >= MinFieldSize.height &&
        value.width <= MaxFieldSize.width && value.height <= MaxFieldSize.height;
    }

    Cell& operator[](Quantity index) {return _cells[index];}
    Cell& operator[](const Point& pos) {return operator()(pos.x, pos.y);}
    Cell& operator()(Coord x, Coord y) {return _cells(x, y);}
    const Cell& operator[](Quantity index) const {return _cells[index];}
    const Cell& operator[](const Point& pos) const {return operator()(pos.x, pos.y);}
    const Cell& operator()(Coord x, Coord y)  const {return _cells(x, y);}

    bool DoWithAround(const Point& p, std::function<void(const Point&)> f);// const;

    template <typename _Type>
    int CountAround(const Point& pos, _Type Cell::*field, _Type value) const;
    int MinesAround(const Point& pos) const
      {return CountAround(pos, &Cell::has_mine, true);}
    int MineMarkedAround(const Point& pos) const
      {return CountAround(pos, &Cell::status, Cell::Status::MineMarked);}
    int ClosedAround(const Point& pos) const
      {return CountAround(pos, &Cell::status, Cell::Status::Closed);}

    inline Rect GetRoundRect(Coord x, Coord y) const;
    Rect GetRoundRect(const Point& p) const {return GetRoundRect(p.x, p.y);}
  private:
    vslib::Array2d<Cell, Coord> _cells{DefaultFieldSize, DefaultCell};
  };


  /*// The version with inheritance instead of composition
  class GameField: public vslib::Array2d<FieldCell, Coord> {
  public:
    using ParentClass = vslib::Array2d<FieldCell, Coord>;
  public:
    static constexpr Size DefaultFieldSize{9, 9};//{10, 10};
    static constexpr Size MinFieldSize{4, 4};
    static constexpr Size MaxFieldSize{1000, 1000};
    static constexpr FieldCell DefaultCell{false, Cell::Status::Closed};
  public:
    explicit GameField(const Size& size = DefaultFieldSize, FieldCell cell = DefaultCell):
      ParentClass::Array2d{ValidatedSize(size), cell} {}

    bool Setup(const Size& size, FieldCell cell = DefaultCell)
      {return ParentClass::Setup(ValidatedSize(size), cell);}

    bool SetSize(const Size& new_size)
      {return ParentClass::SetSize(ValidatedSize(new_size));}
    bool SetSizeIfPossible(const Size& new_size)
      {return IsSizeValid(new_size) && ParentClass::SetSize(new_size);}

    static Size ValidatedSize(Size size)
      {return size.FitInto(MinFieldSize, MaxFieldSize);}
    static bool IsSizeValid(const Size& value) {
      return
        value.width >= MinFieldSize.width && value.height >= MinFieldSize.height &&
        value.width <= MaxFieldSize.width && value.height <= MaxFieldSize.height;
    }

    using ParentClass::operator[];
    FieldCell& operator[](const Point& pos) //operator()
      {return ParentClass::operator()(pos.x, pos.y);}
    const FieldCell& operator[](const Point& pos) const //operator()
      {return ParentClass::operator()(pos.x, pos.y);}

    template <typename _TypeName>
    int CountAround(Coord x, Coord y,
      _TypeName FieldCell::*field, _TypeName value) const; // inline?
    int MinesAround(Coord x, Coord y) const
      {return CountAround(x, y, &FieldCell::has_mine, true);}//<bool>
    int MineMarkedAround(Coord x, Coord y) const
      {return CountAround(x, y, &FieldCell::status, Cell::Status::MineMarked);}//<CellStatus>

    inline Rect GetRoundRect(Coord x, Coord y) const;
    Rect GetRoundRect(const Point& p) const
      {return GetRoundRect(p.x, p.y);}
  };//*/


  /// GameEngine ///

  class GameEngine {
  public:
    static constexpr Quantity DefaultMinesCount{10};//auto??
    static constexpr Quantity MinFreeCells{9 + 1};
    static constexpr Quantity MinMinesCount{1};
  public:
    enum class GameState: char {IsReady, IsRunning, HasWon, HasLost};//Status/State? (ReadyToStart || Waiting) Win, Lose. Undefined?
  public:
    explicit GameEngine(const Size &field_size = GameField::DefaultFieldSize,
      Quantity mines_count = DefaultMinesCount)
      : _field(field_size, {false, Cell::Status::Closed})
    {
      SetMinesCount(mines_count);
      srand(time(nullptr));
    }
    //virtual ~GameEngine() {}
    GameEngine(const GameEngine&) = delete;
    const GameEngine& operator=(const GameEngine&) = delete;

    void NewGame();

    bool SetFieldSize(const Size& new_size, bool start_new_game = true);
    bool SetMinesCount(Quantity value, bool start_new_game = true);
    bool SetSettings(const Size& field_size, Quantity mines_count); // = 0; //ChangeSettings || Setup
    inline bool SetSettings(const LevelData& data);

    const Size& GetFieldSize() const {return _field.GetSize();}
    Quantity GetMinesCount() const {return _mines_count;}

    bool OpenCell(const Point &pos); // Change bool to void?
    bool MineMarkCell(const Point &pos); // Change bool to void? //rename to MineMarkOrClose?
    void OpenSurroundingCells(const Point &pos);

    GameState GetGameState() const {return _state;}
    bool IsGameOver() const
      {return _state == GameState::HasLost || _state == GameState::HasWon;}
    bool IsGameRunning() const
      {return _state == GameState::IsRunning;}
    Quantity GetCurrentGameTime() const; //size_t?
    Quantity GetMineMarkedCount() const {return _mine_marked_count;}
    Quantity GetNotOpenedCellsCount() const {return _not_opened_cells_count;}

    const GameField& GetField() const {return _field;} // Game needs Field to draw it
  public://protected:
    void OpenAll() {
      for (size_t i = 0, n = _field.GetArea(); i < n; ++i)
        _field[i].status = Cell::Status::Opened;
    }
    void OpenMines() {
      for (size_t i = 0, n = _field.GetArea(); i < n; ++i)
        if (_field[i].has_mine)
          _field[i].status = Cell::Status::Opened;
    }
  protected:
    bool PutMines(const Point& start_pos);
    Point PosToPoint(Quantity pos) const //????
      // This should be checked (so as an implementation of array2d).
      //{return {(Coord)(pos % GetFieldSize().width), (Coord)(pos / GetFieldSize().width)};}
      {return {(Coord)(pos / GetFieldSize().height), (Coord)(pos % GetFieldSize().height)};}
    Quantity GetMaxMinesCount() const {return GetFieldSize().Area() - MinFreeCells;}
    void OpenSurroundingCellsImpl(const Point &pos) {
      _field.DoWithAround(pos, [this](const Point& p) {
        if (this->_field[p].IsClosed()) this->OpenCell(p);});
    }

    bool CanCellBeOpened(const Point& pos) const
      {return IsPosValid(pos) && _field[pos].IsClosed() && !IsGameOver();}
    inline void StartGame(const Point& start_pos);
    void FinishGame(GameState state, bool open_mines = true);

    bool IsPosValid(const Point& pos) const
      {return _field.DoesPosExist(pos);}

    static bool IsMinesCountValid(Quantity mines_count, const Size& field_size)
      {return mines_count > 0 && mines_count <= field_size.Area() - MinFreeCells;}
  private:
    GameField _field{GameField::DefaultFieldSize, {false, Cell::Status::Closed}};
    Quantity _mines_count{DefaultMinesCount};
    GameState _state{GameState::IsReady};
    Quantity _game_time{0};
    Quantity _mine_marked_count{0};
    Quantity _not_opened_cells_count{(Quantity)_field.GetArea()};
  };


  /// struct LevelData ///

  struct LevelData {
  public:
    Size field_size{GameField::DefaultFieldSize};//{9, 9};
    Quantity mines_count{GameEngine::DefaultMinesCount};//10
  };


  /// consts ///

  constexpr std::array<LevelData, 3> DefaultLevels {
    LevelData{{9, 9}, 10},
    LevelData{{16, 16}, 40},
    LevelData{{30, 16}, 99}
  };


  /// IMPLEMENTATION ///


  /// class GameField ///

  template <typename _Type>
  int GameField::CountAround(const Point& pos, _Type Cell::*field, _Type value) const {
    int res{0};
    const_cast<GameField*>(this)->DoWithAround(pos, // const_cast is safe here.
      [this, field, &value, &res](const Point& p) {
        if ((*this)[p].*field == value)
          ++res;
      }
    );
    return res;
  }

  inline Rect GameField::GetRoundRect(Coord x, Coord y) const
  {
    return {
      (Rect::FieldType)(x == 0 ? 0 : x-1),
      (Rect::FieldType)(y == 0 ? 0 : y-1),
      (Rect::FieldType)(x == GetWidth()-1 ? GetWidth()-1 : x+1),
      (Rect::FieldType)(y == GetHeight()-1 ? GetHeight()-1 : y+1)
    };
  }


  /// class GameEngine ///

  // LabelData defined after GameEngine, so this code has to be here
  inline bool GameEngine::SetSettings(const LevelData& data)
  {
    return SetSettings(data.field_size, data.mines_count);
  }

  inline void GameEngine::StartGame(const Point& start_pos)
  {
    PutMines(start_pos);
    _mine_marked_count = 0;//?
    _state = GameState::IsRunning;
    _game_time = time(nullptr);
  }


} // namespace minesweeper_game_core


#endif // MINESWEEPER_CORE_HPP_INCLUDED
