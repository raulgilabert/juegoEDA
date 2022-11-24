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

#include <list>
#include <algorithm>

#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nya
#define in :
#define Inf 100
#define MAX_DIST 20

typedef vector<int> VI;

typedef vector<Cell> VC;
typedef vector<VC> VVC;

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
    int prev_x;
    int prev_y;

    Data_to_visit(Dir direction, int distance, int x, int y, int prev_x, int
    prev_y) {
        this->direction = direction;
        this->distance = distance;
        this->x = x;
        this->y = y;
        this->prev_x = prev_x;
        this->prev_y = prev_y;
    }
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

    void add_tiles(QData_to_visit& to_visit,
                   const VVData_visited& visited, int x, int y, int distance) {
        /*
         * Explicación de los tipos usados aquí:
         * El struct Data_to_visit tiene estos elementos porque es necesario
         * tener guardada la dirección que se toma desde la célula anterior a
         * esta que se visita además de la distancia que hay desde el inicio
         * junto con la obviamente necesaria posición de la celda a visitar
         */

        if (pos_ok(Pos(x, y) + Down) and not visited[y][x + 1].visited
        and cell(Pos(x, y) + Down).type == Street)
            to_visit.push(
                    Data_to_visit(Down, distance, x + 1, y,
                                  x, y));

        if (pos_ok(Pos(x, y) + Up) and not visited[y][x - 1].visited
        and cell(Pos(x, y) + Up).type == Street)
            to_visit.push(
                    Data_to_visit(Up, distance, x - 1, y,
                                  x ,y));

        if (pos_ok(Pos(x, y) + Right) and not
        visited[y + 1][x].visited and
        cell(Pos(x, y) + Right).type == Street)
            to_visit.push(
                    Data_to_visit(Right, distance, x, y + 1,
                                  x, y));

        if (pos_ok(Pos(x, y) + Left) and not visited[y - 1][x].visited
        and cell(Pos(x, y) + Left).type == Street)
            to_visit.push(
                    Data_to_visit(Left, distance, x, y - 1,
                                  x, y));
    }


    Dir bfs_food(int x, int y) {
        VVData_visited visited(60, VData_visited(60));

        Data_visited origin_cell;
        origin_cell.visited = true;
        origin_cell.distance = 0;

        visited[y][x] = origin_cell;

        /*
         * Hay que añadir las casillas adyacentes que no sean pared a la cola
         * de posiciones que mirar
         */

        QData_to_visit to_visit;

        add_tiles(to_visit, visited, x, y, 1);

        /*
         * Se recorre la cola hasta que esta esté vacía o se llegue a una
         * distancia demasiado grande desde el inicio hasta las posiciones
         * que se visitan o ya se haya encontrado comida
         */
        while (not to_visit.empty() and to_visit.front().distance < MAX_DIST) {
            /*
             * TODO: Hay que hacer todavía la lógica de aquí que ya desarrollaré
             * después
             */

            if (cell(to_visit.front().x, to_visit.front().y).food) {
                Data_visited new_cell;
                new_cell.visited = true;
                new_cell.distance = to_visit.front().distance;
                new_cell.direction = to_visit.front().direction;

                new_cell.x = to_visit.front().prev_x;
                new_cell.y = to_visit.front().prev_y;

                visited[to_visit.front().y][to_visit.front().x] = new_cell;

                auto current_cell = visited[to_visit.front().y]
                        [to_visit.front().x];

                while (current_cell.distance > 1) {
/*                    cerr << current_pos << ' ' << current_cell.distance <<
endl;

                    current_pos += current_cell.direction;
                    current_cell = visited[current_pos.j][current_pos.i];*/

                    /*
                     *
                     */

                    cerr << current_cell.x << ' ' << current_cell.y <<
                    current_cell.direction << ' ' << current_cell.distance
                            << endl;
                    current_cell = visited[current_cell.y][current_cell.x];
                }
                cerr << current_cell.x << ' ' << current_cell.y <<
                current_cell.direction << endl;
                cerr << "-----------------------" << endl;
                return current_cell.direction;
            }

            add_tiles(to_visit, visited, to_visit.front().x,
                      to_visit.front().y,
                      to_visit.front().distance + 1);

            Data_visited new_cell;
            new_cell.visited = true;
            new_cell.distance = to_visit.front().distance;
            new_cell.direction = to_visit.front().direction;

            new_cell.x = to_visit.front().prev_x;
            new_cell.y = to_visit.front().prev_y;

            visited[to_visit.front().y][to_visit.front().x] = new_cell;

            to_visit.pop();
        }


        vector<Dir> dirs = {Up, Down, Left, Right};
        Dir dir = dirs[random(0, 3)];

        return dir;

        /*
         * TODO: En caso de que found sea true se mira desde el elemento que se
         * ha tratado en último lugar en el while. Si es false se devuelve una
         * dirección aleatoria
         */
    }




    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int id = me();

        VI mine_alive = alive_units(id);

        vector<pair<int, Dir>> movements;

        // Search for food for every troop
        for (auto troop in mine_alive) {
            auto t = unit(troop);
            movements.emplace_back(pair<int, Dir>(troop, bfs_food(
                                                               t.pos.i,
                                                               t.pos.j)));
        }
    
        for (auto mov in movements) {
            move(mov.first, mov.second);
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
