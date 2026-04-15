#include <array>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

struct Section {
    std::string heading;
    std::vector<std::string> bullets;
};

namespace ui {
const std::string kReset = "\x1b[0m";
const std::string kAccent = "\x1b[38;5;45m";
const std::string kMuted = "\x1b[38;5;246m";
const std::string kWarn = "\x1b[38;5;214m";
const std::string kGood = "\x1b[38;5;82m";

void divider(char c = '=') {
    std::cout << std::string(78, c) << "\n";
}

void printHeader(const std::string& title, const std::string& subtitle) {
    divider('=');
    std::cout << kAccent << title << kReset << "\n";
    std::cout << kMuted << subtitle << kReset << "\n";
    divider('=');
    std::cout << "\n";
}

void printSection(const Section& section) {
    std::cout << kAccent << section.heading << kReset << "\n";
    divider('-');
    for (const auto& bullet : section.bullets) {
        std::cout << "  - " << bullet << "\n";
    }
    std::cout << "\n";
}

void waitForEnter() {
    std::cout << kMuted << "Press ENTER to return to the menu..." << kReset;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    std::cout << "\n";
}
}

struct GamePitch {
    std::string title;
    std::string logline;
    Section coreLoop;
    Section missionMasterNode;
    Section missionPrimaryCopy;
    Section missionMajority;
    Section mapSystem;
    Section advancedConcepts;
    Section resumeHighlights;
};

GamePitch buildPitch() {
    return GamePitch{
        "LOCKSTATE: Shards of Consensus",
        "In a fractured archive-city made of living data Nodes, a field operative restores order "
        "by turning distributed database principles into cinematic third-person combat systems "
        "across a tightly scoped AA campaign.",
        Section{
            "Core Gameplay Loop (Movement, Combat, Traversal)",
            {
                "Scout local Node sectors with wall-runs, rail slides, and grapple relays while reading live lock heatmaps.",
                "Engage rogue transactions using melee chains, precision sidearms, and control gadgets (rollback pulse, semaphore trap, deadlock breaker).",
                "Manipulate contention in real time by rerouting queues, opening temporary critical sections, and deprioritizing hostile process lanes.",
                "Secure mission objective state, commit progress, and exfiltrate before cascading failures repopulate the space.",
                "AA target: 8-10 hours, 3 major authored missions, one compact operations hub, zero open-world filler."
            }
        },
        Section{
            "Mission 1: Centralized Lock System - Master Node Gridlock",
            {
                "The Master Node is a towering archive where every lock request funnels through one authority, creating visible physical queues and contention spikes.",
                "Player fights through rogue transactions while triaging queue priority to keep critical services alive.",
                "Single-point failure events trigger rotating blackout zones and elite deadlock hunter spawns.",
                "Win condition: stabilize Lock Arbiter throughput above 65 percent long enough to flush backlog and unlock extraction path.",
                "Design takeaway: centralized locking is easy to reason about but brittle under pressure."
            }
        },
        Section{
            "Mission 2: Primary-Copy Locking - Primary Branch Retrieval",
            {
                "Three encrypted write-keys exist only in designated Primary Branch sectors; mirror branches are read-only and can fail safely.",
                "Secondary sectors dynamically go offline during the mission without creating fail state, proving continuity through primary ownership.",
                "Traversal challenge focuses on route planning between active transit lanes and unstable branch connectors.",
                "Payload can be intercepted, forcing a short rollback objective rather than full mission restart.",
                "Design takeaway: primary-copy assignment preserves write authority through partial outages."
            }
        },
        Section{
            "Mission 3: Majority Locking - Quorum Siege (Boss + Systems Puzzle)",
            {
                "A global shield around the antagonist process drops only when quorum is achieved.",
                "Player must broadcast to 5 towers and defend at least 3 simultaneously until consensus commit completes.",
                "Boss forks into parallel aggression routines that pressure different towers and force high-speed rotation.",
                "Player can deploy autonomous sentry threads and temporary tower locks to hold lanes while repositioning.",
                "Design takeaway: majority consensus prioritizes resiliency over perfect speed."
            }
        },
        Section{
            "UI and Map: Partially Replicated Catalogs",
            {
                "Local sector map is always exact; remote sectors are incomplete until discovered, cached, or traded.",
                "Remote intel appears as sticky-note packets with source signature, timestamp, and confidence score.",
                "Stale packets decay visually (faded, jittered markers) and can intentionally mislead if not revalidated.",
                "Players choose between speed using stale intel or safety via verification sweeps, creating meaningful navigation risk.",
                "This turns exploration into a data-trust decision loop rather than passive waypoint following."
            }
        },
        Section{
            "Advanced Concepts (Unique Resume Differentiators)",
            {
                "MVCC Combat Timeline: enemies exist in read snapshots; player can swap to a newer snapshot to reveal fresh hazards at resource cost.",
                "Vector-Clock Stealth: patrol awareness propagates by causal order, allowing players to cut a specific edge and prevent alarm fan-out.",
                "CRDT Territory Control: allied drones and enemy swarms update shared control fields with commutative merges, eliminating desync ambiguity.",
                "2PC Extraction Choice: high-value loot requires prepare and commit from two distant vault controllers, creating tense split-defense moments.",
                "Adaptive Isolation Modes: mission mutators map to Read Committed, Repeatable Read, and Serializable, directly changing enemy spawn consistency.",
                "Failure Injection Events: scripted latency, partition, and clock-skew anomalies force players to adapt strategy under shifting guarantees.",
                "Deterministic Replay Seed: every mission run can export a seed to reproduce topology and contention patterns for balancing and speedrun legitimacy."
            }
        },
        Section{
            "Resume Value Highlights",
            {
                "Mapped abstract distributed systems theory into readable real-time player mechanics.",
                "Designed mission structures that teach architecture trade-offs through level goals and fail states.",
                "Defined uncertainty-aware UI with stale-data handling and confidence-driven navigation.",
                "Built a focused AA scope plan with replayable systems depth instead of open-world feature sprawl."
            }
        }
    };
}

