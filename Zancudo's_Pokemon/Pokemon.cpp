#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

/* 
  ============================================================
  TIPOS Y TABLA DE EFECTIVIDADES
  ============================================================
*/ 

enum class Type {
    NORMAL, FUEGO, AGUA, PLANTA, ELECTRICO,
    HIELO, LUCHA, VENENO, TIERRA, VOLADOR,
    PSIQUICO, BICHO, ROCA, FANTASMA, DRAGON,
    SINIESTRO, ACERO, NINGUNO
};

string typeToString(Type t) {
    switch (t) {
        case Type::NORMAL:    return "Normal";
        case Type::FUEGO:     return "Fuego";
        case Type::AGUA:      return "Agua";
        case Type::PLANTA:    return "Planta";
        case Type::ELECTRICO: return "Electrico";
        case Type::HIELO:     return "Hielo";
        case Type::LUCHA:     return "Lucha";
        case Type::VENENO:    return "Veneno";
        case Type::TIERRA:    return "Tierra";
        case Type::VOLADOR:   return "Volador";
        case Type::PSIQUICO:  return "Psiquico";
        case Type::BICHO:     return "Bicho";
        case Type::ROCA:      return "Roca";
        case Type::FANTASMA:  return "Fantasma";
        case Type::DRAGON:    return "Dragon";
        case Type::SINIESTRO: return "Siniestro";
        case Type::ACERO:     return "Acero";
        default:              return "---";
    }
}

// "Determinar multiplicador: x2 / x1 / x0.5 / x0" (ver leyenda del diagrama)
class TypeChart {
public:
    static TypeChart& getInstance() {
        static TypeChart instance;
        return instance;
    }

    // Compara Tipo del ataque vs Tipos del defensor
    float getMultiplier(Type attacker, Type defender) const {
        if (attacker == Type::NINGUNO || defender == Type::NINGUNO) return 1.0f;
        return chart[idx(attacker)][idx(defender)];
    }

private:
    float chart[18][18];

