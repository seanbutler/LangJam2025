
#pragma once

#include "tokens.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class Tokeniser
{
public:
    Tokeniser() = default;

    bool tokenise(const std::string& file_path, const std::string& compilation_unit)
    {
        compilation_unit_ = compilation_unit;
        tokens_.clear();
        position_ = 0;

        std::ifstream in(file_path, std::ios::binary);
        if (!in)
        {
            return false;
        }

        source_.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

        while (position_ < source_.size())
        {
            if (std::isspace(static_cast<unsigned char>(source_[position_])))
            {
                ++position_;
                continue;
            }

            if (accept_keyword() 
                || accept_digraph() 
                || accept_symbol() 
                || accept_identifier() 
                || accept_integer())
            {
                continue;
            }

            ++position_;
        }

        return true;
    }

    void printTokens(std::ostream& os = std::cout) const
    {
        for (const auto& tok : tokens_)
        {
            os << tok.compilation_unit << ":" << tok.position << " " << mylang::type_strings[tok.type_id];
                
            if (tok.type_id == mylang::TypeIDs::KEYWORD) 
            {
                os << " " << mylang::keyword_strings[tok.token_id-1]; 
            }
            else
            {
                os << " " << static_cast<uint32_t>(tok.token_id);
            }

            os << " \"" << tok.token << "\"\n";
        }

        os << "\n";
    }

    const std::vector<mylang::Token>& tokens() const { return tokens_; }

private:
    bool accept_keyword()
    {
        for (size_t n = 0; n < mylang::keyword_strings.size(); ++n)
        {
            const std::string_view keyword = mylang::keyword_strings[n];
            const size_t len = keyword.size();
            if (position_ + len <= source_.size() && std::string_view(source_).substr(position_, len) == keyword)
            {
                const auto token_id = static_cast<mylang::TokenIDs>(mylang::KEYWORD_DECL + n);
                tokens_.push_back(mylang::Token{mylang::TypeIDs::KEYWORD, token_id, std::string(keyword), position_, compilation_unit_});
                position_ += len;
                return true;
            }
        }
        return false;
    }

    bool accept_digraph()
    {
        if (position_ + 1 >= source_.size())
        {
            return false;
        }

        const char a = source_[position_];
        const char b = source_[position_ + 1];

        if (a == ':' && b == '=')
        {
            tokens_.push_back(mylang::Token{mylang::TypeIDs::DIGRAPH, mylang::TokenIDs::DIGRAPH_ASSIGN, ":=", position_, compilation_unit_});
            position_ += 2;
            return true;
        }
        if (a == '<' && b == '=')
        {
            tokens_.push_back(mylang::Token{mylang::TypeIDs::DIGRAPH, mylang::TokenIDs::DIGRAPH_LESS_EQUAL, "<=", position_, compilation_unit_});
            position_ += 2;
            return true;
        }
        if (a == '>' && b == '=')
        {
            tokens_.push_back(mylang::Token{mylang::TypeIDs::DIGRAPH, mylang::TokenIDs::DIGRAPH_GREATER_EQUAL, ">=", position_, compilation_unit_});
            position_ += 2;
            return true;
        }
        if (a == '=' && b == '=')
        {
            tokens_.push_back(mylang::Token{mylang::TypeIDs::DIGRAPH, mylang::TokenIDs::DIGRAPH_EQUAL, "==", position_, compilation_unit_});
            position_ += 2;
            return true;
        }
        if (a == '!' && b == '=')
        {
            tokens_.push_back(mylang::Token{mylang::TypeIDs::DIGRAPH, mylang::TokenIDs::DIGRAPH_NOT_EQUAL, "!=", position_, compilation_unit_});
            position_ += 2;
            return true;
        }

        return false;
    }

    bool accept_symbol()
    {
        const char c = source_[position_];
        switch (c)
        {
        case ',':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_COMMA, ",", position_, compilation_unit_});
            ++position_;
            return true;
        
        case '+':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_ADD, "+", position_, compilation_unit_});
            ++position_;
            return true;
        case '-':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_SUBTRACT, "-", position_, compilation_unit_});
            ++position_;
            return true;
        case '*':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_MULTIPLY, "*", position_, compilation_unit_});
            ++position_;
            return true;
        case '/':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_DIVIDE, "/", position_, compilation_unit_});
            ++position_;
            return true;
        case '{':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_BEGIN_BLOCK, "{", position_, compilation_unit_});
            ++position_;
            return true;
        case '}':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_END_BLOCK, "}", position_, compilation_unit_});
            ++position_;
            return true;
        case '(':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_LEFT_PAREN, "(", position_, compilation_unit_});
            ++position_;
            return true;
        case ')':
            tokens_.push_back(mylang::Token{mylang::TypeIDs::SYMBOL, mylang::TokenIDs::SYM_RIGHT_PAREN, ")", position_, compilation_unit_});
            ++position_;
            return true;
        default:
            return false;
        }
    }

    bool accept_identifier()
    {
        const unsigned char first = static_cast<unsigned char>(source_[position_]);
        if (source_[position_] == '_' || std::isalpha(first))
        {
            const size_t start = position_;
            ++position_;
            while (position_ < source_.size())
            {
                const unsigned char ch = static_cast<unsigned char>(source_[position_]);
                if (source_[position_] != '_' && !std::isalnum(ch))
                {
                    break;
                }
                ++position_;
            }

            tokens_.push_back(mylang::Token{mylang::TypeIDs::IDENTIFIER, mylang::TokenIDs::TOK_UNKNOWN, source_.substr(start, position_ - start), start, compilation_unit_});
            return true;
        }
        return false;
    }

    bool accept_integer()
    {
        const unsigned char first = static_cast<unsigned char>(source_[position_]);
        if (std::isdigit(first))
        {
            const size_t start = position_;
            ++position_;
            while (position_ < source_.size())
            {
                const unsigned char ch = static_cast<unsigned char>(source_[position_]);
                if (!std::isdigit(ch))
                {
                    break;
                }
                ++position_;
            }
            tokens_.push_back(mylang::Token{mylang::TypeIDs::INTEGER, mylang::TokenIDs::TOK_UNKNOWN, source_.substr(start, position_ - start), start, compilation_unit_});
            return true;
        }
        return false;
    }

private:
    std::string source_;
    std::string compilation_unit_;
    size_t position_ = 0;
    std::vector<mylang::Token> tokens_;
};