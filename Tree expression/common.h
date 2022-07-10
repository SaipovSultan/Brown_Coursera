#pragma once

#include <memory>
#include <string>

class Expression {
public:
    virtual ~Expression() = default;

    virtual int Evaluate() const = 0;

    virtual std::string ToString() const = 0;
};

using ExpressionPtr = std::unique_ptr<Expression>;

class ValueExp : public Expression {
public:
    ValueExp(int value) : value(value) {}

    int Evaluate() const override{
        return value;
    }
    std::string ToString() const override{
        return std::to_string(value);
    }
private:
    int value;
};

class BinaryExp : public Expression {
public:
    BinaryExp(ExpressionPtr left, ExpressionPtr right) : left(std::move(left)), right(std::move(right)) {}

    int Evaluate() const {
        return EvaluateOnValues(left->Evaluate(), right->Evaluate());
    }

    std::string ToString() const{
        return '(' + left->ToString() + ')' + GetSymbol() + '(' + right->ToString() + ')';
    }
private:
    virtual int EvaluateOnValues(int left, int right) const = 0;

    virtual char GetSymbol() const = 0;

    ExpressionPtr left, right;
};

class ProductExp : public BinaryExp {
public:
    ProductExp(ExpressionPtr left, ExpressionPtr right) : BinaryExp(std::move(left), std::move(right)) {}

    int EvaluateOnValues(int left, int right) const override {
        return left * right;
    }

    char GetSymbol() const {
        return '*';
    }
};

class SumExp : public BinaryExp {
public:
    SumExp(ExpressionPtr left, ExpressionPtr right) : BinaryExp(std::move(left), std::move(right)) {}

    int EvaluateOnValues(int left, int right) const override {
        return left + right;
    }

    char GetSymbol() const {
        return '+';
    }
};

ExpressionPtr Value(int value) {
    return std::make_unique<ValueExp>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<SumExp>(std::move(left), std::move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<ProductExp>(std::move(left), std::move(right));
}