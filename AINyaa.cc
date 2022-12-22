#include "Player.hh"
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nyaa


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

    struct info_visited {
        int distance;
        Dir direction;
        bool visited;

        info_visited() {
            distance = -1;
            direction = Up;
            visited = false;
        }
    };

    struct info_to_visit {
        Pos pos;
        int distance;
        Dir direction;
    };

    int MAX_CELLS = 500;

    vector<Dir> dirs = {Up, Left, Down, Right};
    vector<Dir> diagonals = {UL, LD, DR, RU};

    priority_queue<pair<int, pair<int, Dir>>> movements;

    bool correct_pos(Pos pos) {
        return (pos_ok(pos) and cell(pos).type == Street);
    }

    void add(queue<info_to_visit>& to_visit, vector<vector<info_visited>>&
    visited, int& counter) {
        int distance = to_visit.front().distance + 1;
        Dir direction = to_visit.front().direction;
        Pos pos = to_visit.front().pos;


        for (Dir dir : dirs) {
            Pos new_pos = pos + dir;

            if (correct_pos(new_pos) and
                not visited[new_pos.j][new_pos.i].visited) {
                visited[new_pos.j][new_pos.i].visited = true;
                ++counter;
                to_visit.push({new_pos, distance, direction});
            }
        }
    }

    pair<bool, Dir> is_diagonal(int a, int b) {
        int x = unit(a).pos.i - unit(b).pos.i;
        int y = unit(a).pos.j - unit(b).pos.j;

        if (x == 1 and y == 1)
            return {true, UL};
        if (x == 1 and y == -1)
            return {true, RU};
        if (x == -1 and y == 1)
            return {true, LD};
        if (x == -1 and y == -1)
            return {true, DR};

        return {false, Up};
    }

    void bfs(int t, int id) {
        queue<info_to_visit> to_visit;
        vector<vector<info_visited>> visited(60,
                                             vector<info_visited>(60));

        Pos init_pos = unit(t).pos;

        for (Dir dir : dirs) {
            if (correct_pos(init_pos + dir)) {
               to_visit.push({init_pos + dir, 1,
                               dir});
            }
        }

        int counter = 0;

        while (not to_visit.empty() and to_visit.front().distance < 20) {
            Cell current_cell = cell(to_visit.front().pos);

            if (to_visit.front().distance == 2 and (current_cell.id != -1 and
            unit(current_cell.id).player != id)) {
                pair<bool, Dir> diag = is_diagonal(t, current_cell.id);

                cerr << unit(t).pos << ' ' << diag.second << endl;

                if (diag.first) {
                    int indice;
                    if (diag.second == RU) {
                        indice = 1;
                    }
                    else if (diag.second == DR) {
                        indice = 0;
                    }
                    else if (diag.second == LD) {
                        indice = 3;
                    }
                    else {
                        indice = 2;
                    }

                    for (int i = 0; i < 4; ++i) {
                        if (correct_pos(unit(t).pos +
                            dirs[(indice + i) %4])
                            and unit(cell(unit(t).pos +
                            dirs[(indice + i)%4]).id).type != Dead) {

                            movements.push({10,
                                            {t, dirs[(indice + i)%4]}});
                            return;
                        }
                    }
                }

                if (unit(current_cell.id).player == -1) {
                    movements.push({100,{t, RU}});

                    return;
                }
                movements.push({8,
                                {t, to_visit.front().direction}});
                return;
            }

            /*if (to_visit.front().distance == 2 and (current_cell.id != -1 and
            unit(current_cell.id).player != id and
            unit(current_cell.id).type != Dead)) {
                return to_visit.front().direction;
            }*/



            if (to_visit.front().distance == 3 and (current_cell.id != -1 and
            unit(current_cell.id).player == -1)) {
                for (Dir dir : dirs) {
                    for (Dir diag: diagonals) {
                        if (to_visit.front().pos == init_pos+dir+diag)
                             movements.push({3,{t, RU}});
                            return;
                    }
                }
                movements.push({7,
                                {t, to_visit.front().direction}});
                return;
            }

            if(current_cell.food) {
                bool enemy = false;
                for (Dir dir : dirs) {
                    if (cell(to_visit.front().pos + dir).id != -1 and
                    unit(cell(to_visit.front().pos + dir).id).player !=
                    id)
                        enemy = true;
                }
                if (not enemy) {
                    movements.push({5,
                                    {t, to_visit.front().direction}});
                    return;
                }
            }

            if (( current_cell.id != -1 and
            unit(current_cell.id).player != id and
            unit(current_cell.id).type != Dead)) {
                 movements.push({100,
                                {t, to_visit.front().direction}});

                return;
            }

            info_visited new_cell;
            new_cell.visited = true;
            new_cell.direction = to_visit.front().direction;
            new_cell.distance = to_visit.front().distance;

            visited[to_visit.front().pos.j][to_visit.front().pos.i] = new_cell;

            add(to_visit, visited, counter);

            to_visit.pop();
        }
        for (Dir dir : dirs) {
            if (correct_pos(init_pos + dir) and
            cell(init_pos + dir).owner != id) {
                movements.push({1,{t, dir}});

                return;
            }
        }

        movements.push({0,{t, dirs[random(0, 3)]}});
        return;
    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int id = me();
        for (int t : alive_units(id)) {
            bfs(t, id);
        }

        while (not movements.empty()) {
            move(movements.top().second.first,
                 movements.top().second.second);
            movements.pop();
        }
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