    TypeChart() {
        for (auto& row : chart)
            for (auto& cell : row)
                cell = 1.0f; // Normal = x1 por defecto

        set(Type::FUEGO, Type::PLANTA, 2.0f);    set(Type::FUEGO, Type::HIELO, 2.0f);
        set(Type::FUEGO, Type::BICHO, 2.0f);     set(Type::FUEGO, Type::ACERO, 2.0f);
        set(Type::FUEGO, Type::AGUA, 0.5f);      set(Type::FUEGO, Type::ROCA, 0.5f);
        set(Type::FUEGO, Type::FUEGO, 0.5f);     set(Type::FUEGO, Type::DRAGON, 0.5f);

        set(Type::AGUA, Type::FUEGO, 2.0f);      set(Type::AGUA, Type::TIERRA, 2.0f);
        set(Type::AGUA, Type::ROCA, 2.0f);       set(Type::AGUA, Type::AGUA, 0.5f);
        set(Type::AGUA, Type::PLANTA, 0.5f);     set(Type::AGUA, Type::DRAGON, 0.5f);

        set(Type::PLANTA, Type::AGUA, 2.0f);     set(Type::PLANTA, Type::TIERRA, 2.0f);
        set(Type::PLANTA, Type::ROCA, 2.0f);     set(Type::PLANTA, Type::FUEGO, 0.5f);
        set(Type::PLANTA, Type::PLANTA, 0.5f);   set(Type::PLANTA, Type::VENENO, 0.5f);
        set(Type::PLANTA, Type::VOLADOR, 0.5f);  set(Type::PLANTA, Type::BICHO, 0.5f);
        set(Type::PLANTA, Type::DRAGON, 0.5f);   set(Type::PLANTA, Type::ACERO, 0.5f);

        set(Type::ELECTRICO, Type::AGUA, 2.0f);    set(Type::ELECTRICO, Type::VOLADOR, 2.0f);
        set(Type::ELECTRICO, Type::PLANTA, 0.5f);  set(Type::ELECTRICO, Type::ELECTRICO, 0.5f);
        set(Type::ELECTRICO, Type::DRAGON, 0.5f);  set(Type::ELECTRICO, Type::TIERRA, 0.0f);

        set(Type::HIELO, Type::PLANTA, 2.0f);    set(Type::HIELO, Type::TIERRA, 2.0f);
        set(Type::HIELO, Type::VOLADOR, 2.0f);   set(Type::HIELO, Type::DRAGON, 2.0f);
        set(Type::HIELO, Type::AGUA, 0.5f);      set(Type::HIELO, Type::HIELO, 0.5f);
        set(Type::HIELO, Type::ACERO, 0.5f);     set(Type::HIELO, Type::FUEGO, 0.5f);

        set(Type::LUCHA, Type::NORMAL, 2.0f);    set(Type::LUCHA, Type::HIELO, 2.0f);
        set(Type::LUCHA, Type::ROCA, 2.0f);      set(Type::LUCHA, Type::SINIESTRO, 2.0f);
        set(Type::LUCHA, Type::ACERO, 2.0f);     set(Type::LUCHA, Type::VENENO, 0.5f);
        set(Type::LUCHA, Type::VOLADOR, 0.5f);   set(Type::LUCHA, Type::PSIQUICO, 0.5f);
        set(Type::LUCHA, Type::BICHO, 0.5f);     set(Type::LUCHA, Type::FANTASMA, 0.0f);

        set(Type::VENENO, Type::PLANTA, 2.0f);   set(Type::VENENO, Type::ROCA, 0.5f);
        set(Type::VENENO, Type::FANTASMA, 0.5f); set(Type::VENENO, Type::TIERRA, 0.5f);
        set(Type::VENENO, Type::VENENO, 0.5f);   set(Type::VENENO, Type::ACERO, 0.0f);

        set(Type::TIERRA, Type::FUEGO, 2.0f);    set(Type::TIERRA, Type::ELECTRICO, 2.0f);
        set(Type::TIERRA, Type::VENENO, 2.0f);   set(Type::TIERRA, Type::ROCA, 2.0f);
        set(Type::TIERRA, Type::ACERO, 2.0f);    set(Type::TIERRA, Type::PLANTA, 0.5f);
        set(Type::TIERRA, Type::BICHO, 0.5f);    set(Type::TIERRA, Type::VOLADOR, 0.0f);

        set(Type::VOLADOR, Type::PLANTA, 2.0f);  set(Type::VOLADOR, Type::LUCHA, 2.0f);
        set(Type::VOLADOR, Type::BICHO, 2.0f);   set(Type::VOLADOR, Type::ROCA, 0.5f);
        set(Type::VOLADOR, Type::ELECTRICO, 0.5f); set(Type::VOLADOR, Type::ACERO, 0.5f);

        set(Type::PSIQUICO, Type::LUCHA, 2.0f);  set(Type::PSIQUICO, Type::VENENO, 2.0f);
        set(Type::PSIQUICO, Type::PSIQUICO, 0.5f); set(Type::PSIQUICO, Type::ACERO, 0.5f);
        set(Type::PSIQUICO, Type::SINIESTRO, 0.0f);

        set(Type::BICHO, Type::PLANTA, 2.0f);    set(Type::BICHO, Type::PSIQUICO, 2.0f);
        set(Type::BICHO, Type::SINIESTRO, 2.0f); set(Type::BICHO, Type::FUEGO, 0.5f);
        set(Type::BICHO, Type::LUCHA, 0.5f);     set(Type::BICHO, Type::VENENO, 0.5f);
        set(Type::BICHO, Type::VOLADOR, 0.5f);   set(Type::BICHO, Type::FANTASMA, 0.5f);
        set(Type::BICHO, Type::ACERO, 0.5f);

        set(Type::ROCA, Type::FUEGO, 2.0f);      set(Type::ROCA, Type::HIELO, 2.0f);
        set(Type::ROCA, Type::VOLADOR, 2.0f);    set(Type::ROCA, Type::BICHO, 2.0f);
        set(Type::ROCA, Type::LUCHA, 0.5f);      set(Type::ROCA, Type::TIERRA, 0.5f);
        set(Type::ROCA, Type::ACERO, 0.5f);

        set(Type::FANTASMA, Type::PSIQUICO, 2.0f); set(Type::FANTASMA, Type::FANTASMA, 2.0f);
        set(Type::FANTASMA, Type::SINIESTRO, 0.5f); set(Type::FANTASMA, Type::NORMAL, 0.0f);

        set(Type::DRAGON, Type::DRAGON, 2.0f);   set(Type::DRAGON, Type::ACERO, 0.5f);

        set(Type::SINIESTRO, Type::PSIQUICO, 2.0f); set(Type::SINIESTRO, Type::FANTASMA, 2.0f);
        set(Type::SINIESTRO, Type::LUCHA, 0.5f);    set(Type::SINIESTRO, Type::SINIESTRO, 0.5f);

        set(Type::ACERO, Type::HIELO, 2.0f);     set(Type::ACERO, Type::ROCA, 2.0f);
        set(Type::ACERO, Type::FUEGO, 0.5f);     set(Type::ACERO, Type::AGUA, 0.5f);
        set(Type::ACERO, Type::ELECTRICO, 0.5f); set(Type::ACERO, Type::ACERO, 0.5f);
    }

