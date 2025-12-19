#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <string_view>

namespace mylang
{

    enum TypeIDs : uint32_t
    {
        UNKNOWN = 0,
        KEYWORD,
        IDENTIFIER,
        INTEGER,
        DIGRAPH, 
        SYMBOL
    };

    inline constexpr std::array<std::string_view, 6> type_strings = {
        "unknown",
        "keyword",
        "identifier",
        "integer",
        "digraph",
        "symbol",
    };

    enum TokenIDs : uint32_t
    {
        TOK_UNKNOWN = 0,

        KEYWORD_LET = 1,
        KEYWORD_IF,
        KEYWORD_ELSE,
        KEYWORD_FUNC,
        KEYWORD_RETURN,
        KEYWORD_LOOP,
        KEYWORD_INPUT,
        KEYWORD_OUTPUT,

        DIGRAPH_ASSIGN,
        DIGRAPH_EQUAL,
        DIGRAPH_NOT_EQUAL,

        DIGRAPH_GREATER_EQUAL,
        DIGRAPH_LESS_EQUAL,

        SYM_GREATERTHAN,
        SYM_LESSTHAN,

        SYM_ADD,
        SYM_SUBTRACT,
        SYM_MULTIPLY,
        SYM_DIVIDE,
        SYM_MODULO,

        SYM_BEGIN_BLOCK,
        SYM_END_BLOCK,
        SYM_LEFT_PAREN,
        SYM_RIGHT_PAREN
    };

    inline constexpr std::array<std::string_view, 9> keyword_strings = {
        "let",
        "if",
        "else",
        "func",
        "return",
        "loop",
        "endloop",
        "read",
        "write",
    };

    struct Token
    {
        mylang::TypeIDs type_id;
        mylang::TokenIDs token_id;
        std::string token;
        size_t position;
        std::string compilation_unit;
    };

}