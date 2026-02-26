#include "fen.hpp"

const std::string pieces = "PRNBKQprnbkq";
constexpr piece NO_PIECE = 255;

// Convert a character to its corresponding piece
piece char_to_piece (char ch) {
    piece pc = NO_PIECE;

    switch (ch) {
        case 'P':
            pc = P; break;
        case 'R':
            pc = R; break;
        case 'N':
            pc = N; break;    
        case 'B':
            pc = B; break;
        case 'K':
            pc = K; break;
        case 'Q':
            pc = Q; break;
        case 'p':
            pc = p; break;
        case 'r':
            pc = r; break;
        case 'n':
            pc = n; break;    
        case 'b':
            pc = b; break;
        case 'k':
            pc = k; break;
        case 'q':
            pc = q; break;    
        default:
            break;
    }

    assert(pc != NO_PIECE);
    return pc;
}

// Convert a string to its corresponding square
square str_to_sq (const std::string &str) {
    assert(str.length() == 2);

    return (str[1] - '1') * 8 + (str[0] - 'a');
}

// Check if a character represents a valid piece
bool is_piece (char character) {
    return (pieces.find(character) != std::string::npos);
}

// Convert a string to an integer
// Return -1 if conversion fails
int16_t str_to_int (const std::string &str) {
    counter cnt = 0;
    for (char ch : str) {
        if (isdigit(ch)) {
            cnt = cnt * 10 + (ch - '0');
        } else {
            return -1;
        }
    }

    return cnt;
}

// Split FEN string in different fields
bool split_str (const std::string &position, std::string &side_to_move, std::string &board,
    std::string &castling_rights, std::string &en_passant, std::string &hm_clock, std::string &fm_clock) {
    counter cnt = 0;
    std::string fields[6];
    size_t start = 0, end;

    while ((end = position.find(' ', start)) != std::string::npos) {
        if (cnt >= 5) {
            std::cerr << "Too many spaces.\n";
            return false;
        }
        fields[cnt] = position.substr(start, end - start);
        start = end + 1;
        cnt++;
    }

    fields[cnt] = position.substr(start);
    cnt++;

    if (cnt != 6) {
        std::cerr << "FEN must have 6 fields.\n";
        return false;
    }

    board = fields[0];
    side_to_move = fields[1];
    castling_rights = fields[2];
    en_passant = fields[3];
    hm_clock = fields[4];
    fm_clock = fields[5];

    return true;
}

// Set board according to the corresponding FEN field
bool set_board (const std::string &board) {
    uint8_t chindex = 0;
    for (int8_t r = 7; r >= 0; r--) {
        for (int8_t f = 0; f <= 7; f++) {
            square sq = r * 8 + f;
            char ch = board[chindex];

            if (is_piece(ch)) {
                piece pc = char_to_piece(ch);
                setbit(pos.bitboards[pc], sq);
            } else if ('1' <= ch && ch <= '8') {
                uint8_t offset = ch - '0';
                f += offset - 1;
            } else {
                std::cerr << "Invalid FEN character in piece placement.\n";
                return false;   
            }

            chindex++;
        }

        if (board[chindex] == '/') {
            chindex++;
        }
    }

    return true;
}

// Set castling rights according to the corresponding FEN field
bool set_castling_rights (const std::string &castling_rights) {
    for (char ch : castling_rights) {
        if (ch != '-' && ch != 'K' && ch != 'k' && ch != 'Q' && ch != 'q') {
            std::cerr << "Invalid castling character.\n";
            return false;
        }

        switch (ch) {
            case 'K':
                pos.castling |= wk; break;
            case 'Q':
                pos.castling |= wq; break;
            case 'k':
                pos.castling |= bk; break;
            case 'q':
                pos.castling |= bq; break;
            default:
                break;
        }
    }

    return true;
}

// Set en passant according to the corresponding FEN field
bool set_en_passant (const std::string &en_passant) {
    if (en_passant == "-") {
        pos.enpassant = no_sq;
    } else if (en_passant.length() != 2
                || !('a' <= en_passant[0] && en_passant[0] <= 'h')
                || !('1' <= en_passant[1] && en_passant[1] <= '8')) {
        std::cerr << "Invalid en passant square.\n";
        return false;
    } else {
        pos.enpassant = str_to_sq(en_passant);
    }

    return true;
}

// Parse a FEN string and set up board state
// Invalid FEN strings generate an error but make the board state invalid
void parse_fen (const std::string &position) {
    // Reset state variables
    memset(pos.bitboards, 0ULL, sizeof(pos.bitboards));
    memset(pos.occupancies, 0ULL, sizeof(pos.occupancies));
    pos.stm = 0;
    pos.enpassant = no_sq;
    pos.castling = 0;
    pos.hmclock = 0;
    pos.fmclock = 0;

    std::string side_to_move;
    std::string board;
    std::string castling_rights;
    std::string en_passant;
    std::string hm_clock;
    std::string fm_clock;

    if (!split_str(
        position,
        side_to_move,
        board,
        castling_rights,
        en_passant,
        hm_clock,
        fm_clock
    )) {
        parse_fen(starting_pos);
        return;
    }

    if (side_to_move != "w" && side_to_move != "b") {
        std::cerr << "Side must be 'w' or 'b'.\n";
        parse_fen(starting_pos);
        return;
    }

    pos.stm = (side_to_move == "w") ? white : black;

    if (!set_board(board)) {
        parse_fen(starting_pos);
        return;
    }

    if (!set_castling_rights(castling_rights)) {
        parse_fen(starting_pos);
        return;
    }

    if (!set_en_passant(en_passant)) {
        parse_fen(starting_pos);
        return;
    }

    uint8_t half_move_clock = str_to_int(hm_clock);
    if (half_move_clock < 0) {
        std::cerr << "Invalid half move clock value.\n";
        parse_fen(starting_pos);
        return;
    }
    pos.hmclock = half_move_clock;

    uint16_t full_move_clock = str_to_int(fm_clock);
    if (full_move_clock < 0) {
        std::cerr << "Invalid full move clock value.\n";
        parse_fen(starting_pos);
        return;
    }
    pos.fmclock = full_move_clock;

    // Update occupancies
    for (piece pc = P; pc <= Q; pc++) {
        pos.occupancies[white] |= pos.bitboards[pc];
    }
    for (piece pc = p; pc <= q; pc++) {
        pos.occupancies[black] |= pos.bitboards[pc];
    }
    pos.occupancies[both] |= pos.occupancies[white] | pos.occupancies[black];
}
