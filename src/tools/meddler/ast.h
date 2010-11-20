#pragma once

#include <vector>
#include <string>

namespace AST
{

class ast_node_t
{
};

// Variable or parameter declaration as "type-name" pair.
class var_decl_t
{
public:
    std::string type; // use known types! check LLVM's Type/TypeBuilder
    std::string name;
};

// Represents both method arguments and returns.
class parameter_t : public var_decl_t
{
public:
    enum { in, out, inout } direction;
};

class alias_t : public var_decl_t
{
};
// subtree of alias defines concrete variants: set_alias, array_alias, record_alias

class exception_t
{
public:
    std::string name;
    std::vector<var_decl_t*> fields;
};

class method_t
{
public:
    bool idempotent; //..., async, oneway(==never_returns?)
    std::string name;
    std::vector<parameter_t*> params;
    std::vector<parameter_t*> returns;
    std::vector<exception_t*> raises;
    bool never_returns;
};

class interface_t
{
public:
    bool local;
    bool final;
    std::string name;
    std::string base;
    std::vector<interface_t*> imports;//implicitly derived from 'types' contents.
    std::vector<alias_t*>     types;
    std::vector<exception_t*> exceptions;
    std::vector<method_t*>    methods;
};

}
