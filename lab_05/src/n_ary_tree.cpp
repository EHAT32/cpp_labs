#include "boost/phoenix/function/adapt_function.hpp"
#include "boost/phoenix/stl/algorithm/transformation.hpp"
#include "boost/spirit/home/support/common_terminals.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <variant>

#include <boost/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>
#include <boost/phoenix/stl.hpp>


using namespace std;

namespace fusion = boost::fusion;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

class ExprAST{
    public:
        virtual ~ExprAST(){}
        virtual std::string gen() = 0;
};

class ModuleExprAST : public ExprAST{
    public:
        std::vector<std::unique_ptr<ExprAST>> module;
        ModuleExprAST(std::vector<std::unique_ptr<ExprAST>> module) : module(std::move(module)){}
        std::string gen() override{
            throw std::runtime_error("Not Implemented");
        }

};

class NumberExprAST : public ExprAST{
    public:
        double val;
        NumberExprAST(double val) : val(val){}
        std::string gen() override{
            throw std::runtime_error("Not Implemented");
        }
};

class VariableExprAST : public ExprAST{
    public:
        std::string name;
        VariableExprAST(const std::string& name) : 
            name(name){}

        std::string gen() override{
            throw std::runtime_error("Not Implemented");
        }
};

class BinaryExprAST : public ExprAST{
    public:
        char op;
        std::unique_ptr<ExprAST> lhs, rhs;
        BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) : 
            op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
        std::string gen() override{
            throw std::runtime_error("Not Implemented");
        }
};

//call function

// class CallExprAST : public ExprAST{
//     public:
//         std::string Callee;
//         std::vector<std::unique_ptr<ExprAST>> Args;
//         CallExprAST(const std::string& callee, std::vector<std::unique_ptr<ExprAST>>& args) : Callee(callee), Args(args) {}
//         std::string gen() override{
//             throw std::runtime_error("Not Implemented");
//         }
// };

//function prototype and function itself

class ForPrototypeExprAST : public ExprAST {
    public:
        std::string from;
        std::string to;
        std::string step;
        ForPrototypeExprAST(const std::string& from, const std::string& to, const std::string& step) : 
            from(from), to(to), step(step) {}
        std::string gen() override{
            throw std::runtime_error("Not Implemented");
        }
};

class BodyExprAST : public ExprAST{
    public:
        std::vector<std::unique_ptr<ExprAST>> expressions;
        BodyExprAST() = default;
        std::string gen() override{
            throw std::runtime_error("Not Implemented");
        }
};

class ForLoopExprAST : public ExprAST {
    public:
        std::unique_ptr<ExprAST> prototype;
        std::unique_ptr<ExprAST> body;
    
    ForLoopExprAST(std::unique_ptr<ForPrototypeExprAST> proto, std::unique_ptr<BodyExprAST> body) : prototype(std::move(prototype)), body(std::move(body)) {}

    std::string gen() override {
        throw std::runtime_error("Not Implemented");
    }
};

namespace {

#define ADD_STD_SMART_PTR_WRAPPER(name) \
template <typename T> \
struct make_##name##_f \
{ \
    template <typename... A> struct result  \
        { typedef std::name##_ptr<T> type; }; \
    template <typename... A> \
    typename result<A...>::type operator()(A&&... a) const { \
        return std::make_##name<T>(std::forward<A>(a)...); \
    } \
}; \
template <typename T> \
using make_##name##_ = boost::phoenix::function<make_##name##_f<T>>;

ADD_STD_SMART_PTR_WRAPPER(shared)
ADD_STD_SMART_PTR_WRAPPER(unique)

template<typename T>
struct move_f
{
    typedef T result_type;
    
    template <typename A>
    result_type operator()(A arg1) const {
        return std::move(arg1);
    }
};

template<typename T>
using move_ = boost::phoenix::function<move_f<T>>;

}

template<typename Iterator>
struct SLATT_grammar : qi::grammar<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type>{
    SLATT_grammar() : SLATT_grammar::base_type(slatt){
        using qi::lit;
        using qi::lexeme;
        using ascii::char_;
        using ascii::string;
        using namespace qi::labels;
        using boost::spirit::qi::as_string;
        using boost::spirit::double_;
        using boost::spirit::qi::_1;
        using boost::spirit::qi::_val;
        using phx::at_c;
        using phx::push_back;
        

        // number = double_[_val = std::make_unique<NumberExprAST>(_1)];
        variable = as_string[lexeme[+char_]][_val = ::make_unique_<VariableExprAST>()(_1)];
        for_loop %= for_prototype >> body;
        binary %= variable >> (lit('=') | '+' | '-' | '*' | '/') >> (variable);
        body %= '{' >> *(variable | binary | for_loop | body) >> '}';
        slatt = body[_val = ::move_<std::unique_ptr<ExprAST>>()(_1)];
    }

