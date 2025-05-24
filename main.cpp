#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <queue>      // <-- for std::queue
#include <random>     // <-- for mt19937, uniform_int_distribution
#include <ctime>      // <-- for time()
using namespace std;

enum Move { ROCK = 1, PAPER, SCISSORS, BAD };

Move toMove(const string &tok)               // accept 1/2/3 or words
{
    if (tok == "1") return ROCK;
    if (tok == "2") return PAPER;
    if (tok == "3") return SCISSORS;

    string t;                                 // lower-case copy
    transform(tok.begin(), tok.end(), back_inserter(t), ::tolower);
    if (t == "rock"     || t == "r") return ROCK;
    if (t == "paper"    || t == "p") return PAPER;
    if (t == "scissors" || t == "s") return SCISSORS;

    return BAD;
}

const char *toText(Move m)
{
    return m == ROCK ? "Rock" :
           m == PAPER ? "Paper" : "Scissors";
}

/* rotate computer’s move when equal to player’s => no draw */
Move fix(Move player, Move comp)
{
    if (player != comp) return comp;
    return player == ROCK ? SCISSORS :
           player == PAPER ? ROCK      : PAPER;
}

int winner(Move p, Move c)                    // 1 = player, -1 = computer
{
    return (p == ROCK     && c == SCISSORS) ||
           (p == PAPER    && c == ROCK)     ||
           (p == SCISSORS && c == PAPER)    ?  1 : -1;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---------------- prompt text (exactly as required) ---------------- */
    cout << "Welcome to the Rock, Paper, Scissors Game!\n\n"
         << "Please enter your name, number of repetitions, \n"
         << "and your chosen steps\n"
         << "1 - Rock, \n"
         << "2 - Paper, \n"
         << "3 - Scissors:\n \n"
         << "> ";
    cout.flush();

    /* ---------------- read first line ---------------- */
    string line;
    getline(cin, line);
    while (line.empty()) getline(cin, line);          // ignore blank

    stringstream ss(line);
    string player;
    int rounds;
    if (!(ss >> player >> rounds)) return 0;          // malformed

    queue<Move> moves;
    string tok;
    while (ss >> tok) {
        Move mv = toMove(tok);
        if (mv != BAD) moves.push(mv);
    }

    cout << "\nGame Start!\n";

    mt19937 rng{static_cast<unsigned>(time(nullptr))};
    uniform_int_distribution<int> rnd(1, 3);

    int pScore = 0, cScore = 0;

    for (int r = 1; r <= rounds; ++r) {
        /* make sure we have ONE player move */
        while (moves.empty()) {                 // try to pull one token
            if (!(cin >> tok)) {                // EOF ⇒ default to Rock
                moves.push(ROCK);
                break;
            }
            Move mv = toMove(tok);
            if (mv != BAD) moves.push(mv);      // ignore garbage tokens
        }
        Move playerMove = moves.front(); moves.pop();

        Move compMove = fix(playerMove, static_cast<Move>(rnd(rng)));
        int res = winner(playerMove, compMove);       // never 0

        if (res == 1) ++pScore; else ++cScore;

        cout << "Round " << r << ": Computer chose " << toText(compMove)
             << ", " << player << " chose " << toText(playerMove)
             << ". Winner: " << (res == 1 ? player : "Computer") << ".\n";
    }

    cout << "\nGame Over!\n"
         << "Total Score - " << player << ": " << pScore
         << ", Computer: " << cScore << '\n'
         << "Would you like to play again? (yes/no)";
    return 0;
}
