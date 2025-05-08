#ifndef CDREAM_AST_H
#define CDREAM_AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <variant>

#include "../lexer/token.h"
#include "../lexer/lexer.h"

namespace cdream {

    class Expression;

    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        virtual void print(std::ostream& os, int indent = 0) const = 0;
    };

    class Statement : public ASTNode {
    public:
        virtual ~Statement() = default;
    };

    class Program : public ASTNode {
    public:
        explicit Program(std::vector<std::unique_ptr<Statement>> statements)
            : statements_(std::move(statements)) {}

        void print(std::ostream& os, int indent = 0) const override;
        const std::vector<std::unique_ptr<Statement>>& statements() const { return statements_; }

    private:
        std::vector<std::unique_ptr<Statement>> statements_;
    };

    class Expression : public ASTNode {
    public:
        virtual ~Expression() = default;
    };

    // Literal values
    class LiteralExpression : public Expression {
    public:
        using Value = std::variant<int, double, bool, std::string>;
        
        explicit LiteralExpression(Value value) : value_(std::move(value)) {}
        
        void print(std::ostream& os, int indent = 0) const override;
        const Value& value() const { return value_; }
        
    private:
        Value value_;
    };

    // Variable references
    class VariableExpression : public Expression {
    public:
        explicit VariableExpression(const std::string& name) : name_(name) {}
        
        void print(std::ostream& os, int indent = 0) const override;
        const std::string& name() const { return name_; }
        
    private:
        std::string name_;
    };

    class BinaryExpression : public Expression {
    public:
        enum class Operator {
            ADD, SUBTRACT, MULTIPLY, DIVIDE, POWER, MODULO
        };
        
        BinaryExpression(std::unique_ptr<Expression> left, Operator op, std::unique_ptr<Expression> right)
            : left_(std::move(left)), op_(op), right_(std::move(right)) {}
        
        void print(std::ostream& os, int indent = 0) const override;
        const Expression* left() const { return left_.get(); }
        const Expression* right() const { return right_.get(); }
        Operator op() const { return op_; }
        
    private:
        std::unique_ptr<Expression> left_;
        Operator op_;
        std::unique_ptr<Expression> right_;
    };

    // Variable declaration
    struct VarDeclaration {
        std::string name;
        TokenType dataType;
    };

    class VarStatement : public Statement {
    public:
        VarStatement(const std::string& name, TokenType varType, TokenType dataType, std::unique_ptr<Expression> initializer) 
            : varType_(varType) {
            declarations_.push_back({name, dataType});
            if (initializer) {
                initializers_.push_back(std::move(initializer));
            }
        }
        
        VarStatement(
            std::vector<VarDeclaration> declarations,
            TokenType varType,
            std::vector<std::unique_ptr<Expression>> initializers)
            : declarations_(std::move(declarations)), 
              varType_(varType),
              initializers_(std::move(initializers)) {}
        
        void print(std::ostream& os, int indent = 0) const override;
        const std::vector<VarDeclaration>& declarations() const { return declarations_; }
        TokenType varType() const { return varType_; }
        const std::vector<std::unique_ptr<Expression>>& initializers() const { return initializers_; }
        
        // Convenience methods for backward compatibility
        const std::string& name() const { return declarations_[0].name; }
        TokenType dataType() const { return declarations_[0].dataType; }
        const Expression* initializer() const { 
            return initializers_.empty() ? nullptr : initializers_[0].get(); 
        }
        bool isMultiVar() const { return declarations_.size() > 1 || initializers_.size() > 1; }
        
    private:
        std::vector<VarDeclaration> declarations_;
        TokenType varType_; // VAR or VAL
        std::vector<std::unique_ptr<Expression>> initializers_;
    };

    class ExpressionStatement : public Statement {
    public:
        explicit ExpressionStatement(std::unique_ptr<Expression> expression)
            : expression_(std::move(expression)) {}
        
        void print(std::ostream& os, int indent = 0) const override;
        const Expression* expression() const { return expression_.get(); }
        
    private:
        std::unique_ptr<Expression> expression_;
    };

}

#endif