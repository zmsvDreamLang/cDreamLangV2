#ifndef CDREAM_LEXER_H
#define CDREAM_LEXER_H

#include <string>
#include <vector>
#include "token.h"

namespace cdream {

    class Lexer {
        public:
            Lexer(const std::string& source);
            std::vector<Token> tokenize();

        private:
            Token next_token();
            Token skip_whitespace();
            Token scan_comment();
            Token scan_documentation();
            Token scan_number();
            Token scan_identifier();
            Token scan_latex_expression();
            
            char advance();
            bool match(char expected);
            char peek(int offset = 0) const;
            bool is_at_end() const;
            bool is_keyword(const std::string& identifier);
            Token create_token(TokenType type);

            std::string source_;
            size_t current_position_;
            size_t start_position_;
            int current_line_;
            int current_column_;
    };

}

#endif