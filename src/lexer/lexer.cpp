#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

namespace cdream {

    Lexer::Lexer(const std::string& source) 
        : source_(source), current_position_(0), start_position_(0), 
        current_line_(1), current_column_(1) {}

    std::vector<Token> Lexer::tokenize() {
        std::vector<Token> tokens;
        
        while (!is_at_end()) {
            start_position_ = current_position_;
            
            Token token = next_token();
            if (token.type != TokenType::WHITESPACE && 
                token.type != TokenType::COMMENT && 
                token.type != TokenType::DOCUMENTATION) {
                tokens.push_back(token);
            }
        }
        
        tokens.push_back(Token(TokenType::EOF_TOKEN, "", current_line_, current_column_));
        
        return tokens;
    }

    Token Lexer::next_token() {
        char c = advance();
        
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                return skip_whitespace();
            case '\n':
                current_line_++;
                current_column_ = 1;
                return Token(TokenType::WHITESPACE, "\n", current_line_ - 1, current_column_);
                
            case '(': return create_token(TokenType::LEFT_PAREN);
            case ')': return create_token(TokenType::RIGHT_PAREN);
            case '{': return create_token(TokenType::LEFT_BRACE);
            case '}': return create_token(TokenType::RIGHT_BRACE);
            case ',': return create_token(TokenType::COMMA);
            case '.': return create_token(TokenType::DOT);
            case '-': return create_token(TokenType::MINUS);
            case '+': return create_token(TokenType::PLUS);
            case '*': return create_token(TokenType::STAR);
            case '^': return create_token(TokenType::POWER);
            case '%': return create_token(TokenType::MODULO);
            
            case ':': return create_token(TokenType::COLON);
            
            case '/':
                if (match('/')) {
                    if (match('/')) {
                        return scan_documentation();
                    } else {
                        return scan_comment();
                    }
                } else {
                    return create_token(TokenType::SLASH);
                }
                
            case '=': return create_token(TokenType::EQUAL);
        
                
            default:
                if (is_digit(c)) {
                    current_position_--;
                    current_column_--;
                    return scan_number();
                } else if (isalpha(c) || c == '_') {
                    return scan_identifier();
                } else {
                    throw std::runtime_error("Unexpected character: " + std::string(1, c));
                }
        }
    }

    Token Lexer::skip_whitespace() {
        while (isspace(peek()) && peek() != '\n' && !is_at_end()) {
            advance();
        }
        
        return Token(TokenType::WHITESPACE, 
                    source_.substr(start_position_, current_position_ - start_position_),
                    current_line_, current_column_ - (current_position_ - start_position_));
    }

    Token Lexer::scan_comment() {
        while (peek() != '\n' && !is_at_end()) {
            advance();
        }
        
        return Token(TokenType::COMMENT, 
                    source_.substr(start_position_, current_position_ - start_position_),
                    current_line_, current_column_ - (current_position_ - start_position_));
    }

    Token Lexer::scan_documentation() {
        std::string documentation = "///";
        
        while (!is_at_end()) {
            while (peek() != '\n' && !is_at_end()) {
                documentation += advance();
            }
            
            if (peek() == '\n') {
                documentation += advance();
                current_line_++;
                current_column_ = 1;
                
                if (peek() == '/' && peek(1) == '/' && peek(2) == '/') {
                    advance();
                    advance();
                    advance();
                    documentation += "///";
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        
        return Token(TokenType::DOCUMENTATION, documentation, 
                    current_line_, current_column_ - (current_position_ - start_position_));
    }

    bool Lexer::is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool Lexer::is_hex_digit(char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    bool Lexer::is_octal_digit(char c) {
        return c >= '0' && c <= '7';
    }
    bool Lexer::is_binary_digit(char c) {
        return c == '0' || c == '1';
    }

    Token Lexer::scan_number() {
        start_position_ = current_position_;
        char first = advance();
        
        if (first == '0') {
            if (peek() == 'x' || peek() == 'X') {
                advance();
                if (!is_hex_digit(peek())) {
                    throw std::runtime_error("Invalid hexadecimal number: expected at least one hex digit after '0x'");
                }
                while (is_hex_digit(peek())) {
                    advance();
                }
            } else if (peek() == 'b' || peek() == 'B') {
                advance();
                if (!is_binary_digit(peek())) {
                    throw std::runtime_error("Invalid binary number: expected binary digit after '0b'");
                }
                while (is_binary_digit(peek())) {
                    advance();
                }
            } else if (peek() == 'o' || peek() == 'O') {
                advance();
                if (!is_octal_digit(peek())) {
                    throw std::runtime_error("Invalid octal number: expected octal digit after '0o'");
                }
                while (is_octal_digit(peek())) {
                    advance();
                }
            } else {
                while (is_digit(peek())) {
                    advance();
                }
                
                if (peek() == '.' && is_digit(peek(1))) {
                    advance();

                    while (is_digit(peek())) {
                        advance();
                    }
                    
                    return Token(TokenType::FLOAT_LITERAL, 
                                source_.substr(start_position_, current_position_ - start_position_),
                                current_line_, current_column_ - (current_position_ - start_position_));
                }
            }
        } else {
            while (is_digit(peek())) {
                advance();
            }
            if (peek() == '.' && is_digit(peek(1))) {
                advance();

                while (is_digit(peek())) {
                    advance();
                }
                
                return Token(TokenType::FLOAT_LITERAL, 
                            source_.substr(start_position_, current_position_ - start_position_),
                            current_line_, current_column_ - (current_position_ - start_position_));
            }
        }

        return Token(TokenType::INT_LITERAL,
                    source_.substr(start_position_, current_position_ - start_position_),
                    current_line_, current_column_ - (current_position_ - start_position_));
    }

    bool Lexer::is_keyword(const std::string& text) {
        return text == "var" || text == "val" || text == "int" || 
               text == "float" || text == "bool";
    }

    Token Lexer::scan_identifier() {
        while (isalnum(peek()) || peek() == '_') {
            advance();
        }
        
        std::string text = source_.substr(start_position_, current_position_ - start_position_);
        
        TokenType type = TokenType::IDENTIFIER;
        if (is_keyword(text)) {
            if (text == "var") {
                type = TokenType::VAR;
            } else if (text == "val") {
                type = TokenType::VAL;
            } else if (text == "int") {
                type = TokenType::INT_TYPE;
            } else if (text == "float") {
                type = TokenType::FLOAT_TYPE;
            } else if (text == "bool") {
                type = TokenType::BOOL_TYPE;
            }
        }
        
        return Token(type, text, 
                    current_line_, current_column_ - (current_position_ - start_position_));
    }

    char Lexer::advance() {
        current_position_++;
        current_column_++;
        return source_[current_position_ - 1];
    }

    bool Lexer::match(char expected) {
        if (is_at_end() || source_[current_position_] != expected) {
            return false;
        }
        
        current_position_++;
        current_column_++;
        return true;
    }

    char Lexer::peek(int offset) const {
        if (current_position_ + offset >= source_.length()) {
            return '\0';
        }
        return source_[current_position_ + offset];
    }

    bool Lexer::is_at_end() const {
        return current_position_ >= source_.length();
    }

    Token Lexer::create_token(TokenType type) {
        return Token(type, 
                    source_.substr(start_position_, current_position_ - start_position_),
                    current_line_, current_column_ - 1);
    }

}