    void set(Type atk, Type def, float val) { chart[idx(atk)][idx(def)] = val; }
    int  idx(Type t) const { return static_cast<int>(t); }
};

// ============================================================
//  MOVIMIENTOS
// ============================================================

enum class Category { FISICO, ESPECIAL, ESTADO };

struct Move {
    string name;
    Type        type;
    Category    category;
    int         power;
    int         accuracy;
    int         priority;   // prioridad del movimiento (para el orden del turno)
};

// ============================================================
//  POKEMON
// ============================================================

struct Stats { int hp, attack, defense, spAtk, spDef, speed; };

struct Pokemon {
    string name;
    Type        type1;
    Type        type2;
    Stats       base;
    int         maxHP;
    int         currentHP;
    vector<Move> moves;

    bool isFainted() const { return currentHP <= 0; }

    void takeDamage(int dmg) { currentHP = max(0, currentHP - dmg); }

    // Compara Tipo del ataque vs Tipos del defensor (este Pokemon)
    float typeMultiplierReceived(Type attackType) const {
        auto& tc = TypeChart::getInstance();
        float mult = tc.getMultiplier(attackType, type1);
        if (type2 != Type::NINGUNO) mult *= tc.getMultiplier(attackType, type2);
        return mult;
    }

    void displayHPBar() const {
        int filled = maxHP > 0 ? (currentHP * 20) / maxHP : 0;
        cout << "HP [";
        for (int i = 0; i < 20; ++i) cout << (i < filled ? "=" : " ");
        cout << "] " << currentHP << "/" << maxHP << "\n";
    }
};

// ============================================================
//  ENTRENADOR (Jugador o IA)
// ============================================================

struct Trainer {
    string name;
    vector<Pokemon> team;   // equipo de 6 Pokemon
    int active = 0;

    Pokemon&       activePokemon()       { return team[active]; }
    const Pokemon& activePokemon() const { return team[active]; }

    // ¿Quedan Pokemon en el equipo?
    bool hasUsablePokemon() const {
        for (const auto& p : team) if (!p.isFainted()) return true;
        return false;
    }

    // Enviar siguiente Pokemon disponible
    int nextAvailableIndex() const {
        for (size_t i = 0; i < team.size(); ++i)
            if (!team[i].isFainted()) return static_cast<int>(i);
        return -1;
    }
};

// ============================================================
//  BASE DE DATOS DE MOVIMIENTOS Y POKEMON (en memoria)
// ============================================================

vector<Move> buildMovePool() {
    return {
        {"Lanzallamas",  Type::FUEGO,     Category::ESPECIAL, 90,  100, 0},
        {"Llamarada",    Type::FUEGO,     Category::ESPECIAL, 110, 85,  0},
        {"Hidrobomba",   Type::AGUA,      Category::ESPECIAL, 110, 80,  0},
        {"Pistola Agua", Type::AGUA,      Category::ESPECIAL, 40,  100, 0},
        {"Latigo Cepa",  Type::PLANTA,    Category::FISICO,   45,  100, 0},
        {"Rayo Solar",   Type::PLANTA,    Category::ESPECIAL, 120, 100, 0},
        {"Trueno",       Type::ELECTRICO, Category::ESPECIAL, 110, 70,  0},
        {"Impactrueno",  Type::ELECTRICO, Category::ESPECIAL, 40,  100, 0},
        {"Mordisco",     Type::SINIESTRO, Category::FISICO,   60,  100, 0},
        {"Ataque Roca",  Type::ROCA,      Category::FISICO,   50,  90,  0},
        {"Psicorrayo",   Type::PSIQUICO,  Category::ESPECIAL, 65,  100, 0},
        {"Cuchillada",   Type::NORMAL,    Category::FISICO,   70,  100, 0},
        {"Cabezazo",     Type::NORMAL,    Category::FISICO,   70,  100, 0},
        {"Bola Sombra",  Type::FANTASMA,  Category::ESPECIAL, 80,  100, 0},
        {"Rayo Hielo",   Type::HIELO,     Category::ESPECIAL, 90,  100, 0},
        {"Tajo Aereo",   Type::VOLADOR,   Category::FISICO,   60,  95,  0},
        {"Garra Metal",  Type::ACERO,     Category::FISICO,   50,  95,  0},
        {"Terratemblor", Type::TIERRA,    Category::FISICO,   100, 100, 0},
        {"Ataque Rapido",Type::NORMAL,    Category::FISICO,   40,  100, 1}, // prioridad +1
        {"Proteccion",   Type::NORMAL,    Category::ESTADO,   0,   100, 4}  // prioridad +4
    };
}

