#ifndef CDREAM_TOKEN_H
#define CDREAM_TOKEN_H

#include <string>

namespace cdream {

    enum class TokenType {
        // Keywords
        VAR,
        VAL,
        
        // Types
        NULL_TYPE,
        INT_TYPE,
        FLOAT_TYPE,
        BOOL_TYPE,
        STRING_TYPE,    
        
        INT_LITERAL,
        FLOAT_LITERAL,
        LATEX_EXPRESSION,
        
        IDENTIFIER,
        
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SLASH,
        STAR,
        POWER,
        MODULO,
        COLON,
        EQUAL,
        
        WHITESPACE,
        COMMENT,
        DOCUMENTATION,
        EOF_TOKEN
    };

    class Token {
        public:
            Token(TokenType type, const std::string& lexeme, int line, int column)
                : type(type), lexeme(lexeme), line(line), column(column) {}
            
            TokenType type;
            std::string lexeme;
            int line;
            int column;
            
            std::string toString() const {
                std::string typeStr;
                switch (type) {
                    case TokenType::VAR: typeStr = "VAR"; break;
                    case TokenType::VAL: typeStr = "VAL"; break;
                    case TokenType::NULL_TYPE: typeStr = "NULL_TYPE"; break;
                    case TokenType::INT_TYPE: typeStr = "INT_TYPE"; break;
                    case TokenType::FLOAT_TYPE: typeStr = "FLOAT_TYPE"; break;
                    case TokenType::BOOL_TYPE: typeStr = "BOOL_TYPE"; break;
                    case TokenType::INT_LITERAL: typeStr = "INT_LITERAL"; break;
                    case TokenType::FLOAT_LITERAL: typeStr = "FLOAT_LITERAL"; break;
                    case TokenType::STRING_TYPE: typeStr = "STRING_TYPE"; break;
                    case TokenType::LATEX_EXPRESSION: typeStr = "LATEX_EXPRESSION"; break;
                    case TokenType::IDENTIFIER: typeStr = "IDENTIFIER"; break;
                    case TokenType::LEFT_PAREN: typeStr = "LEFT_PAREN"; break;
                    case TokenType::RIGHT_PAREN: typeStr = "RIGHT_PAREN"; break;
                    case TokenType::LEFT_BRACE: typeStr = "LEFT_BRACE"; break;
                    case TokenType::RIGHT_BRACE: typeStr = "RIGHT_BRACE"; break;
                    case TokenType::COMMA: typeStr = "COMMA"; break;
                    case TokenType::DOT: typeStr = "DOT"; break;
                    case TokenType::MINUS: typeStr = "MINUS"; break;
                    case TokenType::PLUS: typeStr = "PLUS"; break;
                    case TokenType::SLASH: typeStr = "SLASH"; break;
                    case TokenType::STAR: typeStr = "STAR"; break;
                    case TokenType::POWER: typeStr = "POWER"; break;
                    case TokenType::MODULO: typeStr = "MODULO"; break;
                    case TokenType::COLON: typeStr = "COLON"; break;
                    case TokenType::EQUAL: typeStr = "EQUAL"; break;
                    case TokenType::WHITESPACE: typeStr = "WHITESPACE"; break;
                    case TokenType::COMMENT: typeStr = "COMMENT"; break;
                    case TokenType::DOCUMENTATION: typeStr = "DOCUMENTATION"; break;
                    case TokenType::EOF_TOKEN: typeStr = "EOF"; break;
                    default: typeStr = "UNKNOWN"; break;
                }
                
                return typeStr + " '" + lexeme + "' at line " + std::to_string(line) + 
                    ", column " + std::to_string(column);
            }
    };

}

#endif