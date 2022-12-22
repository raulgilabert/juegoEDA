#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nyaaa


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

    int MAX_DIST_FOOD = 20;

    vector<Dir> dirs = {Up, Left, Down, Right};

    bool correct_pos(Pos pos) {
        return (pos_ok(pos) and cell(pos).type == Street);
    }

    void add(queue<info_to_visit>& to_visit, const vector<vector<info_visited>>&
    visited) {
        int distance = to_visit.front().distance + 1;
        Dir direction = to_visit.front().direction;
        Pos pos = to_visit.front().pos;


        for (Dir dir : dirs) {
            Pos new_pos = pos + dir;

            if (correct_pos(new_pos) and
                not visited[new_pos.j][new_pos.i].visited) {
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
            return {true, LD};
        if (x == -1 and y == 1)
            return {true, RU};
        if (x == -1 and y == -1)
            return {true, DR};

        return {false, Up};
    }

    Dir bfs(int t, int id) {
        queue<info_to_visit> to_visit;
        vector<vector<info_visited>> visited(60,
                                             vector<info_visited>(60));

        Pos init_pos = unit(t).pos;

        for (Dir dir : dirs) {
            if (correct_pos(init_pos + dir)) {
                if (cell(init_pos + dir).id != -1) {
                    Unit unit_to_check =  unit(cell(init_pos + dir).id);

                    if (unit_to_check.player != id and unit_to_check.type !=
                    Dead)
                    return dir;
                }
                to_visit.push({init_pos + dir, 1,
                               dir});
            }
        }

        while (not to_visit.empty() and to_visit.front().distance <
        MAX_DIST_FOOD) {
            Cell current_cell = cell(to_visit.front().pos);

            info_visited new_cell;
            new_cell.visited = true;
            new_cell.direction = to_visit.front().direction;
            new_cell.distance = to_visit.front().distance;

            visited[to_visit.front().pos.j][to_visit.front().pos.i] = new_cell;

            if (to_visit.front().distance == 2 and current_cell.id != -1 and
            current_cell.owner != id) {
                pair<bool, Dir> diag = is_diagonal(t, current_cell.id);

                if (diag.first) {
                    int indice;
                    if (diag.second == RU) {
                        indice = 1;
                    }
                    if (diag.second == DR) {
                        indice = 0;
                    }
                    if (diag.second == LD) {
                        indice = 3;
                    }
                    if (diag.second == UL) {
                        indice = 2;
                    }

                    for (int i = 0; i < 4; ++i) {
                        if (correct_pos(unit(t).pos +
                        dirs[(indice + i) %4])
                        and unit(cell(unit(t).pos +
                        dirs[(indice + i)%4]).id).type != Dead) {
                            return dirs[(indice + i)%4];
                        }
                    }
                }

                return RU;
            }

            if (current_cell.food or (current_cell.id != -1 and current_cell
            .owner != id))
                return to_visit.front().direction;

            add(to_visit, visited);

            to_visit.pop();
        }
        for (Dir dir : dirs) {
            if (correct_pos(init_pos + dir) and
            cell(init_pos + dir).owner != id)
                return dir;
        }

        return dirs[random(0, 3)];
    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int id = me();
        for (int t : alive_units(id)) {
            Dir dir  = bfs(t, id);

            if (correct_pos(unit(t).pos + dir)) {
                cerr << t << ' ' << unit(t).pos << ' ' << dir << endl;
                move(t, dir);
            }
        }
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