Move findMove(const vector<Move>& pool, const string& name) {
    for (const auto& m : pool) if (m.name == name) return m;
    return pool[0]; // fallback de seguridad
}

vector<Pokemon> buildPokemonPool(const vector<Move>& mp) {
    return {
        {"Charizard", Type::FUEGO, Type::VOLADOR, {156,130,111,159,111,150}, 156, 156,
            {findMove(mp,"Lanzallamas"), findMove(mp,"Llamarada"), findMove(mp,"Tajo Aereo"), findMove(mp,"Ataque Rapido")}},
        {"Blastoise", Type::AGUA, Type::NINGUNO, {164,138,164,141,154,120}, 164, 164,
            {findMove(mp,"Hidrobomba"), findMove(mp,"Pistola Agua"), findMove(mp,"Ataque Roca"), findMove(mp,"Cabezazo")}},
        {"Venusaur", Type::PLANTA, Type::VENENO, {160,132,142,142,150,120}, 160, 160,
            {findMove(mp,"Latigo Cepa"), findMove(mp,"Rayo Solar"), findMove(mp,"Cabezazo"), findMove(mp,"Proteccion")}},
        {"Pikachu", Type::ELECTRICO, Type::NINGUNO, {110,112,83,112,102,180}, 110, 110,
            {findMove(mp,"Trueno"), findMove(mp,"Impactrueno"), findMove(mp,"Cuchillada"), findMove(mp,"Ataque Rapido")}},
        {"Gengar", Type::FANTASMA, Type::VENENO, {108,104,86,162,106,164}, 108, 108,
            {findMove(mp,"Bola Sombra"), findMove(mp,"Psicorrayo"), findMove(mp,"Mordisco"), findMove(mp,"Proteccion")}},
        {"Tyranitar", Type::ROCA, Type::SINIESTRO, {200,202,168,146,150,130}, 200, 200,
            {findMove(mp,"Ataque Roca"), findMove(mp,"Mordisco"), findMove(mp,"Terratemblor"), findMove(mp,"Cuchillada")}},
        {"Lapras", Type::AGUA, Type::HIELO, {190,124,128,144,154,108}, 190, 190,
            {findMove(mp,"Rayo Hielo"), findMove(mp,"Hidrobomba"), findMove(mp,"Cabezazo"), findMove(mp,"Proteccion")}},
        {"Steelix", Type::ACERO, Type::TIERRA, {170,180,260,110,140,90}, 170, 170,
            {findMove(mp,"Garra Metal"), findMove(mp,"Terratemblor"), findMove(mp,"Ataque Roca"), findMove(mp,"Cuchillada")}},
    };
}

// ============================================================
//  GENERAR EQUIPO ALEATORIO DE 6 POKEMON
// ============================================================

Trainer generarEquipoAleatorio(const string& nombre, const vector<Pokemon>& pool) {
    Trainer t;
    t.name = nombre;
    vector<int> indices;
    for (size_t i = 0; i < pool.size(); ++i) indices.push_back(static_cast<int>(i));

    for (int i = 0; i < 6 && !indices.empty(); ++i) {
        int r = rand() % indices.size();
        t.team.push_back(pool[indices[r]]);
        indices.erase(indices.begin() + r);
    }
    return t;
}

// ============================================================
//  ELEGIR POKEMON ACTIVO
// ============================================================

void elegirPokemonActivoJugador(Trainer& jugador) {
    cout << "\n=== Elige tu Pokemon activo ===\n";
    for (size_t i = 0; i < jugador.team.size(); ++i)
        cout << "  " << (i + 1) << ". " << jugador.team[i].name
                  << " [" << typeToString(jugador.team[i].type1) << "]\n";

    int choice;
    while (true) {
        cout << "Opcion: ";
        cin >> choice;
        choice--;
        if (choice >= 0 && choice < static_cast<int>(jugador.team.size())) break;
        cout << "Seleccion invalida.\n";
    }
    jugador.active = choice;
}

