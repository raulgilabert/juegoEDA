/*
 * Raúl Gilabert Gámez - Q1 2022-2023 EDA
 *
 * Para el que esté leyendo sea profesor, alumno de años posteriores buscando
 * desesperadamente ayuda para algún juego de EDA o un curioso que está mirando
 * código de otra gente a saber por qué motivo:
 *
 * Estás a punto de comprobar las maravillas sin sentido que soy capaz de
 * escribir y que sorprendentemente funcionan.
 */

#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nya
#define in :
#define MAX_DIST 20
#define MAX_DIST_ZOMBIE 4
#define MAX_DIST_PLAYER 1

typedef pair<int, Dir> PID;

typedef vector<int> VI;

struct Data_visited {
    bool visited;
    Dir direction;
    int distance;
    int x;
    int y;

    Data_visited() {
        visited = false;
        direction = Up;
        distance = -1;
        x = -1;
        y = -1;
   }
};

typedef vector<Data_visited> VData_visited;
typedef vector<VData_visited> VVData_visited;

struct Data_to_visit {
    Dir direction;
    int distance;
    int x;
    int y;

    Data_to_visit(Dir direction, int distance, int x, int y) {
        this->direction = direction;
        this->distance = distance;
        this->x = x;
        this->y = y;
    }
};

struct zombie_found {
    int distance;
    Dir direction;
    int id_zombie;
};