void printQuickPitch(const GamePitch& pitch) {
    ui::printHeader("LOCKSTATE: Resume-Friendly GDD", "Third-Person Mission Action x Distributed Systems");
    std::cout << ui::kAccent << "Game Title: " << ui::kReset << pitch.title << "\n\n";
    std::cout << ui::kAccent << "Logline" << ui::kReset << "\n";
    ui::divider('-');
    std::cout << pitch.logline << "\n\n";
    ui::printSection(pitch.coreLoop);
    ui::printSection(pitch.resumeHighlights);
}

void printAdvancedOnly(const GamePitch& pitch) {
    ui::printHeader("Advanced Design Systems", "Unique concepts with strong resume signal");
    ui::printSection(pitch.advancedConcepts);
    ui::printSection(pitch.resumeHighlights);
}

void printMissionBoardUI(const GamePitch& pitch) {
    (void)pitch;
    ui::printHeader("Mission Board UI Prototype", "Diegetic operations console for portfolio demos");

    std::array<std::string, 5> towerState = {
        "Tower A: LOCKED + defended",
        "Tower B: LOCKED + contested",
        "Tower C: UNSTABLE (packet loss)",
        "Tower D: LOCKED + defended",
        "Tower E: OFFLINE (partition)"
    };

    std::cout << "+------------------- LOCAL NODE HUD -------------------+\n";
    std::cout << "| Sector: Quorum Basin     Lock Heat: 72% (HIGH)       |\n";
    std::cout << "| Throughput: 18 txn/s     Queue Depth: 43             |\n";
    std::cout << "| Isolation Mode: Repeatable Read                        |\n";
    std::cout << "+-------------------------------------------------------+\n\n";

    std::cout << "+------------- REMOTE CATALOG STICKY NOTES ------------+\n";
    std::cout << "| Branch-9  | last seen 00:34 | confidence 0.91 | FRESH |\n";
    std::cout << "| Branch-12 | last seen 02:42 | confidence 0.47 | STALE |\n";
    std::cout << "| Branch-18 | last seen 05:10 | confidence 0.22 | RISKY |\n";
    std::cout << "+-------------------------------------------------------+\n\n";

    std::cout << ui::kAccent << "Quorum Towers" << ui::kReset << "\n";
    ui::divider('-');
    for (const auto& line : towerState) {
        bool good = line.find("LOCKED") != std::string::npos && line.find("contested") == std::string::npos;
        bool warn = line.find("contested") != std::string::npos || line.find("UNSTABLE") != std::string::npos;
        if (good) {
            std::cout << "  " << ui::kGood << line << ui::kReset << "\n";
        } else if (warn) {
            std::cout << "  " << ui::kWarn << line << ui::kReset << "\n";
        } else {
            std::cout << "  " << ui::kMuted << line << ui::kReset << "\n";
        }
    }

    std::cout << "\n";
    std::cout << ui::kGood << "Consensus Status: 3/5 towers stable -> GLOBAL SHIELD DOWN" << ui::kReset << "\n\n";
    std::cout << ui::kMuted
              << "UI Note: Local data is authoritative. Remote entries are replicated hints that can become stale."
              << ui::kReset << "\n\n";
}