void elegirPokemonActivoIA(Trainer& ia) {
    ia.active = 0; // la IA simplemente envia su primer Pokemon
}

// ============================================================
//  SELECCIONAR ATAQUE
// ============================================================

int seleccionarAtaqueJugador(const Pokemon& activo) {
    cout << "\nMovimientos de " << activo.name << ":\n";
    for (size_t i = 0; i < activo.moves.size(); ++i) {
        const auto& m = activo.moves[i];
        cout << "  " << (i + 1) << ". " << m.name
                  << " [" << typeToString(m.type) << "]"
                  << " Poder:" << m.power << " Precision:" << m.accuracy << "\n";
    }
    int choice;
    while (true) {
        cout << "Elige ataque: ";
        cin >> choice;
        choice--;
        if (choice >= 0 && choice < static_cast<int>(activo.moves.size())) return choice;
        cout << "Seleccion invalida.\n";
    }
}

// IA simple: elige el movimiento con mayor daño esperado segun tipo
int seleccionarAtaqueIA(const Pokemon& atacante, const Pokemon& defensor) {
    int best = 0;
    float bestScore = -1.0f;
    for (size_t i = 0; i < atacante.moves.size(); ++i) {
        const auto& m = atacante.moves[i];
        float eff = defensor.typeMultiplierReceived(m.type);
        float score = m.power * eff;
        if (score > bestScore) { bestScore = score; best = static_cast<int>(i); }
    }
    return best;
}

// ============================================================
//  RESOLVER ATAQUE (daño: tipo + ataque/defensa)
// ============================================================

int calcularDano(const Pokemon& atk, const Pokemon& def, const Move& move, float& efectividad) {
    int atkStat = (move.category == Category::FISICO) ? atk.base.attack : atk.base.spAtk;
    int defStat = (move.category == Category::FISICO) ? def.base.defense : def.base.spDef;

    // Comparar Tipo del ataque vs Tipos del defensor -> multiplicador
    efectividad = def.typeMultiplierReceived(move.type);

    float stab = (move.type == atk.type1 || move.type == atk.type2) ? 1.5f : 1.0f;
    float rnd  = (85 + rand() % 16) / 100.0f;

    float base = ((2.0f * 50 / 5 + 2) * move.power * atkStat / defStat) / 50 + 2;
    return static_cast<int>(base * stab * efectividad * rnd);
}

bool hitCheck(int accuracy) {
    if (accuracy <= 0) return true;
    return (rand() % 100) < accuracy;
}

// Resolver ataque -> Aplicar daño y efectos -> mostrar resultado
void resolverAtaque(Trainer& atacante, Trainer& defensor, int moveIndex) {
    Pokemon& atk = atacante.activePokemon();
    Pokemon& def = defensor.activePokemon();
    const Move& move = atk.moves[moveIndex];

    cout << "\n" << atk.name << " (" << atacante.name << ") usa " << move.name << "!\n";

    if (!hitCheck(move.accuracy)) {
        cout << "El ataque fallo!\n";
        return;
    }
    if (move.power == 0) {
        cout << "(Movimiento de estado, sin daño directo)\n";
        return;
    }

    float efectividad;
    int dmg = calcularDano(atk, def, move, efectividad);

    if (efectividad == 0.0f) {
        cout << "No afecta a " << def.name << "...\n";
        return;
    } else if (efectividad >= 2.0f) {
        cout << "Es muy efectivo! (x2)\n";
    } else if (efectividad <= 0.5f) {
        cout << "No es muy efectivo... (x0.5)\n";
    }

    def.takeDamage(dmg);
    cout << def.name << " recibio " << dmg << " de dano. ";
    def.displayHPBar();
}

// ============================================================
//  ENVIAR SIGUIENTE POKEMON DISPONIBLE
// ============================================================

bool enviarSiguientePokemon(Trainer& trainer) {
    int next = trainer.nextAvailableIndex();
    if (next == -1) return false; // no quedan Pokemon
    trainer.active = next;
    cout << trainer.name << " envia a " << trainer.activePokemon().name << "!\n";
    return true;
}

// ============================================================
//  DETERMINAR ORDEN DEL TURNO (velocidad + prioridad + azar)
// ============================================================

bool jugadorVaPrimero(const Pokemon& jugadorPoke, const Move& movJugador,
                       const Pokemon& iaPoke, const Move& movIA) {
    // 1. Prioridad del movimiento
    if (movJugador.priority != movIA.priority)
        return movJugador.priority > movIA.priority;

    // 2. Velocidad del Pokemon
    if (jugadorPoke.base.speed != iaPoke.base.speed)
        return jugadorPoke.base.speed > iaPoke.base.speed;

    // 3. Azar en caso de empate
    return (rand() % 2) == 0;
}

