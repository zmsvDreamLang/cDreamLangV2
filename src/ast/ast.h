#ifndef CDREAM_AST_H
#define CDREAM_AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "token.h"
#include "lexer.h"

namespace cdream {

    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        virtual void print(std::ostream& os, int indent = 0) const = 0;
    };

    class Program : public ASTNode {
    public:
        explicit Program(std::vector<std::unique_ptr<ASTNode>> statements)
            : statements_(std::move(statements)) {}

        void print(std::ostream& os, int indent = 0) const override;

    private:
        std::vector<std::unique_ptr<ASTNode>> statements_;
    };

    class Expression : public ASTNode {
    public:
        void print(std::ostream& os, int indent = 0) const override;
    };

}

#endif