void printFullGDD(const GamePitch& pitch) {
    ui::printHeader("HIGH-LEVEL GAME DESIGN DOCUMENT (AA)", "Distributed architecture as playable action grammar");
    std::cout << ui::kAccent << "Game Title: " << ui::kReset << pitch.title << "\n\n";
    std::cout << ui::kAccent << "Logline" << ui::kReset << "\n";
    ui::divider('-');
    std::cout << pitch.logline << "\n\n";

    ui::printSection(pitch.coreLoop);
    ui::printSection(pitch.missionMasterNode);
    ui::printSection(pitch.missionPrimaryCopy);
    ui::printSection(pitch.missionMajority);
    ui::printSection(pitch.mapSystem);
    ui::printSection(pitch.advancedConcepts);
    ui::printSection(pitch.resumeHighlights);

    std::cout << ui::kMuted << "End of document." << ui::kReset << "\n\n";
}

int readChoice() {
    std::cout << "Choose an option: ";
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return choice;
}

void printMenu() {
    ui::printHeader("LOCKSTATE Design Console", "Interactive GDD showcase");
    std::cout << "1) Quick pitch (resume summary)\n";
    std::cout << "2) Advanced concepts only\n";
    std::cout << "3) UI mission board demo\n";
    std::cout << "4) Full high-level GDD\n";
    std::cout << "5) Playable prototype: Quorum Defense\n";
    std::cout << "6) Exit\n\n";
}

struct Tower {
    int x;
    int y;
    int hp;
    bool locked;
};

struct Enemy {
    int x;
    int y;
    int hp;
};

