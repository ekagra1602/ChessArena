#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <memory>

using namespace std;

// Function to communicate with Stockfish and get the best move
string getBestMove(const string& fen) {
    string command = "stockfish";
    FILE* pipe = popen(command.c_str(), "w");
    if (!pipe) {
        cerr << "Error: Could not start Stockfish." << endl;
        return "e2e4"; // Default move in case of failure
    }
    
    // Send UCI commands to Stockfish
    fprintf(pipe, "uci\n");
    fprintf(pipe, "isready\n");
    fprintf(pipe, "position fen %s\n", fen.c_str());
    fprintf(pipe, "go depth 15\n");
    fflush(pipe);
    
    // Read Stockfish output
    char buffer[128];
    string bestMove;
    FILE* output = popen("stockfish", "r");
    if (!output) {
        cerr << "Error: Could not read Stockfish output." << endl;
        return "e2e4";
    }
    
    while (fgets(buffer, 128, output) != NULL) {
        string line(buffer);
        if (line.find("bestmove") != string::npos) {
            bestMove = line.substr(9, 5); // Extract best move
            break;
        }
    }
    
    pclose(pipe);
    pclose(output);
    return bestMove.empty() ? "e2e4" : bestMove;
}

int main() {
    string fen;
    while (getline(cin, fen)) {
        cout << getBestMove(fen) << endl;
    }
    return 0;
}