typedef queue<Data_to_visit> QData_to_visit;


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

    bool check_pos(Pos pos) {
        return (pos_ok(pos) and cell(pos).type == Street);
    }

    void add_tiles(QData_to_visit& to_visit,
                   VVData_visited& visited, int x, int y, int distance) {
        /*
         * Explicación de los tipos usados aquí:
         * El struct Data_to_visit tiene estos elementos porque es necesario
         * tener guardada la dirección que se toma desde la célula anterior a
         * esta que se visita además de la distancia que hay desde el inicio
         * junto con la obviamente necesaria posición de la celda a visitar
         */

        if (distance == 1) {
            if (check_pos(Pos(x, y) + Down) and
                not visited[y][x + 1].visited) {
                visited[y][x + 1].visited = true;
                to_visit.push(
                        Data_to_visit(Down, distance, x + 1, y));
            }
            if (check_pos(Pos(x, y) + Up) and
                not visited[y][x - 1].visited) {
                visited[y][x - 1].visited = true;
                to_visit.push(
                        Data_to_visit(Up, distance, x - 1, y));
            }
            if (check_pos(Pos(x, y) + Right) and
                not visited[y + 1][x].visited) {
                visited[y + 1][x].visited = true;
                to_visit.push(
                        Data_to_visit(Right, distance, x, y + 1));
            }
            if (check_pos(Pos(x, y) + Left) and
                not visited[y - 1][x].visited) {
                visited[y - 1][x].visited = true;
                to_visit.push(
                        Data_to_visit(Left, distance, x, y - 1));
            }
        }
        else {
             if (check_pos(Pos(x, y) + Down) and
                not visited[y][x + 1].visited) {
                 visited[y][x + 1].visited = true;
                 to_visit.push(
                         Data_to_visit(visited[y][x].direction,
                                       distance, x + 1, y));
             }
            if (check_pos(Pos(x, y) + Up) and
                not visited[y][x - 1].visited) {
                visited[y][x - 1].visited = true;
                to_visit.push(
                        Data_to_visit(visited[y][x].direction,
                                      distance, x - 1, y));
            }
            if (check_pos(Pos(x, y) + Right) and
                not visited[y + 1][x].visited) {
                visited[y + 1][x].visited = true;
                to_visit.push(
                        Data_to_visit(visited[y][x].direction,
                                      distance, x, y + 1));
            }
            if (check_pos(Pos(x, y) + Left) and
                not visited[y - 1][x].visited) {
                visited[y - 1][x].visited = true;
                to_visit.push(
                        Data_to_visit(visited[y][x].direction,
                                      distance, x, y - 1));
            }
        }
    }

    pair <bool, Dir> diagonal(int player, int other) {
        int x = unit(player).pos.i - unit(other).pos.i;
        int y = unit(player).pos.j - unit(other).pos.j;

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


    Dir bfs_food(int id, int x, int y) {
        VVData_visited visited(60, VData_visited(60));

        Data_visited origin_cell;
        origin_cell.visited = true;
        origin_cell.distance = 0;

        visited[y][x] = origin_cell;

        QData_to_visit to_visit;

        add_tiles(to_visit, visited, x, y, 1);

        while (not to_visit.empty() and to_visit.front().distance < MAX_DIST) {
            auto current_cell = cell(to_visit.front().x,
                                            to_visit.front().y);

            if (current_cell.food) {
                return to_visit.front().direction;
            }

            if (to_visit.front().distance < MAX_DIST_ZOMBIE and
            current_cell.id != -1 and unit(current_cell.id).player == -1) {
                auto is_diagonal = diagonal(id,
                                            current_cell.id);

                if (to_visit.front().distance == 2 and is_diagonal.first) {
                    vector<Dir> dirs = {Up, Left, Down, Right};

                    int indice;
                    if (is_diagonal.second == RU) {
                        indice = 1;
                    }
                    if (is_diagonal.second == DR) {
                        indice = 0;
                    }
                    if (is_diagonal.second == LD) {
                        indice = 3;
                    }
                    if (is_diagonal.second == UL) {
                        indice = 2;
                    }

                    for (int i = 0; i < 4; ++i) {
                        if (check_pos(unit(id).pos + dirs[(indice + i)%4])
                            and unit(cell(unit(id).pos +
                            dirs[(indice + i)%4]).id).type != Dead) {
                            return dirs[(indice + i)%4];
                        }
                    }

                }

                return to_visit.front().direction;
            }

            Data_visited new_cell;
            new_cell.visited = true;
            new_cell.distance = to_visit.front().distance;
            new_cell.direction = to_visit.front().direction;

            visited[to_visit.front().y][to_visit.front().x] = new_cell;

            add_tiles(to_visit, visited, to_visit.front().x,
                      to_visit.front().y,
                      to_visit.front().distance + 1);

            to_visit.pop();
        }

        vector<Dir> dirs = {Up, Down, Left, Right};
        Dir dir = dirs[random(0, 3)];

        return dir;
    }


    /*
     * Devuelvo la distancia y la dirección inicial que se ha tomado para
     * llegar al zombi
     *
    zombie_found bfs_zombies(int x, int y) {
        /
         * Esta función es casi un copia-pega de la de comida, no hace falta
         * comentar mucha cosa
         *
         VVData_visited visited(60, VData_visited(60));
         Data_visited origin_cell;
         origin_cell.visited = true;
         origin_cell.distance = 0;

         visited[y][x] = origin_cell;


         QData_to_visit to_visit;

        add_tiles(to_visit, visited, x, y, 1);

        auto zombies_id = zombies();

        while (not to_visit.empty() and to_visit.front().distance <
        MAX_DIST_ZOMBIE) {
            Cell current_cell = cell(to_visit.front().x,
                                     to_visit.front().y);

            if (current_cell.id != -1 and
                unit(current_cell.id).player == -1) {
                return {to_visit.front().distance,
                        to_visit.front().direction,
                        current_cell.id};
            }

            Data_visited new_cell;
            new_cell.visited = true;
            new_cell.distance = to_visit.front().distance;
            new_cell.direction = to_visit.front().direction;

            visited[to_visit.front().y][to_visit.front().x] = new_cell;

            add_tiles(to_visit, visited, to_visit.front().x,
                      to_visit.front().y,
                      to_visit.front().distance + 1);

            to_visit.pop();
        }

        // En caso de no encontrar zombies se devuelve un pair que indique esto
        return {-1, Up, -1};
    }


    zombie_found bfs_zombies(Pos pos) {
        return bfs_zombies(pos.i, pos.j);
    }


    }*/



    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int id = me();

        VI mine_alive = alive_units(id);


        /*VI enemies_alive;

        for (int i = 0; i < 4; ++i) {
            if (i != id) {
                VI enemies = alive_units(i);

                for (auto e in enemies) {
                    enemies_alive.emplace_back(e);
                }
            }
        }*/



        /*
         * Primero eliminamos jugadores adyacentes al ser un peligro por su
         * movilidad y después atacamos a los zombies provocando un bonito
         * GENOCIDIO
         */

        /*vector<PID> zombies_movements;
        vector<int> dont_move;
        VI already_moved;
        vector<PID> movements;
        */

        for (int troop in mine_alive) {
            auto t = unit(troop);

            move(troop, bfs_food(troop, t.pos.i, t.pos.j));
        }
   }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

            /*
            // Comprobar si hay zombies en celdas anexas y atacar
            auto zombie = bfs_zombies(t.pos);

            if (zombie.distance != -1) {
                auto is_diagonal = diagonal(troop,
                                                             zombie
                                                             .id_zombie);
                if (zombie.distance == 1) {
                    zombies_movements.emplace_back(PID(troop,zombie
                                                                  .direction));
                    already_moved.emplace_back(troop);
                    continue;
                }

                if (zombie.distance == 2 and is_diagonal.first) {
                    /
                     * Recorrer el vector en círculo con índice inicial
                     * dependiendo de la dirección del zombi
                     *
                   continue;
                }

                if (zombie.distance == 2 and not is_diagonal.first) {
                    zombies_movements.emplace_back((PID(troop,Up)));
                    already_moved.emplace_back(troop);
                    continue;
                }
            }

            // Comprobación de enemigos en celdas adyacentes
            vector<Dir> dirs = {Up, Right, Down, Left};

            bool found = false;

            for (int i = 0; i < 4; ++i) {
                if (check_pos(t.pos + dirs[i])) {
                    auto cell_view = cell(t.pos + dirs[i]).id;
                    if (cell_view != -1) {
                        auto enemy = unit(cell_view);

                        if (enemy.player != id) {
                            zombies_movements.emplace_back(PID(troop,
                                                               dirs[i]));

                            found = true;

                            break;
                        }
                    }
                }
            }

            if (found)
                continue;
            */