int manhattan(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

void printQuorumHelp() {
    std::cout << ui::kAccent << "Controls" << ui::kReset << "\n";
    ui::divider('-');
    std::cout << "  w/a/s/d: move\n";
    std::cout << "  f: attack adjacent enemy\n";
    std::cout << "  l: lock current tower\n";
    std::cout << "  h: show help\n";
    std::cout << "  q: quit mission\n\n";
}

void renderQuorumBoard(int width,
                       int height,
                       int px,
                       int py,
                       const std::vector<Tower>& towers,
                       const std::vector<Enemy>& enemies,
                       int playerHp,
                       int turn,
                       int stableTurns,
                       int targetStableTurns) {
    std::vector<std::string> board(static_cast<size_t>(height), std::string(static_cast<size_t>(width), '.'));

    for (const auto& t : towers) {
        if (t.hp <= 0) {
            board[static_cast<size_t>(t.y)][static_cast<size_t>(t.x)] = 'x';
        } else if (t.locked) {
            board[static_cast<size_t>(t.y)][static_cast<size_t>(t.x)] = 'L';
        } else {
            board[static_cast<size_t>(t.y)][static_cast<size_t>(t.x)] = 'T';
        }
    }

    for (const auto& e : enemies) {
        if (e.hp > 0) {
            board[static_cast<size_t>(e.y)][static_cast<size_t>(e.x)] = 'E';
        }
    }

    board[static_cast<size_t>(py)][static_cast<size_t>(px)] = 'P';

    int lockedAlive = 0;
    for (const auto& t : towers) {
        if (t.hp > 0 && t.locked) {
            lockedAlive++;
        }
    }

    ui::printHeader("LOCKSTATE: Quorum Defense", "Playable systems prototype");
    std::cout << "Turn: " << turn
              << " | Player HP: " << playerHp
              << " | Locked Towers Alive: " << lockedAlive << "/5"
              << " | Stable Quorum Turns: " << stableTurns << "/" << targetStableTurns
              << "\n\n";

    std::cout << "Legend: P=Player E=Enemy T=Tower L=Locked x=Offline .=Empty\n\n";
    for (const auto& row : board) {
        std::cout << "  " << row << "\n";
    }
    std::cout << "\n";
}

int findTowerAt(const std::vector<Tower>& towers, int x, int y) {
    for (size_t i = 0; i < towers.size(); ++i) {
        if (towers[i].x == x && towers[i].y == y && towers[i].hp > 0) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int findAdjacentEnemy(const std::vector<Enemy>& enemies, int px, int py) {
    int best = -1;
    int bestDist = 9999;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].hp <= 0) {
            continue;
        }
        const int d = manhattan(px, py, enemies[i].x, enemies[i].y);
        if (d <= 1 && d < bestDist) {
            bestDist = d;
            best = static_cast<int>(i);
        }
    }
    return best;
}

void spawnEnemy(std::vector<Enemy>& enemies, int width, int height, std::mt19937& rng) {
    std::uniform_int_distribution<int> sideDist(0, 3);
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    Enemy e{0, 0, 1};
    const int side = sideDist(rng);
    if (side == 0) {
        e.x = xDist(rng);
        e.y = 0;
    } else if (side == 1) {
        e.x = xDist(rng);
        e.y = height - 1;
    } else if (side == 2) {
        e.x = 0;
        e.y = yDist(rng);
    } else {
        e.x = width - 1;
        e.y = yDist(rng);
    }
    enemies.push_back(e);
}

void moveEnemies(std::vector<Enemy>& enemies,
                 std::vector<Tower>& towers,
                 int px,
                 int py,
                 int& playerHp) {
    for (auto& e : enemies) {
        if (e.hp <= 0) {
            continue;
        }

        int tx = px;
        int ty = py;
        int bestDist = manhattan(e.x, e.y, px, py);
        bool targetPlayer = true;

        for (const auto& t : towers) {
            if (t.hp <= 0) {
                continue;
            }
            const int d = manhattan(e.x, e.y, t.x, t.y);
            if (d < bestDist) {
                bestDist = d;
                tx = t.x;
                ty = t.y;
                targetPlayer = false;
            }
        }

        if (e.x < tx) {
            e.x++;
        } else if (e.x > tx) {
            e.x--;
        } else if (e.y < ty) {
            e.y++;
        } else if (e.y > ty) {
            e.y--;
        }

        if (targetPlayer && e.x == px && e.y == py) {
            playerHp--;
            e.hp = 0;
            continue;
        }

        for (auto& t : towers) {
            if (t.hp > 0 && e.x == t.x && e.y == t.y) {
                t.hp--;
                if (t.hp <= 0) {
                    t.locked = false;
                }
                e.hp = 0;
                break;
            }
        }
    }
}

