#ifndef CDREAM_PARSER_H
#define CDREAM_PARSER_H

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

#include "../lexer/token.h"
#include "../lexer/lexer.h"
#include "../ast/ast.h"

namespace cdream {

    class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& message, int line, int column)
            : std::runtime_error(message + " at line " + std::to_string(line) + 
                                ", column " + std::to_string(column)),
              line_(line), column_(column) {}
        
        int line() const { return line_; }
        int column() const { return column_; }
        
    private:
        int line_;
        int column_;
    };

    class Parser {
    public:
        Parser(const std::vector<Token>& tokens);
        
        std::unique_ptr<Program> parse();
        
    private:
        std::unique_ptr<Statement> parse_statement();
        std::unique_ptr<Statement> parse_var_statement();
        std::unique_ptr<ExpressionStatement> parse_expression_statement();
        
        std::unique_ptr<Expression> parse_expression();
        std::unique_ptr<Expression> parse_term();
        std::unique_ptr<Expression> parse_factor();
        std::unique_ptr<Expression> parse_primary();
        
        bool match(TokenType type);
        bool check(TokenType type) const;
        Token advance();
        Token peek() const;
        Token previous() const;
        bool is_at_end() const;
        Token consume(TokenType type, const std::string& error_message);
        ParseError error(const Token& token, const std::string& message) const;
        void synchronize();
        
        const std::vector<Token>& tokens_;
        size_t current_;
    };
}

#endif