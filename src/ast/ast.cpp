#include "ast.h"
#include <iostream>
#include <string>

namespace cdream {

    static void indent(std::ostream& os, int level) {
        for (int i = 0; i < level; ++i) {
            os << "  ";
        }
    }

    void Program::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "Program:" << std::endl;
        
        for (const auto& statement : statements_) {
            statement->print(os, indent_level + 1);
        }
    }

    void LiteralExpression::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "Literal: ";
        
        if (std::holds_alternative<int>(value_)) {
            os << "INT(" << std::get<int>(value_) << ")";
        } else if (std::holds_alternative<double>(value_)) {
            os << "FLOAT(" << std::get<double>(value_) << ")";
        } else if (std::holds_alternative<bool>(value_)) {
            os << "BOOL(" << (std::get<bool>(value_) ? "true" : "false") << ")";
        } else if (std::holds_alternative<std::string>(value_)) {
            os << "STRING(\"" << std::get<std::string>(value_) << "\")";
        }
        
        os << std::endl;
    }

    void VariableExpression::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "Variable: " << name_ << std::endl;
    }

    void BinaryExpression::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "BinaryExpression: ";
        
        switch (op_) {
            case Operator::ADD: os << "+"; break;
            case Operator::SUBTRACT: os << "-"; break;
            case Operator::MULTIPLY: os << "*"; break;
            case Operator::DIVIDE: os << "/"; break;
            case Operator::POWER: os << "^"; break;
            case Operator::MODULO: os << "%"; break;
        }
        
        os << std::endl;
        
        left_->print(os, indent_level + 1);
        right_->print(os, indent_level + 1);
    }

    void VarStatement::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "VarStatement: " 
           << (varType_ == TokenType::VAR ? "var" : "val") << std::endl;
        
        // Print variable declarations
        indent(os, indent_level + 1);
        os << "Declarations:" << std::endl;
        for (size_t i = 0; i < declarations_.size(); ++i) {
            indent(os, indent_level + 2);
            os << declarations_[i].name;
            
            // Print type annotation if available
            switch (declarations_[i].dataType) {
                case TokenType::INT_TYPE: os << ": int"; break;
                case TokenType::FLOAT_TYPE: os << ": float"; break;
                case TokenType::BOOL_TYPE: os << ": bool"; break;
                case TokenType::STRING_TYPE: os << ": string"; break;
                default: break; // No type annotation
            }
            os << std::endl;
        }
        
        // Print initializers if available
        if (!initializers_.empty()) {
            indent(os, indent_level + 1);
            os << "Initializers:" << std::endl;
            for (const auto& initializer : initializers_) {
                initializer->print(os, indent_level + 2);
            }
        }
    }

    void ExpressionStatement::print(std::ostream& os, int indent_level) const {
        indent(os, indent_level);
        os << "ExpressionStatement:" << std::endl;
        expression_->print(os, indent_level + 1);
    }

}