void playQuorumDefense() {
    const int width = 11;
    const int height = 9;
    const int targetStableTurns = 5;

    int px = width / 2;
    int py = height / 2;
    int playerHp = 7;
    int turn = 1;
    int stableTurns = 0;

    std::vector<Tower> towers = {
        {1, 1, 3, false},
        {width - 2, 1, 3, false},
        {1, height - 2, 3, false},
        {width - 2, height - 2, 3, false},
        {width / 2, 1, 3, false}
    };
    std::vector<Enemy> enemies;

    std::mt19937 rng(static_cast<unsigned int>(std::random_device{}()));

    printQuorumHelp();
    while (true) {
        if (turn % 2 == 1) {
            spawnEnemy(enemies, width, height, rng);
        }
        if (turn % 4 == 0) {
            spawnEnemy(enemies, width, height, rng);
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) {
                          return e.hp <= 0;
                      }),
                      enemies.end());

        renderQuorumBoard(width, height, px, py, towers, enemies, playerHp, turn, stableTurns, targetStableTurns);
        std::cout << "Command (w/a/s/d/f/l/h/q): ";

        char cmd = '\0';
        std::cin >> cmd;

        if (cmd == 'q') {
            std::cout << "Mission aborted.\n\n";
            return;
        }
        if (cmd == 'h') {
            printQuorumHelp();
        }

        int nx = px;
        int ny = py;
        if (cmd == 'w') {
            ny--;
        } else if (cmd == 's') {
            ny++;
        } else if (cmd == 'a') {
            nx--;
        } else if (cmd == 'd') {
            nx++;
        } else if (cmd == 'f') {
            const int idx = findAdjacentEnemy(enemies, px, py);
            if (idx >= 0) {
                enemies[static_cast<size_t>(idx)].hp = 0;
                std::cout << "You terminated a rogue transaction.\n";
            } else {
                std::cout << "No adjacent enemy to attack.\n";
            }
        } else if (cmd == 'l') {
            const int towerIdx = findTowerAt(towers, px, py);
            if (towerIdx >= 0) {
                towers[static_cast<size_t>(towerIdx)].locked = true;
                std::cout << "Tower locked and broadcasting quorum vote.\n";
            } else {
                std::cout << "Stand on a tower to lock it.\n";
            }
        }

        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            px = nx;
            py = ny;
        }

        moveEnemies(enemies, towers, px, py, playerHp);

        int lockedAlive = 0;
        int aliveTowers = 0;
        for (const auto& t : towers) {
            if (t.hp > 0) {
                aliveTowers++;
                if (t.locked) {
                    lockedAlive++;
                }
            }
        }

        if (lockedAlive >= 3) {
            stableTurns++;
        } else {
            stableTurns = 0;
        }

        if (stableTurns >= targetStableTurns) {
            renderQuorumBoard(width, height, px, py, towers, enemies, playerHp, turn, stableTurns, targetStableTurns);
            std::cout << ui::kGood << "Quorum achieved. Global shield collapsed. Mission success." << ui::kReset << "\n\n";
            return;
        }

        if (playerHp <= 0) {
            std::cout << ui::kWarn << "You were overwhelmed by rogue transactions. Mission failed." << ui::kReset << "\n\n";
            return;
        }

        if (aliveTowers < 3) {
            std::cout << ui::kWarn << "Too many towers went offline. Consensus impossible. Mission failed." << ui::kReset << "\n\n";
            return;
        }

        turn++;
    }
}

int main(int argc, char* argv[]) {
    const GamePitch pitch = buildPitch();

    if (argc > 1 && std::string(argv[1]) == "--full") {
        printFullGDD(pitch);
        return 0;
    }

    while (true) {
        printMenu();
        const int choice = readChoice();

        switch (choice) {
            case 1:
                printQuickPitch(pitch);
                ui::waitForEnter();
                break;
            case 2:
                printAdvancedOnly(pitch);
                ui::waitForEnter();
                break;
            case 3:
                printMissionBoardUI(pitch);
                ui::waitForEnter();
                break;
            case 4:
                printFullGDD(pitch);
                ui::waitForEnter();
                break;
            case 5:
                playQuorumDefense();
                break;
            case 6:
                std::cout << "Exiting design console.\n";
                return 0;
            default:
                std::cout << "Invalid selection. Enter 1-6.\n\n";
                break;
        }
    }
}
