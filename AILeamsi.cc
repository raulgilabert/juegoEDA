#include "Player.hh"
#include <vector>
#include <queue>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Leamsi


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  enum Content {
    Enemy,
    Corpse,
    Zombie,
    Friend,
    Food,
    None
  };
  
  typedef vector<int> VI;
  typedef vector<CellType> VC;
  typedef vector<VC> VVC;

  typedef vector<Content> VN;
  typedef vector<VN> VVN;

  VVC board;


  const vector<Dir> dirs = {Up, Down, Left, Right};

  //*******PROCEDURES*******//
  
  void read_board_data() {
    board.resize(board_rows());
    for (int i = 0; i < board_rows(); ++i) {
      board[i].resize(board_cols());
      for (int j = 0; j < board_cols(); ++j)
        board[i][j] = cell(i,j).type;
    }
  }

  void read_content(VVN& cnt) {
    cnt.resize(board_rows());
    for (int i = 0; i < board_rows(); ++i) {
      cnt[i].resize(board_cols());
      for (int j = 0; j < board_cols(); ++j) {
        Cell c_aux = cell(i,j);
        if (c_aux.food) cnt[i][j] = Food;
        else if (c_aux.id != -1) {
          int player = unit(c_aux.id).player;
          if (unit(c_aux.id).type == Dead) cnt[i][j] = Corpse;
          else if (player == me()) cnt[i][j] = Friend;
          else if (player != -1) cnt[i][j] = Enemy;
          else cnt[i][j] = Zombie;
        }
        else cnt[i][j] = None;
      }
    }
  }
  
  bool zombie_will_kill_me(const Pos& p, const VVN& cnt) {
    int x = p.i, y = p.j;
    int lim_x = int(cnt.size()-1);
    int lim_y = int(cnt[0].size()-1);

    if (x > 0 and cnt[x-1][y] == Zombie) return true;
    if (x < lim_x and cnt[x+1][y] == Zombie) return true;
    if (y > 0 and cnt[x][y-1] == Zombie) return true;
    if (y < lim_y and cnt[x][y+1] == Zombie) return true;
    
    if (x > 0 and y > 0 and cnt[x-1][y-1] == Zombie) return true;
    if (x < lim_x and y > 0 and cnt[x+1][y-1] == Zombie) return true;
    if (x > 0 and y < lim_y and cnt[x-1][y+1] == Zombie) return true;
    if (x < lim_x and y < lim_y and cnt[x+1][y+1] == Zombie) return true;

    return false;
  }

  Dir search_another_cell(const Pos& p, bool& be_quiet, const VVN& cnt) {
    for (auto d : dirs) {
      Pos new_pos = p + d;
      if (pos_ok(new_pos)) {
        Cell temp_cell = cell(new_pos);
        if (temp_cell.type != Waste and temp_cell.id == -1 and not zombie_will_kill_me(new_pos, cnt)) {
          be_quiet = false;
          return d;
        }
      }
    }
    return Left;
  }

  Dir random_pos_ok(const Pos& p) {
    for (auto d : dirs) {
      if (pos_ok(p+d) and cell(p+d).type != Waste ) return d;
    }
    return Left;
  }

  Dir ocupe_cell(const Pos& p) {
    for (auto d : dirs) {
      if (pos_ok(p+d) and cell(p+d).type != Waste and cell(p+d).id == -1 and cell(p+d).owner != me()) return d;
    }
    return random_pos_ok(p);
  }

  Dir go_for_food(const VVN& cnt, const Pos& p, const Content& what_to_search) {
    vector<vector<bool>> visited(board.size(), vector<bool>(board[0].size(), false));
    queue <pair<Dir,pair<int,int>>> Q;
    
    visited[p.i][p.j] = true;
    int x = p.i, y = p.j;
   
    if (x > 0 and board[x-1][y] != Waste and cnt[x-1][y] != Corpse) Q.push(make_pair(Up, make_pair(x-1, y)));
    if (y < int(cnt[0].size()-1) and board[x][y+1] != Waste and cnt[x][y+1] != Corpse) Q.push(make_pair(Right, make_pair(x, y+1)));
    if (x < int(cnt.size()-1) and board[x+1][y] != Waste and cnt[x+1][y] != Corpse) Q.push(make_pair(Down, make_pair(x+1, y)));
    if (y > 0 and board[x][y-1] != Waste and cnt[x][y-1] != Corpse) Q.push(make_pair(Left, make_pair(x, y-1)));

    int depth = 1;
    while (not Q.empty()) {
      int level_size = Q.size(); 
      
      while (level_size-- != 0) {
        int x = Q.front().second.first;
        int y = Q.front().second.second;
        Dir d = Q.front().first;

       Q.pop();

        if (cnt[x][y] == what_to_search) return d;
        
        if (not visited[x][y]) {
          visited[x][y] = true;
          if (x > 0 and board[x-1][y] != Waste) Q.push(make_pair(d, make_pair(x-1, y)));
          if (x < int(cnt.size()-1) and board[x+1][y] != Waste) Q.push(make_pair(d, make_pair(x+1, y)));
          if (y > 0 and board[x][y-1] != Waste) Q.push(make_pair(d, make_pair(x, y-1)));
          if (y < int(board[0].size()-1) and board[x][y+1] != Waste) Q.push(make_pair(d, make_pair(x, y+1)));
        }
      }
      ++depth;
      if (depth > 100) break; 
    }
    return ocupe_cell(p);
    //return search_another_cell(p, tmp, cnt);
  }

  bool enemy_near(const VVN& cnt, const Pos& p, Dir& what_to_do) {
    long unsigned int x = p.i, y = p.j;
    if (x > 0 and cnt[x-1][y] == Enemy) {what_to_do = Up; return true;}
    else if (x < cnt.size()-1 and cnt[x+1][y] == Enemy) {what_to_do = Down; return true;}
    else if (y > 0 and cnt[x][y-1] == Enemy) {what_to_do = Left; return true;}
    else if (y < cnt[0].size()-1 and cnt[x][y+1] == Enemy) {what_to_do = Right; return true;}

    else if (x > 0 and cnt[x-1][y] == Zombie) {what_to_do = Up; return true;}
    else if (x < cnt.size()-1 and cnt[x+1][y] == Zombie) {what_to_do = Down; return true;}
    else if (y > 0 and cnt[x][y-1] == Zombie) {what_to_do = Left; return true;}
    else if (y < cnt[0].size()-1 and cnt[x][y+1] == Zombie) {what_to_do = Right; return true;}

    return false;
  }


  void move_units() {
    //Makings the moves with priotity;
    priority_queue<pair<int,pair<int,Dir>>> pq; 
    VI alive = alive_units(me());
    VVN content;
    read_content(content);

    for (int id : alive) {
      bool need_to_move;
      Dir what_to_do;
      need_to_move = enemy_near(content, unit(id).pos, what_to_do);
      
      if (not need_to_move) {
        what_to_do = go_for_food(content, unit(id).pos, Food);
        
        if (zombie_will_kill_me(unit(id).pos + what_to_do, content)) {
          bool be_quiet = true;
          what_to_do = search_another_cell(unit(id).pos, be_quiet, content);
          if (not be_quiet) pq.push(make_pair(0,make_pair(id, what_to_do)));
        }
        else pq.push(make_pair(0,make_pair(id,what_to_do)));
      }
      else pq.push(make_pair(1,make_pair(id,what_to_do)));
      //move(id, what_to_do);
    }
    
    
    //Now do the steps in priority order.
    while (not pq.empty()) {
      int id = pq.top().second.first;
      Dir d = pq.top().second.second;
      move(id, d);
      pq.pop();
    }
    
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if (round() == 0) read_board_data();
    double st = status(me());
    //If nearly out of time (90%) do something special; 
    if (st >= 0.9) return; //cerr << "CPU TIME EXCEDED" << endl;
    move_units();  
    cerr << status(me()) << endl;
  }

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME); 
