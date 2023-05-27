// Example how to use Boost Spirit to construct an abstract syntax tree (AST)
// for a simple arithmetic grammar and to evaluate expressions _with_ variables!
//
// The grammar accepts expressions like "y = 1 + 2 * x", constructs an AST and
// evaluates it correctly. Non-assignment expression are also evaluated.

#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

#define BOOST_SPIRIT_DEBUG
#include <boost/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

/******************************************************************************/

// Utility to run a parser, check for errors, and capture the results.
template <typename Parser, typename Skipper, typename ... Args>
void PhraseParse(
    const std::string& input, const Parser& p, const Skipper& s,
    Args&& ... args)
{
    std::string::const_iterator begin = input.begin(), end = input.end();
    boost::spirit::qi::phrase_parse(
        begin, end, p, s, std::forward<Args>(args) ...);
    if (begin != end) {
        std::cout << "Unparseable: "
                  << std::quoted(std::string(begin, end)) << std::endl;
    }
}

/******************************************************************************/

class BinaryOpNode;
template<typename T> class ConstantNode;
class VariableNode;
class AssignmentNode;
class BodyNode;
class WhileNode;
class IfNode;
class CommandNode;

class IASTVisitor {
public:
    virtual void visit(const ConstantNode<std::string>& ref) = 0;
    virtual void visit(const ConstantNode<double>& ref) = 0;
    virtual void visit(const BinaryOpNode& ref) = 0;
    virtual void visit(const VariableNode& ref) = 0;
    virtual void visit(const AssignmentNode& ref) = 0;
    virtual void visit(const WhileNode& ref) = 0;
    virtual void visit(const BodyNode& ref) = 0;
    virtual void visit(const IfNode& ref) = 0;
    virtual void visit(const CommandNode& ref) = 0;

    virtual std::string getCompiledCode() const = 0;

    virtual ~IASTVisitor() = default;
};

class ASTNode
{
public:
    virtual void accept(IASTVisitor&) = 0;
    virtual ~ASTNode() { }
};

using ASTNodePtr = ASTNode*;

class BinaryOpNode : public ASTNode
{
public:
    BinaryOpNode(const ASTNodePtr& left, const ASTNodePtr& right, const std::string& op)
        : _left(left), _right(right), _op(op) { }

    ASTNodePtr left() const {
        return _left;
    }

    ASTNodePtr right() const {
        return _right;
    }

    const std::string& op() const {
        return _op;
    }

    void accept(IASTVisitor& v) override { v.visit(*this); }

    ~BinaryOpNode() {
        delete _left;
        delete _right;
    }

private:
    ASTNodePtr _left, _right; std::string _op;
};

template<typename T>
class ConstantNode : public ASTNode
{
public:
    ConstantNode(T value)
        : _value(value) { }

    T value() const {
        return _value;
    }

    void accept(IASTVisitor& v) override { v.visit(*this); }

private:
    T _value;
};

class VariableNode : public ASTNode
{
public:
    VariableNode(std::string identifier)
        : _identifier(identifier) { }

    std::string identifier() const { return _identifier; }

    void accept(IASTVisitor& v) { v.visit(*this); }

private:
    std::string _identifier;
};

class AssignmentNode : public ASTNode
{
public:
    AssignmentNode(const ASTNodePtr& var, const ASTNodePtr& value)
        : _var(var), _value(value) { }

    ASTNodePtr var() const {
        return _var;
    }

    ASTNodePtr value() const {
        return _value;
    }

    void accept(IASTVisitor& v) override { v.visit(*this); }

private:
    ASTNodePtr _var;
    ASTNodePtr _value;
};

class BodyNode : public ASTNode 
{
public:
    BodyNode(const std::vector<ASTNodePtr>& expressions) : _expressions(expressions) {}

    const std::vector<ASTNodePtr>& expressions() const {
        return _expressions;
    }

    void accept(IASTVisitor& v) override { v.visit(*this); }

private:
    std::vector<ASTNodePtr> _expressions;
};

class WhileNode : public ASTNode 
{
public:
    WhileNode(ASTNodePtr expr, ASTNodePtr body) : _expr(expr), _body(body) {}

    ASTNodePtr expr() const {
        return _expr;
    }
    ASTNodePtr body() const {
        return _body;
    }

    void accept(IASTVisitor& v) override { v.visit(*this); }

private:
    ASTNodePtr _expr;
    ASTNodePtr _body;
};

class IfNode : public ASTNode 
{
public:
    IfNode(ASTNodePtr expr, ASTNodePtr body) : _expr(expr), _body(body) {}

