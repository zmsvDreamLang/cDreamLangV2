#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include "lexer/lexer.h"

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    try {
        std::string filePath = "./DreamLang/src/calc.zv";
        std::cout << "Reading file: " << filePath << std::endl;
        
        std::string source = read_file(filePath);
        std::cout << "File content length: " << source.length() << " characters" << std::endl;
        
        // 基本的词法分析
        cdream::Lexer lexer(source);
        std::vector<cdream::Token> tokens = lexer.tokenize();
        
        // 输出分析结果
        std::cout << "Tokenization result:" << std::endl;
        std::cout << "====================" << std::endl;
        std::cout << "Total tokens: " << tokens.size() << std::endl;
        std::cout << "====================" << std::endl;
        
        for (const auto& token : tokens) {
            std::string typeStr;
            switch (token.type) {
                case cdream::TokenType::VAR: typeStr = "VAR"; break;
                case cdream::TokenType::VAL: typeStr = "VAL"; break;
                case cdream::TokenType::INT_TYPE: typeStr = "INT_TYPE"; break;
                case cdream::TokenType::FLOAT_TYPE: typeStr = "FLOAT_TYPE"; break;
                case cdream::TokenType::BOOL_TYPE: typeStr = "BOOL_TYPE"; break;
                case cdream::TokenType::INT_LITERAL: typeStr = "INT_LITERAL"; break;
                case cdream::TokenType::FLOAT_LITERAL: typeStr = "FLOAT_LITERAL"; break;
                case cdream::TokenType::LATEX_EXPRESSION: typeStr = "LATEX_EXPRESSION"; break;
                case cdream::TokenType::IDENTIFIER: typeStr = "IDENTIFIER"; break;
                case cdream::TokenType::LEFT_PAREN: typeStr = "LEFT_PAREN"; break;
                case cdream::TokenType::RIGHT_PAREN: typeStr = "RIGHT_PAREN"; break;
                case cdream::TokenType::LEFT_BRACE: typeStr = "LEFT_BRACE"; break;
                case cdream::TokenType::RIGHT_BRACE: typeStr = "RIGHT_BRACE"; break;
                case cdream::TokenType::COMMA: typeStr = "COMMA"; break;
                case cdream::TokenType::DOT: typeStr = "DOT"; break;
                case cdream::TokenType::MINUS: typeStr = "MINUS"; break;
                case cdream::TokenType::PLUS: typeStr = "PLUS"; break;
                case cdream::TokenType::SLASH: typeStr = "SLASH"; break;
                case cdream::TokenType::STAR: typeStr = "STAR"; break;
                case cdream::TokenType::POWER: typeStr = "POWER"; break;
                case cdream::TokenType::MODULO: typeStr = "MODULO"; break;
                case cdream::TokenType::COLON: typeStr = "COLON"; break;
                case cdream::TokenType::EQUAL: typeStr = "EQUAL"; break;
                case cdream::TokenType::EOF_TOKEN: typeStr = "EOF"; break;
                default: typeStr = "UNKNOWN"; break;
            }
            
            std::cout << "Line " << token.line << ", Col " << token.column 
                      << ": " << typeStr << " '" << token.lexeme << "'" << std::endl;
        }
        
        // 性能测试
        int iterations = 1000000;
        std::cout << "\nPerformance Test:" << std::endl;
        std::cout << "====================" << std::endl;
        std::cout << "Tokenize for " << iterations << " iterations." << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            cdream::Lexer lexer(source);
            std::vector<cdream::Token> tokens = lexer.tokenize();
        }
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> duration = end - start;
        double tokensPerSecond = (tokens.size() * iterations) / duration.count();
        double msPerIteration = (duration.count() * 1000.0) / iterations;
        
        std::cout << "Tokenization completed in " << duration.count() << " seconds." << std::endl;
        std::cout << "Average time per iteration: " << std::fixed << std::setprecision(6) 
                  << msPerIteration << " ms" << std::endl;
        std::cout << "Tokens processed per second: " << std::fixed << std::setprecision(2) 
                  << tokensPerSecond << std::endl;
        std::cout << "====================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}