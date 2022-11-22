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

typedef vector<int> VI;
typedef vector<bool> VB;
typedef vector<Cell> VC;
typedef vector<VC> VVC;

typedef pair<int, int> PII;
typedef queue<PII> QPII;
typedef list<PII> LPII;
typedef vector<PII> VPII;



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

    void add_tiles(const VVC& G, QPII& to_visit, const VB& visited, int x,
                   int y) {
        if (not visited[y*60 + x + 1] and pos_ok(x + 1, y))
            to_visit.push(PII(x + 1, y));

        if (not visited[y*60 + x - 1] and pos_ok(x - 1, y))
            to_visit.push(PII(x - 1, y));

        if (not visited[(y + 1)*60 + x] and pos_ok(x, y + 1))
            to_visit.push(PII(x, y + 1));

        if (not visited[(y - 1)*60 + x] and pos_ok(x, y - 1))
            to_visit.push(PII(x, y - 1));
    }


    void bfs(const VVC& G, QPII& to_visit, VB& visited, int x, int y) {


    }

    pair<int, Dir> bfs_i(const VVC& G, int x, int y) {
        //LPII visited;
        QPII to_visit;
        QPII to_visit_new;

        //visited.insert(visited.end(), pair<int, int>(x, y));

        VB visited(60*60, false);

        add_tiles(G, to_visit_new, visited, x, y);

        bool found = false;

        int distance = 0;

        vector<PII> nodes_found;

        while (not to_visit_new.empty() and not found) {
            ++distance;
            to_visit = to_visit_new;
            while (not to_visit.empty()) {
                auto current_node = to_visit.front();

                if

            }
        }

    }




    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int id = me();

        VI mine_alive = alive_units(id);

        vector<pair<int, Dir>> movements;

        // Create map
        cerr << "1" << endl;
        VVC terrain = VVC(60, VC(60));
        for (int i = 0; i < 60; ++i) {
            for (int j = 0; j < 60; ++j) {
                terrain[i][j] = cell(i, j);
            }
        }

        cerr << "2" << endl;
        // Search for food for every troop
        
        for (auto troop in mine_alive) {
            auto t = unit(troop);

            cerr << "3" << endl;
            movements.emplace_back(pair<int, Dir>(troop, bfs_i(terrain,
                                                               t.pos.i,
                                                               t.pos.j)
                                                                .second));
            cerr << "3 finished" << endl;
        }
    
        cerr << "4" << endl;

        for (auto mov in movements) {
            cerr << "5" << endl;
            move(mov.first, mov.second);
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