    qi::rule<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type> slatt;
    // qi::rule<Iterator, std::unique_ptr<NumberExprAST>(), ascii::space_type> number;
    qi::rule<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type> variable;
    qi::rule<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type> binary;
    qi::rule<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type> for_prototype;
    qi::rule<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type> body;
    qi::rule<Iterator, std::unique_ptr<ExprAST>(), ascii::space_type> for_loop;

};


// struct Node;


// using expr_node = boost::variant<boost::recursive_wrapper<Node<T>>, std::string>;

// struct Node {
// public:
//     vector<expr_node*> children;

//     virtual T operate() = 0;
// };


// template <typename Iterator>

// template<class T>
// struct NodeVariable : public Node<T>{
//     public:
//     T data;

//     NodeVariable(const T& value){
//         data = value;
//     }

//     virtual T operate() override{
//             return data;
//     }
// };

// class NodePlus : public Node<std::string>{
//     public:

//         NodePlus(Node<std::string>* left_operand, Node<std::string>* right_operand){
//             children.push_back(left_operand);
//             children.push_back(right_operand);
//         }

//         virtual std::string operate() override{
//             return "(" +  children[0]->operate() + "+" + children[1]->operate() + ")";
//         } 
// };

// class NodeMinus : public Node<std::string>{
//     public:
//         NodeMinus(Node<std::string>* left_operand, Node<std::string>* right_operand){
//             children.push_back(left_operand);
//             children.push_back(right_operand);
//         }

//         virtual std::string operate() override{
//             return "(" +  children[0]->operate() + "-" + children[1]->operate() + ")";
//         } 
// };

// class NodeMultiply : public Node<std::string>{
//     public:
//         NodeMultiply(Node<std::string>* left_operand, Node<std::string>* right_operand){
//             children.push_back(left_operand);
//             children.push_back(right_operand);
//         }

//         virtual std::string operate() override{
//             return "(" +  children[0]->operate() + "*" + children[1]->operate() + ")";
//         } 
// };

// class NodeDivide : public Node<std::string>{
//     public:
//         NodeDivide(Node<std::string>* left_operand, Node<std::string>* right_operand){
//             children.push_back(left_operand);
//             children.push_back(right_operand);
//         }
//         virtual std::string operate() override{
//             return "(" +  children[0]->operate() + "/" + children[1]->operate() + ")";
//         } 
// };



// template<class T>
// class Tree {
// public:
//     Node<T>* root;

//     Tree(T val) {
//         root = new Node(val);
//     }

//     void add_child(Node<T>* parent, Node<T>* child) {
//         parent->children.push_back(child);
//     }

//     void print_tree(Node<T>* node, int level = 0) {
//         if (node == nullptr) {
//             return;
//         }

//         for (int i = 0; i < level; i++) {
//             cout << "  ";
//         }

//         cout << node->data << endl;

//         for (Node<T>* child : node->children) {
//             print_tree(child, level + 1);
//         }
//     }


//     // auto operate(Node<T>* operator_node){
//     //     if (operator_node->data == "+") {
//     //         return operator_node->children[0]->children->data + operator_node->children[1]->children->data;
//     //     }
//     //     if (operator_node->data == "-") {
//     //         return operator_node->children[0]->children->data - operator_node->children[1]->children->data;
//     //     }
//     //     if (operator_node->data == "*") {
//     //         return operator_node->children[0]->children->data * operator_node->children[1]->children->data;
//     //     }
//     //     if (operator_node->data == "/") {
//     //         return operator_node->children[0]->children->data / operator_node->children[1]->children->data;
//     //     }
//     // }
// };

void print_variant(const std::variant<int, std::string>& v) {
    if (std::holds_alternative<int>(v)) {
        std::cout << "int: " << std::get<int>(v) << std::endl;
    } else {
        std::cout << "string: " << std::get<std::string>(v) << std::endl;
    }
}

int main(int argc, char** argv){
    // Tree<string> t("line1");

    //1 + 5 - 6 * 8
    // Node<std::string>* var_6 = new NodeVariable<std::string>("6");
    // Node<std::string>* var_8 = new NodeVariable<std::string>("8");
    // Node<std::string>* var_5 = new NodeVariable<std::string>("5");
    // Node<std::string>* var_1 = new NodeVariable<std::string>("1");
    // Node<std::string>* node_mult = new NodeMultiply(var_6, var_8);
    // Node<std::string>* node_minus = new NodeMinus(var_5, node_mult);
    // Node<std::string>* node_plus = new NodePlus(var_1, node_minus);
    // std::cout << node_plus->operate() << std::endl;
    // NodePlus<string>* operator_plus = new Node<string>("+");
    // auto left_operand = new Node<string>("a");
    // auto right_operand = new Node<string>("b");
    // auto l_value = new Node<string>("5");
    // auto r_value = new Node<string>("8");

    // t.add_child(t.root, operator_plus);
    // t.add_child(operator_plus, left_operand);
    // t.add_child(operator_plus, right_operand);
    // t.add_child(left_operand, l_value);
    // t.add_child(right_operand, r_value);

    // t.print_tree(t.root);

    // std::cout << operator_plus->children[0]->data << std::endl;
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tAn employee parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout
        << "Give me an employee of the form :"
        << "employee{age, \"surname\", \"forename\", salary } \n";
    std::cout << "Type [q or Q] to quit\n\n";

    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef SLATT_grammar<iterator_type> SLATT_parser;

    SLATT_parser g; // Our grammar
    std::string str = "{ variable = 4 }";

    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    [[maybe_unused]] bool r = phrase_parse(iter, end, &g, space);

    // if (r && iter == end)
    // {
    //     std::cout << boost::fusion::tuple_open('[');
    //     std::cout << boost::fusion::tuple_close(']'); 
    //     std::cout << boost::fusion::tuple_delimiter(", ");

    //     std::cout << "-------------------------\n";
    //     std::cout << "Parsing succeeded\n";
    //     std::cout << "got: " << boost::fusion::as_vector(emp) << std::endl;
    //     std::cout << "\n-------------------------\n";
    // }
    // else
    // {
    //     std::cout << "-------------------------\n";
    //     std::cout << "Parsing failed\n";
    //     std::cout << "-------------------------\n";
    // }

    std::cout << "Bye... :-) \n\n";

    return 0;
}