
#ifndef PARSER_JSON_HPP
#define PARSER_JSON_HPP

#include <string>
#include <stdint.h>
#include <assert.h>

#define nullptr 0

namespace vodk {
namespace json {

enum Type {
    TYPE_UNKNOWN, TYPE_OBJECT, TYPE_ARRAY, TYPE_BOOLEAN, TYPE_NULL, TYPE_STRING, TYPE_NUMBER
};

enum Error {
    ERROR_UNEXPECTED_OBJECT_END,
    ERROR_UNEXPECTED_ARRAY_END,
    ERROR_UNEXPECTED_COLON,
    ERROR_UNEXPECTED_COMMA,
    ERROR_UNEXPECTED_STRING,
    ERROR_UNEXPECTED_VALUE,
    ERROR_UNTERMINATED_STRING,
    ERROR_SYNTAX
};

class Node;

class Parser {
public:
    Parser();

    bool parse(const std::string& src);
    bool parse(const char* src, int nChars);

    // TODO
    // template<typename CharStream> read(CharStream& s);

    void reset();
    bool is_finished() const;
protected:
    virtual void on_object_start(const json::Node*) {}
    virtual void on_object_end(const  json::Node*) {}
    virtual void on_array_start(const json::Node*) {}
    virtual void on_array_end(const json::Node*) {}
    virtual void on_value(const json::Node*) {}
    virtual void on_error(json::Error err);
    virtual void on_reset() {}

    void abort();

private:
    bool is_in_array() const;
    bool is_in_Object() const;
    void pop_node();

    json::Node* _currentNode;
    std::string _currentName;
    int _currentIndex;
    int _state;
    uint64_t _nextID;
    //std::vector<char> _buffer;
};


class Node {
public:
    json::Type get_type() const {return _type; }
    const std::string& get_name() const { return _name; }
    const int get_index() const { return _index; }
    uint64_t get_id() const { return _id; }

    const Node* getParent() const { return _parent; }
    bool is_in_array() const { return _parent ? _parent->_type == TYPE_ARRAY : false; }
    bool is_in_Object() const { return _parent ? _parent->_type == TYPE_OBJECT : false; }
    int get_depth() const { return _parent ? _parent->get_depth() + 1 : 0; }

    bool is_boolean() const { return _type == TYPE_BOOLEAN; }
    bool is_number() const { return _type == TYPE_NUMBER; }
    bool is_string() const { return _type == TYPE_STRING; }
    bool is_null() const { return _type == TYPE_NULL; }

    const bool get_boolean() const {
        assert(is_boolean());
        return _boolean;
    }
    const double get_number() const {
        assert(is_number());
        return _number;
    }
    const std::string& get_string() const {
        assert(is_string());
        return _str;
    }

protected:
    Node(uint64_t id, Node* parentNode, Type nodeType, const std::string& nodeName, int idx)
    : _id(id)
    , _parent(parentNode)
    , _type(nodeType)
    , _name(nodeName)
    , _index(idx)
    {}

    uint64_t    _id;
    json::Node* _parent;
    json::Type  _type;
    std::string _name;
    int         _index;
    bool        _boolean;
    std::string _str;
    double      _number;
    friend class Parser;
};

bool read_number(const char* src, int maxChars, int* nCharsRead);


} // json
} // vodk

#endif
