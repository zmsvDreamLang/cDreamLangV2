#include "parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace cdream {

    Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), current_(0) {}

    std::unique_ptr<Program> Parser::parse() {
        std::vector<std::unique_ptr<Statement>> statements;
        
        try {
            while (!is_at_end()) {
                statements.push_back(parse_statement());
            }
        } catch (const ParseError& error) {
            std::cerr << "Parse error: " << error.what() << std::endl;
            synchronize();
        }
        
        return std::make_unique<Program>(std::move(statements));
    }

    std::unique_ptr<Statement> Parser::parse_statement() {
        if (match(TokenType::VAR) || match(TokenType::VAL)) {
            return parse_var_statement();
        }
        
        return parse_expression_statement();
    }

    std::unique_ptr<Statement> Parser::parse_var_statement() {
        TokenType var_type = previous().type;
        
        std::vector<VarDeclaration> declarations;
        
        Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
        
        TokenType data_type = TokenType::EOF_TOKEN;
        if (match(TokenType::COLON)) {
            if (match(TokenType::INT_TYPE)) {
                data_type = TokenType::INT_TYPE;
            } else if (match(TokenType::FLOAT_TYPE)) {
                data_type = TokenType::FLOAT_TYPE;
            } else if (match(TokenType::BOOL_TYPE)) {
                data_type = TokenType::BOOL_TYPE;
            } else if (match(TokenType::STRING_TYPE)) {
                data_type = TokenType::STRING_TYPE;
            } else {
                throw error(peek(), "Expected type after colon");
            }
        }
        
        declarations.push_back({name.lexeme, data_type});
        
        while (peek().type == TokenType::COMMA) {
            advance();
            
            Token var_name = consume(TokenType::IDENTIFIER, "Expected variable name after comma");
            
            TokenType var_data_type = TokenType::EOF_TOKEN;
            if (match(TokenType::COLON)) {
                if (match(TokenType::INT_TYPE)) {
                    var_data_type = TokenType::INT_TYPE;
                } else if (match(TokenType::FLOAT_TYPE)) {
                    var_data_type = TokenType::FLOAT_TYPE;
                } else if (match(TokenType::BOOL_TYPE)) {
                    var_data_type = TokenType::BOOL_TYPE;
                } else if (match(TokenType::STRING_TYPE)) {
                    var_data_type = TokenType::STRING_TYPE;
                } else {
                    throw error(peek(), "Expected type after colon");
                }
            }
            
            declarations.push_back({var_name.lexeme, var_data_type});
        }
        
        std::vector<std::unique_ptr<Expression>> initializers;
        if (match(TokenType::EQUAL)) {
            initializers.push_back(parse_expression());
            while (peek().type == TokenType::COMMA) {
                advance();
                
                auto expr = parse_expression();
                if (initializers.size() < declarations.size()) {
                    initializers.push_back(std::move(expr));
                }
            }
        }
        
        return std::make_unique<VarStatement>(std::move(declarations), var_type, std::move(initializers));
    }

    std::unique_ptr<ExpressionStatement> Parser::parse_expression_statement() {
        auto expr = parse_expression();
        return std::make_unique<ExpressionStatement>(std::move(expr));
    }

    std::unique_ptr<Expression> Parser::parse_expression() {
        auto left = parse_term();
        
        while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
            TokenType op_type = previous().type;
            auto right = parse_term();
            
            BinaryExpression::Operator op = (op_type == TokenType::PLUS) ? 
                BinaryExpression::Operator::ADD : BinaryExpression::Operator::SUBTRACT;
            
            left = std::make_unique<BinaryExpression>(std::move(left), op, std::move(right));
        }
        
        return left;
    }

    std::unique_ptr<Expression> Parser::parse_term() {
        auto left = parse_factor();
        
        while (match(TokenType::STAR) || match(TokenType::SLASH) || match(TokenType::MODULO)) {
            TokenType op_type = previous().type;
            auto right = parse_factor();
            
            BinaryExpression::Operator op;
            if (op_type == TokenType::STAR) {
                op = BinaryExpression::Operator::MULTIPLY;
            } else if (op_type == TokenType::SLASH) {
                op = BinaryExpression::Operator::DIVIDE;
            } else {
                op = BinaryExpression::Operator::MODULO;
            }
            
            left = std::make_unique<BinaryExpression>(std::move(left), op, std::move(right));
        }
        
        return left;
    }

    std::unique_ptr<Expression> Parser::parse_factor() {
        auto left = parse_primary();
        
        while (match(TokenType::POWER)) {
            auto right = parse_primary();
            left = std::make_unique<BinaryExpression>(
                std::move(left), BinaryExpression::Operator::POWER, std::move(right));
        }
        
        return left;
    }

    std::unique_ptr<Expression> Parser::parse_primary() {
        if (match(TokenType::INT_LITERAL)) {
            std::string literal = previous().lexeme;
            int value;
            
            if (literal.size() > 2 && literal[0] == '0') {
                if (literal[1] == 'x' || literal[1] == 'X') {
                    value = std::stoi(literal.substr(2), nullptr, 16);
                } else if (literal[1] == 'b' || literal[1] == 'B') {
                    value = std::stoi(literal.substr(2), nullptr, 2);
                } else if (literal[1] == 'o' || literal[1] == 'O') {
                    value = std::stoi(literal.substr(2), nullptr, 8);
                } else {
                    value = std::stoi(literal);
                }
            } else {
                value = std::stoi(literal);
            }
            
            return std::make_unique<LiteralExpression>(value);
        }
        
        if (match(TokenType::FLOAT_LITERAL)) {
            double value = std::stod(previous().lexeme);
            return std::make_unique<LiteralExpression>(value);
        }
        
        if (match(TokenType::IDENTIFIER)) {
            return std::make_unique<VariableExpression>(previous().lexeme);
        }
        
        if (match(TokenType::LEFT_PAREN)) {
            auto expr = parse_expression();
            consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
            return expr;
        }
        
        throw error(peek(), "Expected expression");
    }

    bool Parser::match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    bool Parser::check(TokenType type) const {
        if (is_at_end()) {
            return false;
        }
        return peek().type == type;
    }

    Token Parser::advance() {
        if (!is_at_end()) {
            current_++;
        }
        return previous();
    }

    Token Parser::peek() const {
        return tokens_[current_];
    }

    Token Parser::previous() const {
        return tokens_[current_ - 1];
    }

    bool Parser::is_at_end() const {
        return peek().type == TokenType::EOF_TOKEN;
    }

    Token Parser::consume(TokenType type, const std::string& error_message) {
        if (check(type)) {
            return advance();
        }
        
        throw error(peek(), error_message);
    }

    ParseError Parser::error(const Token& token, const std::string& message) const {
        return ParseError(message, token.line, token.column);
    }

    void Parser::synchronize() {
        advance();
        
        while (!is_at_end()) {
            if (previous().type == TokenType::EOF_TOKEN) {
                return;
            }
            
            if (peek().type == TokenType::VAR || 
                peek().type == TokenType::VAL) {
                return;
            }
            
            advance();
        }
    }
}