// ============================================================
//  BUCLE PRINCIPAL DE BATALLA (sigue el diagrama paso a paso)
// ============================================================

void ejecutarBatalla(Trainer& jugador, Trainer& ia) {
    int turno = 0;

    // "¿Ambos equipos con Pokemon?" controla el bucle de turnos
    while (jugador.hasUsablePokemon() && ia.hasUsablePokemon()) {
        ++turno;
        cout << "\n------------------------------------------\n";
        cout << "  INICIO DEL TURNO " << turno << "\n";
        cout << "------------------------------------------\n";
        cout << "Jugador -> "; jugador.activePokemon().displayHPBar();
        cout << "IA      -> "; ia.activePokemon().displayHPBar();

        // Seleccionar ataque (Jugador e IA)
        int moveJugador = seleccionarAtaqueJugador(jugador.activePokemon());
        int moveIA       = seleccionarAtaqueIA(ia.activePokemon(), jugador.activePokemon());

        // Determinar orden del turno (velocidad + prioridad + azar)
        bool jPrimero = jugadorVaPrimero(jugador.activePokemon(), jugador.activePokemon().moves[moveJugador],
                                          ia.activePokemon(), ia.activePokemon().moves[moveIA]);

        Trainer* first  = jPrimero ? &jugador : &ia;
        Trainer* second = jPrimero ? &ia      : &jugador;
        Trainer& other1 = jPrimero ? ia       : jugador;
        Trainer& other2 = jPrimero ? jugador  : ia;
        int firstMove   = jPrimero ? moveJugador : moveIA;
        int secondMove  = jPrimero ? moveIA      : moveJugador;

        // Resolver ataque 1
        resolverAtaque(*first, other1, firstMove);

        // ¿Defensor debilitado?
        if (other1.activePokemon().isFainted()) {
            cout << "\n" << other1.activePokemon().name << " se debilito!\n";
            // ¿Quedan Pokemon en el equipo?
            if (!enviarSiguientePokemon(other1)) {
                // No quedan -> termina la batalla, se evalua fuera del bucle
                break;
            }
            // Continuar -> el turno termina aqui (no se ejecuta el 2do ataque)
            continue;
        }

        // Resolver ataque 2 (solo si el defensor 1 no fue debilitado)
        resolverAtaque(*second, other2, secondMove);

        if (other2.activePokemon().isFainted()) {
            cout << "\n" << other2.activePokemon().name << " se debilito!\n";
            if (!enviarSiguientePokemon(other2)) {
                break;
            }
        }
        // Continuar -> siguiente turno (vuelve al inicio del while)
    }

    // ¿Equipo del Jugador sin Pokemon?
    cout << "\n==========================================\n";
    if (!jugador.hasUsablePokemon()) {
        cout << "   GANA LA IA\n";
    } else {
        cout << "   GANA EL JUGADOR\n";
    }
    cout << "==========================================\n";
}

// ============================================================
//  MAIN - Iniciar partida
// ============================================================

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "============================================\n";
    cout << "     POKEMON STADIUM - BATALLA (1 ARCHIVO)\n";
    cout << "============================================\n";

    auto movePool = buildMovePool();
    auto pokePool = buildPokemonPool(movePool);

    bool jugarDeNuevo = true;
    while (jugarDeNuevo) {
        // Generar equipo aleatorio de 6 Pokemon (Jugador) / (IA)
        Trainer jugador = generarEquipoAleatorio("Jugador", pokePool);
        Trainer ia      = generarEquipoAleatorio("IA", pokePool);

        cout << "\nTu equipo generado:\n";
        for (auto& p : jugador.team) cout << "  - " << p.name << "\n";
        cout << "\nEquipo de la IA generado:\n";
        for (auto& p : ia.team) cout << "  - " << p.name << "\n";

        // Elegir Pokemon activo (Jugador) / (IA)
        elegirPokemonActivoJugador(jugador);
        elegirPokemonActivoIA(ia);

        cout << "\n¡Comienza la batalla!\n";
        ejecutarBatalla(jugador, ia);

        cout << "\n¿Jugar de nuevo? (1 = si, 0 = no): ";
        int again;
        cin >> again;
        jugarDeNuevo = (again == 1);
    }

    cout << "\nGracias por jugar!\n";
    return 0;
}