    ASTNodePtr expr() const {
        return _expr;
    }
    ASTNodePtr body() const {
        return _body;
    }

    void accept(IASTVisitor& v) override { v.visit(*this); }

private:
    ASTNodePtr _expr;
    ASTNodePtr _body;
};

class CommandNode : public ASTNode 
{
public:
    CommandNode(const std::string& command, ASTNodePtr expr) : _command(command), _expr(expr) {}

    void accept(IASTVisitor& v) override { v.visit(*this); }

    std::string command() const { return _command; }
    ASTNodePtr expr() const { return _expr; }

private:
    std::string _command;
    ASTNodePtr _expr;
};

/******************************************************************************/
class ASTVisitorImpl : public IASTVisitor
{
public:
    ASTVisitorImpl() {
        _stream << "from multiprocessing import Pool" << std::endl 
                << std::endl << std::string(40, '#') << std::endl << std::endl;
    }

    virtual void visit(const BinaryOpNode& ref) override {
        enum Ops { As, Is, Plus, Minus, Mul, Div, };
        static std::map<std::string, Ops> str2op = { { "as", As }, { "is", Is }, { "+", Plus }, { "-", Minus }, { "*", Mul }, { "/", Div } };
        
        switch (str2op[ref.op()]) {
            case As:
                ref.right()->accept(*this); _stream << "("; ref.left()->accept(*this); _stream << ")";
                return;
            case Is:
                _stream << "("; 
                ref.left()->accept(*this); 
                _stream << "=="; 
                ref.right()->accept(*this);
                _stream << ")"; 
                break;
            case Plus:
            case Minus:
            case Mul:
            case Div:
            default:
                _stream << "("; 
                ref.left()->accept(*this); 
                _stream << " " << ref.op() << " "; 
                ref.right()->accept(*this);
                _stream << ")";
        }
    }

    virtual void visit(const ConstantNode<double>& ref) override {
        _stream << ref.value();
    }

    virtual void visit(const ConstantNode<std::string>& ref) override {
        _stream << "'" <<  ref.value() << "'";
    }

    virtual void visit(const VariableNode& ref) override {
        _stream << ref.identifier();
    }

    virtual void visit(const AssignmentNode& ref) override {
        ref.var()->accept(*this);
        _stream << '=';
        ref.value()->accept(*this);
    }

    virtual void visit(const BodyNode& ref) override {
        ++_level;
        const size_t indents = (_level - 1) * 4;
        if (ref.expressions().empty()) {
            _stream << std::string(indents, ' ') << "pass";
        }
        for (const auto& n : ref.expressions()) {
            _stream << std::string(indents, ' ');
            n->accept(*this);
            _stream << std::endl;
        }
        --_level;
    }

    virtual void visit(const WhileNode& ref) override {
        _stream << "while ";
        ref.expr()->accept(*this); 
        _stream << ":" << std::endl;
        ref.body()->accept(*this);
    }

    virtual void visit(const IfNode& ref) override {
        _stream << "if ";
        ref.expr()->accept(*this);
        _stream << ":" << std::endl;
        ref.body()->accept(*this);
    }

    virtual void visit(const CommandNode& ref) override {
        if (ref.command() == "run!") {
            _stream << "make_new_thread(lambda:";
            ref.expr()->accept(*this);
            _stream << ")";
        }
        if (ref.command() == "wait!") {
            _stream << "wait_all_threads()";
        }
        if (ref.command() == "not!") {
            _stream << "not";
            ref.expr()->accept(*this);
        }
        if (ref.command() == "read!") {
            _stream << "input()";
        }
        if (ref.command() == "write!") {
            _stream << "print(";
            ref.expr()->accept(*this);
            _stream << ")";
        }
    }

    std::string getCompiledCode() const override {
        return _stream.str();
    }

private:
    int _level = 0;
    std::stringstream _stream;

};
/******************************************************************************/

template<typename Iterator>
struct SLATTGrammar
    : public qi::grammar<Iterator, ASTNodePtr(), qi::space_type>
{

    SLATTGrammar() : SLATTGrammar::base_type(slatt, "slatt")
    {
        using boost::spirit::lexeme;
        using boost::spirit::lit;
        using boost::spirit::as_string;
        using qi::on_error;
        using qi::fail;
        using phx::val;
        using phx::construct;

        quoted_string = qi::omit[qi::char_("'\"") [qi::_a = qi::_1]] >> qi::no_skip[*(qi::char_ - qi::char_(qi::_a))] >> lit(qi::_a);
        // BOOST_SPIRIT_DEBUG_NODE(quoted_string);

        constant = 
            qi::double_[qi::_val = phx::new_<ConstantNode<double>>(qi::_1)] |
            quoted_string[qi::_val = phx::new_<ConstantNode<std::string>>(qi::_1)];
        // BOOST_SPIRIT_DEBUG_NODE(constant);

        variable = as_string[lexeme[qi::alpha >> *(qi::alnum - qi::char_('!'))]][qi::_val = phx::new_<VariableNode>(qi::_1)];

        slatt %= body;

        assignment = (variable >> '=' > expr)
            [qi::_val = phx::new_<AssignmentNode>(qi::_1, qi::_2)];

        type %= qi::lit("int") | qi::lit("char");
        binary_op_string = (
            qi::string("as") | 
            qi::string("is") | 
            qi::string("+")  | 
            qi::string("-")  | 
            qi::string("*")  | 
            qi::string("/"));

        expr = (factor >> binary_op_string >> expr) [qi::_val = phx::new_<BinaryOpNode>(qi::_1, qi::_3, qi::_2) ] |
            factor [qi::_val = qi::_1];
            
        factor %= 
            command_result |
            constant |
            variable |
            group;
            
        group %= '(' >> expr > ')';

        body = ('{' >> *(assignment | while_loop | if_statement | command_no_result | expr) > '}')[ qi::_val = phx::new_<BodyNode>(qi::_1) ];
        if_statement = (lit("if") > '(' > expr > ')' > body)[
            qi::_val = phx::new_<IfNode>(qi::_1, qi::_2)
        ];
        while_loop = (lit("while") > '(' >> expr > ')' > body)[
            qi::_val = phx::new_<WhileNode>(qi::_1, qi::_2)
        ];

        command_result =
            qi::string("read!")[qi::_val = phx::new_<CommandNode>(qi::_1, val(nullptr))] |
            (qi::string("not!") > expr)[qi::_val = phx::new_<CommandNode>(qi::_1, qi::_2)];

        //BOOST_SPIRIT_DEBUG_NODE(command_result);
        
        command_no_result = 
            (qi::string("write!") > expr)[qi::_val = phx::new_<CommandNode>(qi::_1, qi::_2)] |
            (qi::string("run!")   > expr)[qi::_val = phx::new_<CommandNode>(qi::_1, qi::_2)] |
             qi::string("wait!")[qi::_val = phx::new_<CommandNode>(qi::_1, val(nullptr))];

        type.name("type");
        quoted_string.name("quoted_string");
        variable.name("variable");
        body.name("body");
        assignment.name("assignment");
        slatt.name("SLATT");
        group.name("group");
        while_loop.name("for_loop");

        on_error<fail>
        (
            slatt
          , std::cout
                << val("Error! Expecting ")
                << qi::_4                               // what failed?
                << val(" here: \"")
                << construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
                << val("\"")
                << std::endl
        );
    }

    qi::rule<Iterator, std::string(), qi::space_type, qi::locals<char>> quoted_string;
    qi::rule<Iterator, std::string(), qi::space_type> binary_op_string, type, command_with_arg_name, command_no_arg_name;
    qi::rule<Iterator, ASTNodePtr(), qi::space_type> 
        slatt, 
        variable,
        body,
        constant,
        assignment, 
        group, 
        while_loop, 
        expr, 
        binary_op, 
        product, 
        factor, 
        if_statement, 
        command_result, command_no_result;
};

/******************************************************************************/

int main()
{
    std::string code = 
        "{                                        \n"
        "    stopCommand = read!                  \n"
        "    while (not! (stopCommand is \"q\")) {\n"
        "        write! \"enter a command:\"      \n"
        "        a = read! as int                 \n"
        "        op = read!                       \n"
        "        b = read! as int                 \n"
        "        run! b + c                       \n"
        "        stopCommand = read!              \n"
        "    }                                    \n"
        "    wait!                                \n"
        "}"
        ;
    // code = "a = 10 + 1";
    std::cout << "code: " << std::endl << code << std::endl;
    
    using qi::space;
    std::string::const_iterator iter = code.begin();
    std::string::const_iterator end = code.end();

    SLATTGrammar<std::string::const_iterator> grammar;
    ASTNodePtr ast;
    bool r = phrase_parse(iter, end, grammar, space, ast);

    if (r && iter == end)
    {
        ASTVisitorImpl visitor;
        ast->accept(visitor);
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
        std::cout << "compiled: " << std::endl;
        std::cout << visitor.getCompiledCode() << std::endl;
        return 0;
    }
    else
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "-------------------------\n";
        return 1;
    }
    return 0;
}

/******************************************************************************/